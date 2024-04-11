#!/usr/bin/python3

import cgi
import cgitb; cgitb.enable()  # This enables CGI error reporting
from html import escape  # Import the escape function from the html module
import os  # Import the os module to access environment variables

def main():
    # Create instance of FieldStorage to get data from the form
    form = cgi.FieldStorage()

    # Get the name from the query parameter 'name', default to 'World' if not provided
    name = form.getvalue('name', 'World')

    # Retrieve the QUERY_STRING environment variable
    query_string = os.environ.get('QUERY_STRING', 'No Query String')

    # HTML response using html.escape to prevent XSS attacks
    print(f"""<!DOCTYPE html>
    <html>
    <head>
        <title>Hello, {escape(name)}!</title>
    </head>
    <body>
        <h1>Hello, {escape(name)}!</h1>
    </body>
    </html>
    """)

if __name__ == "__main__":
    main()
