CREATE TABLE portfolio (
id INTEGER PRIMARY KEY NOT NULL,
symbol VARCHAR(10),
name VARCHAR(30),
shares INTEGER,
price DECIMAL(18,2),
user_id INTEGER NOT NULL,
FOREIGN KEY (user_id) REFERENCES users(id)
);