-- Keep a log of any SQL queries you execute as you solve the mystery.

-- TABLES
-- airports                  crime_scene_reports       people
-- atm_transactions          flights                   phone_calls
-- bank_accounts             interviews
-- courthouse_security_logs  passengers

-- looking for descriptions of the crime
SELECT description FROM crime_scene_reports
WHERE street = 'Chamberlin Street'
AND day = 28
AND month = 7;
-- Theft of the CS50 duck took place at |||10:15am||| at the Chamberlin Street |||courthouse|||.
-- Interviews were conducted |||today||| with |||three witnesses||| who were present at the time — each of their interview transcripts mentions the courthouse.

-- .schema interviews
-- Looking for what the interviews say.

SELECT name, transcript FROM interviews
WHERE day = 28
AND month = 7
AND year = 2020
AND transcript LIKE '%courthouse%';

-- IMPORTANT NOTES:
-- Ruth | Sometime within |||ten minutes of the theft||| 10:15 - 10:25, I saw the thief get into a |||car||| in the |||courthouse parking||| lot and drive away.
-- If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that |||time frame|||.

-- Eugene | I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at the courthouse, I was walking by the |||ATM||| on |||Fifer Street||| and saw the thief there withdrawing some money.

-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for |||less than a minute.|||
-- In the call, I heard the thief say that they were planning to take the |||earliest flight||| out of |||Fiftyville tomorrow|||.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- courthouse log information
-- .schema courthouse_security_logs
SELECT license_plate FROM courthouse_security_logs
WHERE day = 28
AND month = 7
AND hour = 10
AND minute BETWEEN 15 AND 25
AND activity = 'exit';

-- atm information AND bank information
-- .schema atm_transactions
-- .schema bank_accounts

SELECT bank_accounts.person_id FROM bank_accounts
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.day = 28
AND atm_transactions.month = 7
AND atm_transactions.year = 2020
AND atm_transactions.atm_location = 'Fifer Street'
AND atm_transactions.transaction_type = 'withdraw';

-- phone call information
-- .schema phone_calls
SELECT caller FROM phone_calls
WHERE day = 28
AND month = 7
AND year = 2020
AND duration < 60;

-- flight information
-- .schema airports
-- .schema flights
-- .schema passengers
SELECT flights.id, flights.hour, flights.minute FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE city = 'Fiftyville'
AND flights.day = 29
AND flights.month = 7
AND flights.year = 2020;

-- flight 36 | 8 | 20


SELECT DISTINCT passport_number FROM passengers
JOIN flights ON passengers.flight_id = (36);

-- .schema people

SELECT name FROM people
WHERE phone_number IN (
    SELECT caller FROM phone_calls
    WHERE day = 28
    AND month = 7
    AND year = 2020
    AND duration < 60)
AND passport_number IN (
    SELECT DISTINCT passport_number FROM passengers
    JOIN flights ON passengers.flight_id = (36))
AND license_plate IN (
    SELECT license_plate FROM courthouse_security_logs
    WHERE day = 28
    AND month = 7
    AND hour = 10
    AND minute BETWEEN 15 AND 25
    AND activity = 'exit')
AND id IN (
    SELECT bank_accounts.person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.day = 28
    AND atm_transactions.month = 7
    AND atm_transactions.year = 2020
    AND atm_transactions.atm_location = 'Fifer Street'
    AND atm_transactions.transaction_type = 'withdraw');

-- Ernest

SELECT name, phone_number, passport_number FROM people
WHERE name = 'Ernest';

-- destination:
SELECT city FROM airports
WHERE id IN (
    SELECT destination_airport_id FROM flights
    WHERE day = 29
    AND month = 7
    AND year = 2020
    AND flights.id = 36);


-- ACCOMPLICE:
SELECT name FROM people
WHERE phone_number = (
    SELECT receiver FROM phone_calls
    WHERE caller = (SELECT phone_number FROM people WHERE name = 'Ernest')
    AND day = 28
    AND month = 7
    AND year = 2020
    AND duration < 60);





