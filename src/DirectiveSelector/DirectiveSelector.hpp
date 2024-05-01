#pragma once
#include "defines.h"
#include "parsing.hpp"

class DirectiveSelector {

  public:
    // Constructors & Destructors
    DirectiveSelector(t_server &server, std::string &uri);
    ~DirectiveSelector(void);

    std::string getRoot(void) const;
    std::map<unsigned int, std::string> getErrorPages(void) const;
    std::vector<std::string> getAllowedMethods(void) const;
    std::string getUploadPath(void) const;
    std::string getIndex(void) const;
    bool getAutoindex(void) const;
    std::string getPath(void) const;
    std::string getRedirect_to(void) const;
    int getRedirect_code(void) const;
    std::map<std::string, std::string> getCgi(void) const;
    unsigned long long getMaxBodySize(void) const;

  private:
    // Methods
    // Attributes
    t_server _server;
	t_location _location;
    int _location_index;
    const t_location &getLocation(void) const;
};
