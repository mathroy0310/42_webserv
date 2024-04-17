#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // Output the CGI content type header
    std::cout << "Content-type: text/html\r\n\r\n";

    // Output the HTML content
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>Upload CGI Test</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<h2>Upload CGI Test</h2>\n";

    // Check if POST data is available
    if (getenv("CONTENT_LENGTH")) {
        // Get the content length
        int content_length = std::stoi(std::getenv("CONTENT_LENGTH"));

        // Read POST data
        char *post_data = new char[content_length + 1];
        std::cin.read(post_data, content_length);
        post_data[content_length] = '\0';

        // Output uploaded file data
        std::cout << "<p>Received POST data:</p>\n";
        std::cout << "<pre>" << post_data << "</pre>\n";

        // Free memory
        delete[] post_data;
    } else {
        std::cout << "<p>No POST data received.</p>\n";
    }

    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}