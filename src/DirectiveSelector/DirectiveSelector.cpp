#include "DirectiveSelector.hpp"

static int findLocationIndex(std::string &uri, std::vector<t_location> locations) {
    std::vector<std::string> location_paths;

    for (size_t i = 0; i < locations.size(); i++)
        location_paths.push_back(locations[i].path);
    std::string look_for = uri;
    while (look_for.size() > 0) {
        std::vector<std::string>::iterator it = std::find(location_paths.begin(), location_paths.end(), look_for);
        if (it != location_paths.end())
            return (std::distance(location_paths.begin(), it));
        size_t last = look_for.find_last_of('/');
        if (last == look_for.npos)
            break;
        look_for.erase(last, -1);
    }
    if (uri[0] == '/' && look_for.empty()) {
        std::vector<std::string>::iterator it = std::find(location_paths.begin(), location_paths.end(), "/");
        if (it != location_paths.end())
            return (std::distance(location_paths.begin(), it));
    }
    return (-1);
}

// Constructors & Destructors
DirectiveSelector::DirectiveSelector(t_server &server, std::string &uri) {
	this->_server = server;
    this->_location_index = findLocationIndex(uri, server.locations);
	Logger::get().log(DEBUG, "Location index: %d", this->_location_index);
	if (this->_location_index < 0)
		return ;
	this->_location = this->_server.locations[this->_location_index];
}


DirectiveSelector::~DirectiveSelector(void) {}

// Methods
std::string DirectiveSelector::getRoot(void) const {
	if (this->_location_index < 0)
		return (this->_server.root);
	if (this->_location.root.empty())
		return (this->_server.root);
	return (this->_location.root);
}

std::map<unsigned int, std::string> DirectiveSelector::getErrorPages(void) const {
	if (this->_location_index < 0)
		return (this->_server.error_pages);
	if (this->_location.error_pages.empty())
		return (this->_server.error_pages);
	return (this->_location.error_pages);
	 
}

std::vector<std::string> DirectiveSelector::getAllowedMethods(void) const {
	if (this->_location_index < 0)
		return (this->_server.allowed_methods);
	Logger::get().log(DEBUG, "Allowed methods");
	if (this->_location.allowed_methods.empty())
		return (this->_server.allowed_methods);
	return (this->_location.allowed_methods);
}

std::string DirectiveSelector::getUploadPath(void) const {
	if (this->_location_index < 0)
		return (this->_server.upload_path);
	if (this->_location.upload_path.empty())
		return (this->_server.upload_path);
	return (this->_location.upload_path);
}

std::string DirectiveSelector::getIndex(void) const {
	if (this->_location_index < 0)
		return (this->_server.index);
	if (this->_location.index.empty())
		return (this->_server.index);
	return (this->_location.index);
}

bool DirectiveSelector::getAutoindex(void) const {
	if (this->_location_index < 0)
		return (this->_server.is_autoindex);
	if (this->_location.is_autoindex == false)
		return (this->_server.is_autoindex);
	return (this->_location.is_autoindex);
}

std::string DirectiveSelector::getPath(void) const {
	if (this->_location_index < 0)
		return ("");
	return (this->_location.path);
}

std::string DirectiveSelector::getRedirect_to(void) const {
	if (this->_location_index < 0)
		return (this->_server.redirect_to);
	if (this->_location.redirect_to.empty())
		return (this->_server.redirect_to);
	return (this->_location.redirect_to);
}

int DirectiveSelector::getRedirect_code(void) const {
	if (this->_location_index < 0)
		return (this->_server.redirect_code);
	if (this->_location.redirect_code < 0)
		return (this->_server.redirect_code);
	return (this->_location.redirect_code);
}

std::map<std::string,std::string> DirectiveSelector::getCgi(void) const {
	if (this->_location_index < 0)
		return (this->_server.cgi);
	if (this->_location.cgi.empty())
		return (this->_server.cgi);
	return (this->_location.cgi);
}

unsigned long long DirectiveSelector::getMaxBodySize(void) const {
	if (this->_location_index < 0)
		return (this->_server.max_body_size);
	if (this->_location.max_body_size == 0)
		return (this->_server.max_body_size);
	return (this->_location.max_body_size);
}

const t_location &DirectiveSelector::getLocation(void) const {
    if (this->_location_index < 0)
        throw std::out_of_range("getLocation()");
    return (this->_location);
}
