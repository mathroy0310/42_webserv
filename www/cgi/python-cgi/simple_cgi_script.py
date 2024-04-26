#!/usr/bin/python3

import cgi
import cgitb; cgitb.enable()  # This enables CGI error reporting
from html import escape  # Import the escape function from the html module
import os  # Import the os module to access environment variables

def main():
    form = cgi.FieldStorage()  # Get data from the form
    name = form.getvalue('name', 'World')  # Get 'name' parameter
    print(f"""Content-type: text/html\r\n
    <!DOCTYPE html>
    <html>
    <head>
        <title>Hello, {escape(name)}!</title>
        <style>
            body {{
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                font-family: Arial, sans-serif;
            }}
            div {{
                text-align: center;
            }}
        </style>
    </head>
    <body>
        <div>
            <h1>Hello, {escape(name)}!</h1>
        </div>
    </body>
    </html>
    """)  # Print HTML response

if __name__ == "__main__":
    main()