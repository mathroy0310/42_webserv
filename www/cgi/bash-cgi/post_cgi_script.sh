#!/bin/bash

# Send HTTP headers
echo -en "HTTP/1.1 200 OK\r\n"
echo -en "Content-type: text/html\r\n"
echo -en "\r\n"

# Send HTML body
echo -en "<!DOCTYPE html>\r\n"
echo -en "<html>\r\n"
echo -en "<head>\r\n"
echo -en "<title>POST Request</title>\r\n"
echo -en "<style>\r\n"
echo -en "body {\r\n"
echo -en "  display: flex;\r\n"
echo -en "  justify-content: center;\r\n"
echo -en "  align-items: center;\r\n"
echo -en "  height: 100vh;\r\n"
echo -en "  font-family: Arial, sans-serif;\r\n"
echo -en "}\r\n"
echo -en "div {\r\n"
echo -en "  text-align: center;\r\n"
echo -en "}\r\n"
echo -en "</style>\r\n"
echo -en "</head>\r\n"
echo -en "<body>\r\n"
echo -en "<div>\r\n"
echo -en "<h1>POST Request Received</h1>\r\n"
echo -en "<h2>CGI Script is Working</h2>\r\n"
echo -en "<h3>Request Method: $REQUEST_METHOD</h3>\r\n"
echo -en "<h3>Server Protocol: $SERVER_PROTOCOL</h3>\r\n"
echo -en "<h3>Content Length: $CONTENT_LENGTH</h3>\r\n"
echo -en "<h3>Server Name: $SERVER_NAME</h3>\r\n"
echo -en "<h3>Server Port: $SERVER_PORT</h3>\r\n"
echo -en "</div>\r\n"
echo -en "</body>\r\n"
echo -en "</html>\r\n"