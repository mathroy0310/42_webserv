#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Starting comprehensive server tests...${NC}"

# Function to log and check responses
log_and_check() {
    local response=$1
    local expected=$2
    local test_name=$3

    if [ "$response" -eq "$expected" ]; then
        echo -e "${GREEN}$test_name: PASS${NC}"
    else
        echo -e "${RED}$test_name: FAIL${NC}"
    fi
}

# Test GET Request
echo -e "Testing GET request for existing resource..."
response=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:8080/)
log_and_check $response 200 "GET request for existing resource"

# Test POST Request
echo -e "Testing POST request to upload data with multipart/form-data..."
# Assuming 'Example.png' is a file in the current directory that you want to upload
response=$(curl -s -o /dev/null -w "%{http_code}" -X POST -F "file=@Example.png;type=image/png" http://localhost:8080/upload)
log_and_check $response 201 "POST request to upload data"

# Test DELETE Request
echo -e "Testing DELETE request for existing resource..."
response=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE http://localhost:8080/upload/100MB.bin)
log_and_check $response 204 "DELETE request for existing resource"

# Test UNKNOWN Request
echo -e "Testing UNKNOWN method request..."
response=$(curl -s -o /dev/null -w "%{http_code}" -X UNKNOWN http://localhost:8080)
log_and_check $response 501 "UNKNOWN method request"

# File Upload
echo -e "Testing file upload..."
upload_response=$(curl -s -o /dev/null -w "%{http_code}" -X POST -F "file=@/path/to/localfile.txt" http://localhost:8080/upload)
log_and_check $upload_response 201 "File upload"

# File Retrieval
echo -e "Testing file retrieval..."
retrieval_response=$(curl -s -o downloadedfile.txt -w "%{http_code}" http://localhost:8080/uploadedfile.txt)
log_and_check $retrieval_response 200 "File retrieval"

echo -e "${GREEN}All tests completed.${NC}"
