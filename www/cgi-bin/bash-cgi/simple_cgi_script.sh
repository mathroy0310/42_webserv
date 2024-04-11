#!/usr/bin/bash
# Read the value of 'name' parameter from the query string
name=$(echo "$QUERY_STRING" | awk -F '=' '{print $2}')

echo "HTTP/1.1 200 OK"
echo "Content-type: text/html"

# Set content type to HTML
echo ""

# Output HTML response with the formatted name
echo "<!DOCTYPE html>"
echo "<html>"
echo "<head>"
echo "<title>Hello, $name!</title>"
echo "</head>"
echo "<body>"
echo "<h1>Hello, $name!</h1>"
echo "</body>"
echo "</html>"
