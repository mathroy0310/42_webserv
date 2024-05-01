#!/bin/bash

# Set default values
count=${1:-1000}  # Default number of hits to be 1000 if not specified
concurrent=${2:-10}  # Default number of concurrent users to be 10
duration=${3:-"1M"}  # Default duration of the test to be 1 minute

# File containing the URLs to be tested
url_file="urls.txt"

# Check if urls.txt exists, if not default to a single URL
if [ ! -f "$url_file" ]; then
    echo "http://localhost:8080" > urls.txt
fi

# Run siege
siege -c$concurrent -t$duration -f $url_file -b

# Note: The `-b` option runs siege in benchmark mode, which disables delays between requests.
