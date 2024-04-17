#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    // Output the CGI content type header
    std::cout << "Content-type: text/html\r\n\r\n";

    // Retrieve the query string from the environment
    std::string query = std::getenv("QUERY_STRING");

    // Output the HTML content
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>Hello " << query << "</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<h1>Hello " << query << "</h1>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}