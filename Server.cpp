#include "Server.hpp"
#include "./Config/Parsing.hpp"

Server::Server() : _name(""), host(""), _listen("80"), _method(), _locations(), cgi(), _root(""), _index(""), _body_size("100000000"), _autoindex(""), _upload_path("")
{
    _error_pages["400"] = "errors/400.html";
    _error_pages["401"] = "errors/401.html";
    _error_pages["403"] = "errors/403.html";
    _error_pages["404"] = "errors/404.html";
    _error_pages["500"] = "errors/500.html";
    _error_pages["502"] = "errors/502.html";
    _error_pages["503"] = "errors/503.html";
    _error_pages["504"] = "errors/504.html";
    _error_pages["429"] = "errors/429.html";
}

Server::Server(const Server &copy)
{
    *this = copy;
}

Server &Server::operator=(const Server &copy)
{
    if (this != &copy)
    {
        this->_name = copy._name;
        this->host = copy.host;
        this->_listen = copy._listen;
        this->_method = copy._method;
        this->_error_pages = copy._error_pages;
        this->_locations = copy._locations;
        this->_body_size = copy._body_size;
        this->cgi = copy.cgi;
        this->_root = copy._root;
        this->_index = copy._index;
        this->_upload_path = copy._upload_path;
        this->_autoindex = copy._autoindex;
    }
    return (*this);
}

Server::~Server(){}

// ################## setters ########################################
void Server::setServerName(std::string _name)
{
    if (this->_name == "")
    {
        this->_name = _name;
    }
    else
        ft_Error(1, "is Duplicate");
}
void Server::setHost(std::string host)
{
    if (this->host == "")
    {
        this->host = host;
    }
    else
       ft_Error(1, "is Duplicate");
}
void Server::setPort(std::string _listen)
{
    int port = std::atoi(_listen.c_str());
    if (this->_listen == "80")
    {
        if (port >= 0 && port <= 65353)
            this->_listen = _listen;
        else
            ft_Error(1, "not is of range");
    }
    else
        ft_Error(1, "is Duplicate");

}
void Server::setMethods(std::vector<std::string> methods)
{
    std::vector<std::string> allow;
   for (size_t i = 0; i < methods.size(); i++)
    {
        if (Check_semicolon(methods[i]) == 1)
            methods[i] = remove_semicolon(methods[i]);
        if (methods[i] == "GET" || methods[i] == "POST" || methods[i] == "DELETE")
            allow.push_back(methods[i]);
        if (methods[i] == ";")
            continue;
        if (methods[i] != "GET" && methods[i] != "POST" && methods[i] != "DELETE")
            ft_Error(2, methods[i]);
    }
    if (this->_method.empty())
        this->_method = allow;
    else
        ft_Error(1, " is Duplicate-method_server");

}
void Server::setErrorPages(std::map<std::string, std::string> _error_pages)
{
    this->_error_pages = _error_pages;
}
void Server::setLocations(std::vector<Location *> _locations)
{
    this->_locations = _locations;
}
void Server::setCgi(std::map<std::string, std::string> cgi)
{
    this->cgi = cgi;
}
void Server::setRoot(std::string _root)
{
    if (this->_root == "")
        this->_root = _root;
    else
        ft_Error(1, "is Duplicate");
}
void Server::setIndex(std::string index)
{
    if (this->_index == "")
        this->_index = index;
    else
       ft_Error(1, "is Duplicate");
}
void Server::setMaxClientBodySize(std::string _body_size)
{

    if (_body_size.size() > 10)
    {
        ft_Error(1, "not is of range of max  unsigned int <...>");
        exit(1);
    }
     unsigned long long body_max = std::strtoull(_body_size.c_str(), NULL, 10);
    if (this->_body_size == "100000000")
    {
        if (body_max > 0  && body_max <= UINT_MAX)
            this->_body_size = _body_size;
        else
            ft_Error(1, "not is of range of max unsigned int");
    }
    else
        ft_Error(1, "is Duplicate");
}
void Server::setautoindex(std::string word)
{
    if (this->_autoindex == "")
        this->_autoindex = word;
    else
       ft_Error(1, " is Duplicate");
}
void Server::setUploadPath(std::string upload_path)
{
    if (this->_upload_path == "")
        this->_upload_path = upload_path;
    else
       ft_Error(1, " is Duplicate");
}

// ################### fin setters #################################################

// ################### function ####################################################
void Server::addMethod(std::string method)
{
    this->_method.push_back(method);
}
void Server::addErrorPage(std::string error_code, std::string error_page)
{
    std::map<std::string, std::string> copy = getError();
    std::map<std::string, std::string>::iterator it = copy.begin();
    for (size_t len = 0; len < getError().size(); len++)
    {
        if (it->first == error_code)
        {
            it->second = error_page;
        }
        it++;
    }
    this->_error_pages.insert(std::pair<std::string, std::string>(error_code, error_page));
}
void Server::addLocation(Location *location)
{
    this->_locations.push_back(location);
}
void Server::addCgi(std::string extension, std::string cgi)
{
    this->cgi.insert(std::pair<std::string, std::string>(extension, cgi));
}

// ##################### fin function  ###########################################################

// ##################### getters ###########################################################
std::string const &Server::getName() const
{
    return (this->_name);
}
std::string const &Server::getHost() const
{
    return (this->host);
}
std::string const &Server::getListen() const
{
    return (this->_listen);
}
std::vector<std::string> const &Server::getMethod() const
{
    return (this->_method);
}
std::map<std::string, std::string> const &Server::getError() const
{
    return (this->_error_pages);
}
std::vector<Location *> const &Server::getLocation() const
{
    return (this->_locations);
}
std::map<std::string, std::string> const &Server::getCgi() const
{
    return (this->cgi);
}
std::string const &Server::getRoot() const
{
    return (this->_root);
}
std::string const &Server::getIndex() const
{
    return (this->_index);
}
std::string const &Server::getBody() const
{
    return (this->_body_size);
}

std::string const &Server::getautoindex() const
{
    return (this->_autoindex);
}
std::string const &Server::getUploadPath() const
{
    return (this->_upload_path);
}
