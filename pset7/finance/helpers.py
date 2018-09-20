import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        response = requests.get(f"https://api.iextrading.com/1.0/stock/{urllib.parse.quote_plus(symbol)}/quote")
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def stock_report(transaction_history):
    """Generates stock report from transaction history."""

    report = []

    # transaction histories are already summed by stock
    for item in transaction_history:
        if not (item["SUM(shares)"]) == 0:
            report_item = lookup(item["symbol"])
            report_item["shares"] = item["SUM(shares)"]
            report_item["value"] = item["SUM(shares)"] * report_item["price"]
            report_item["price"] = report_item["price"]
            report.append(report_item)

    return report


def usd_stock_report(stock_report):
    """Converts money in stock report to USD"""

    # "price" and "value" need to be converted
    for item in stock_report:
        item["price"] = usd(item["price"])
        item["value"] = usd(item["value"])

    return stock_report