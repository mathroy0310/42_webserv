/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory_listing.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:48:47 by maroy             #+#    #+#             */
/*   Updated: 2024/04/05 17:50:36 by maroy            ###   ########.fr       */
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
    response = "<html><body><ul>";

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (root[root.length() - 1] != '/') {
            root.insert(root.length(), "/");
        }
        response += "<li><a href=\"" + root + std::string(entry->d_name) + (entry->d_type == DT_DIR ? "/\">" : "\">") +
                    std::string(entry->d_name) + "</a></li>";
    }
    response += "</ul></body></html>";
    return (response);
}