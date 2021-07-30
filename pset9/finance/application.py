import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    portfolio = db.execute("SELECT symbol, SUM(shares) AS shares, price FROM portfolio WHERE user_id = ? GROUP BY symbol", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    total = cash[0]['cash']
    cash = cash[0]['cash']

    for i in range(len(portfolio)):
        data = lookup(portfolio[i]["symbol"])
        portfolio[i]["price"] = data["price"]
        portfolio[i]["name"] = data["name"]
        portfolio[i]["total"] = portfolio[i]["price"] * portfolio[i]["shares"]
        total += portfolio[i]["total"]

    return render_template("index.html", portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        user_id = session["user_id"]

        stock = lookup(request.form.get("symbol"))
        if stock:
            shares = (request.form.get("shares"))
            try:
                int(shares)
            except:
                return apology("invalid data type", 400)

            shares = int(shares)

            if shares < 0:
                return apology("invalid data type", 400)

            purchase = shares * stock["price"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

            if purchase > cash[0]['cash']:

                flash("You don't have enough Cash")
                return redirect("/buy")

            else:

                db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", purchase, user_id)

                dt = db.execute("SELECT datetime()")
                db.execute(
                    "INSERT INTO portfolio (symbol, shares, price, user_id, operation, operation_time) VALUES(?, ?, ?, ?, ?, ?)",
                    stock["symbol"], shares, stock["price"], user_id, "b", dt[0]["datetime()"])

                flash("Bought!")

                return redirect("/")
        else:
            return apology("invalid stock", 400)

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    portfolio = db.execute("SELECT symbol, shares, price, operation_time FROM portfolio WHERE user_id = ?", user_id)

    return render_template("history.html", portfolio=portfolio)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        if stock:
            stock_text = ("A share of {}. ({}) costs {}.").format(stock['name'], stock['symbol'], usd(stock['price']))
            return render_template("quote.html", stock_text=stock_text)
        else:
            return apology("invalid stock", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must be the same", 400)

        username = request.form.get("username")
        password = request.form.get("password")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username do not exist yet
        if len(rows) > 0:
            return apology("username already exists", 400)

        # Query database for username
        db.execute("INSERT INTO users (username, hash, cash) VALUES (?, ?, 10000)", username, generate_password_hash(password))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocks_list = db.execute("SELECT symbol, SUM(shares) AS shares FROM portfolio WHERE user_id = ? GROUP BY symbol", user_id)

    if request.method == "POST":

        shares_max = 0
        for i in range(len(stocks_list)):
            if stocks_list[i]["symbol"] == request.form.get("symbol"):
                shares_max = stocks_list[i]["shares"]

        stock = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        try:
            int(shares)
        except:
            return apology("invalid data type", 400)

        shares = int(shares)

        if shares < shares_max:
            shares *= (-1)
            purchase = shares * stock["price"]

            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", purchase, user_id)

            dt = db.execute("SELECT datetime()")
            db.execute(
                "INSERT INTO portfolio (symbol, shares, price, user_id, operation, operation_time) VALUES(?, ?, ?, ?, ?, ?)",
                stock["symbol"], shares, stock["price"], user_id, "s", dt[0]["datetime()"])

            flash("Sold!")

            return redirect("/")

        else:
            return apology("you don't have that many shares", 400)

    else:
        return render_template("sell.html", stocks_list=stocks_list)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
