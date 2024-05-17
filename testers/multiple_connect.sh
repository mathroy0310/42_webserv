#!/bin/bash

# Set default iteration count to 1000
count=${1:-1000}

# Loop for 'count' times
for ((i=1; i<=count; i++))
do
    # Send a GET request to localhost:8080 and capture the output
    response=$(echo -e "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n" | nc localhost 8080)

    # Use awk to extract the HTTP status code and the length of the response
    status=$(echo "$response" | grep "HTTP/1.1" | awk '{print $2}')
    length=$(echo "$response" | wc -c)

    # Output the status and length
    echo "Status: $status, Data received: $length bytes"

    # Sleep for 0.2 seconds
    
done
