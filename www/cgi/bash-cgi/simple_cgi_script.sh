#!/usr/bin/bash

# Read the value of 'name' parameter from the query string
name=$(echo "$QUERY_STRING" | awk -F '=' '{print $2}' | sed 's/+/ /g')

# Output HTML response with the formatted name
echo "Content-type: text/html"
echo ""
echo "<!DOCTYPE html>"
echo "<html>"
echo "<head>"
echo "<title>Hello, $name!</title>"
echo "<style>"
echo "body {"
echo "  display: flex;"
echo "  justify-content: center;"
echo "  align-items: center;"
echo "  height: 100vh;"
echo "  font-family: Arial, sans-serif;"
echo "}"
echo "div {"
echo "  text-align: center;"
echo "}"
echo "</style>"
echo "</head>"
echo "<body>"
echo "<div>"
echo "<h1>Hello, $name!</h1>"
echo "</div>"
echo "</body>"
echo "</html>"