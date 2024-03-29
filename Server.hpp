#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Location.hpp"
#include <cstdlib>
#include <climits>


class Server
{
private:
    std::string _name;
    std::string host;
    std::string _listen;
    std::vector<std::string> _method;
    std::map<std::string , std::string> _error_pages;
    std::vector<Location *> _locations;

    std::map<std::string, std::string> cgi;
    std::string _root;
    std::string _index;
    std::string _body_size;
    std::string	_autoindex;
    std::string _upload_path;

public:
    Server(void);
    Server(const Server &copy);
    Server &operator=(const Server &copy);
    ~Server(void);

    void setServerName(std::string server_name);
    void setHost(std::string host);
    void setPort(std::string _listen);
    void setErrorPages(std::map<std::string, std::string> error_pages);
    void setLocations(std::vector<Location *> locations);
    void setMaxClientBodySize(std::string max_client_body_size);

//++++++++++
    void setMethods(std::vector<std::string> methods);
    void setCgi(std::map<std::string, std::string> cgi);
    void setRoot(std::string root);
    void setIndex(std::string index);
    void setautoindex(std::string word);
    void setUploadPath(std::string upload_path);
//++++++++++


    void addMethod(std::string method);
    void addErrorPage(std::string error_code, std::string error_page);
    void addLocation(Location *location);

    //++++++++++
    void addCgi(std::string extension, std::string cgi);
//++++++++++

    std::string const &getName(void) const;
    std::string const &getHost(void) const;
    std::string const &getListen(void) const;
    std::map<std::string, std::string> const &getError(void) const;
    std::vector<Location *> const &getLocation(void) const;
    std::string const &getBody(void) const;
//++++++++++
    std::vector<std::string> const &getMethod(void) const;
    std::map<std::string, std::string> const &getCgi(void) const;
    std::string const &getRoot(void) const;
    std::string const &getIndex(void) const;
    std::string const &getautoindex() const;
    std::string const &getUploadPath(void) const;
    //++++++++++

    void print(void) const;
};
