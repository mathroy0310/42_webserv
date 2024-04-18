#!/bin/bash

# Set Content-Type header

echo "Content-Type: text/html"
echo ""
echo "<!DOCTYPE html>"
echo "<html>"
echo "<head>"
echo "<title>POST Request</title>"
echo "</head>"
echo "<body>"
echo "<h1>POST Request</h1>"
echo "<p>REQUEST_METHOD: $REQUEST_METHOD</p>"
echo "<p>SERVER_PORT: $HTTP_SERVER_PORT</p>"
echo "</body>"
echo "</html>"