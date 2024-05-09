#!/usr/bin/bash

# Read the value of 'name' parameter from the query string
name=$(echo  "$QUERY_STRING" | awk -F '=' '{print $2}' | sed 's/+/ /g')

# Output HTML response with the formatted name
echo -en  "Content-type: text/html\r\n"
echo -en  "\r\n"
echo -en  "<!DOCTYPE html>\r\n"
echo -en  "<html>\r\n"
echo -en  "<head>\r\n"
echo -en  "<title>Hello, $name!</title>\r\n"
echo -en  "<style>\r\n"
echo -en  "body {\r\n"
echo -en  "  display: flex;\r\n"
echo -en  "  justify-content: center;\r\n"
echo -en  "  align-items: center;\r\n"
echo -en  "  height: 100vh;\r\n"
echo -en  "  font-family: Arial, sans-serif;\r\n"
echo -en  "}\r\n"
echo -en  "div {\r\n"
echo -en  "  text-align: center;\r\n"
echo -en  "}\r\n"
echo -en  "</style>\r\n"
echo -en  "</head>\r\n"
echo -en  "<body>\r\n"
echo -en  "<div>\r\n"
echo -en  "<h1>Hello, $name!</h1>\r\n"
echo -en  "</div>\r\n"
echo -en  "</body>\r\n"
echo -en  "</html>\r\n"
