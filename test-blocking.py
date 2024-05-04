# File: stress_test.py
import requests
from concurrent.futures import ThreadPoolExecutor

def send_request(url):
    response = requests.get(url)
    print(f"Status Code: {response.status_code}, Content: {response.text[:0]}")

# URL of your server
url = "http://localhost:8080/"

# Number of concurrent requests
concurrent_requests = 100

num_requests = 0

with ThreadPoolExecutor(max_workers=concurrent_requests) as executor:
    for _ in range(concurrent_requests):
        num_requests += 1
        executor.submit(send_request, url)
    print(f"Sent {num_requests} requests")