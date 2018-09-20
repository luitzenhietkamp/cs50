import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, stock_report, usd_stock_report

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    funds = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    transaction_history = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE user_id = :user_id GROUP BY symbol",
                                     user_id=session["user_id"])
    report = stock_report(transaction_history)
    total_amount = funds[0]["cash"] + sum(item["value"] for item in report)

    return render_template("index.html", cash=usd(funds[0]["cash"]), report=usd_stock_report(report), total_amount=usd(total_amount))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Look up stock
        stock = lookup(request.form.get("symbol"))

        # Stock not found
        if stock == None:
            flash("Stock not found!")
            return apology("Stock not found!", 400)

        # Verify number of shares
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Enter a whole positive number of shares!", 400)

        # Check whether number of shares is positive
        if shares < 1:
            return apology("Enter a whole positive number of shares!", 400)

        # Verify funds
        funds = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        if funds[0]["cash"] < shares * stock["price"]:
            return apology("Not enough funds", 400)

        # User is allowed to buy shares
        # Add transaction to database
        db.execute("INSERT INTO transactions (symbol, shares, price, user_id) VALUES(:symbol, :shares, :price, :user_id)",
                   symbol=request.form.get("symbol"),
                   shares=shares,
                   price=stock["price"],
                   user_id=session["user_id"])

        # Adjust funds
        db.execute("UPDATE users SET cash = cash - :cost WHERE id = :user_id",
                   cost=shares * stock["price"],
                   user_id=session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT symbol, shares, price, time FROM transactions WHERE user_id = :user_id ORDER BY time DESC",
                         user_id=session["user_id"])

    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Look up stock
        stock = lookup(request.form.get("symbol"))

        # Stock not found
        if stock == None:
            flash("Stock not found!")
            return apology("Stock not found!", 400)

        # Stock found
        else:
            return render_template("show_quote.html", name=stock["name"],
                                   price=usd(stock["price"]), symbol=stock["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("get_quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # try to add username and hashed password to db
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :pwhash)",
                            username=request.form.get("username"),
                            pwhash=generate_password_hash(request.form.get("password"),
                                                          method='pbkdf2:sha256', salt_length=8))
        if not result:
            return apology("username already taken", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Select a stock to sell", 400)

        # Verify number of shares
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Enter a whole positive number of shares!", 400)

        # Check whether number of shares is positive
        if shares < 1:
            return apology("Enter a whole positive number of shares!", 400)

        # Check whether sufficient number of shares are in account
        shares_owned = db.execute("SELECT SUM(shares) FROM transactions WHERE symbol = :symbol", symbol=request.form.get("symbol"))
        if shares > shares_owned[0]["SUM(shares)"]:
            return apology("Not enough shares", 400)

        # User is allowed to sell shares
        # Look up stock
        stock = lookup(request.form.get("symbol"))

        # Add transaction to database
        db.execute("INSERT INTO transactions (symbol, shares, price, user_id) VALUES(:symbol, :shares, :price, :user_id)",
                   symbol=request.form.get("symbol"),
                   shares=-shares,
                   price=stock["price"],
                   user_id=session["user_id"])

        # adjust funds
        db.execute("UPDATE users SET cash = cash + :income WHERE id = :user_id",
                   income=shares * stock["price"],
                   user_id=session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # find all the owned stock in the database
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = :user_id AND symbol IN (SELECT symbol FROM transactions GROUP BY symbol HAVING SUM(shares) != 0) GROUP BY symbol",
                             user_id=session["user_id"])

        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def changepw():
    """Change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("old_password"):
            flash("Enter your password!")
            return redirect("/changepw")

        # Ensure new password was submitted
        if not request.form.get("new_password"):
            flash("Enter a new password!")
            return redirect("/changepw")

        # Ensure passwords match
        elif request.form.get("new_password") != request.form.get("confirmation"):
            flash("Confirm new password!")
            return redirect("/changepw")

        # Query database for hashed password
        rows = db.execute("SELECT hash FROM users WHERE id = :user_id",
                          user_id=session["user_id"])

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            flash("Incorrect password")
            return redirect("/changepw")

        # Change password
        db.execute("UPDATE users SET hash = :pwhash WHERE id = :user_id",
                   user_id=session["user_id"],
                   pwhash=generate_password_hash(request.form.get("new_password"),
                                                 method='pbkdf2:sha256', salt_length=8))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change_pw.html")
