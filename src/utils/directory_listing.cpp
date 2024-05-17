/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_listing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:48:47 by maroy             #+#    #+#             */
/*   Updated: 2024/04/07 20:46:46 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"

/**
 * @brief Generates an HTML string representing a directory listing.
 *
 * This function reads the contents of a directory and generates an HTML string that represents a directory listing.
 * Each entry in the directory is represented as a list item (`<li>`) in an unordered list (`<ul>`).
 * Directories are distinguished from files by a trailing slash in the hyperlink.
 *
 * @param dir A pointer to a DIR structure that represents the open directory. This directory is read to generate the
 * listing.
 * @param root A string representing the root path for the directory. This is used to generate the hyperlinks for each
 * directory entry.
 *
 * @return A string containing the HTML representation of the directory listing. The string is a complete HTML document
 * that can be displayed in a web browser.
 */
std::string directory_listing(DIR *dir, std::string root) {
    std::string response;
    response = "<html><head><style>"
               "body { display: flex; flex-direction: column; justify-content: center; align-items: center; height: "
               "100vh; margin: 0; "
               "background-color: #f0f0f0; font-family: Arial, sans-serif; }"
               "ul { list-style-type: none; padding: 10px; text-align: center; background-color: #fff; border-radius: "
               "5px; padding: 20px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); }"
               "li { margin: 15px; min-width: 200px; }"
               ".directory { padding: 2px; margin: 10px; border-radius: 5px; background-color: #f0f0f0;}"
               "</style></head><body><h1>Directory Listing</h1><ul>";

    struct dirent *entry;
    bool is_empty = true;
    while ((entry = readdir(dir))) {
        std::string filename(entry->d_name);
        if (filename == "." || filename == "..")
            continue;
        is_empty = false;
        if (root[root.length() - 1] != '/')
            root.insert(root.length(), "/");
        if (entry->d_type == DT_DIR)
            response += "<li class=\"directory\"><a href=\"" + root + filename + "/\">" + filename + "</a></li>";
        else
            response += "<li><a href=\"" + root + filename + "\">" + filename + "</a></li>";
    }

    if (is_empty)
        response += "<p>Empty Directory</p>"
					"<p><a href=\"" + root + "..\">Go back</a></p>";

    response += "</ul></body></html>";
    return (response);
}