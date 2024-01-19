#include "./Config/Parsing.hpp"

Location::Location()
    : _dir(""), _root(""), method(), _redir(""), cgi(), indexs(),_index(""),_autoindex(""), _upload_path("")
{
}

Location::Location(const Location &copy)
{
    *this = copy;
}

Location &Location::operator=(const Location &copy)
{
    if (this != &copy)
    {
        this->_dir = copy._dir;
        this->_root = copy._root;
        this->method = copy.method;
        this->_redir = copy._redir;
        this->indexs = copy.indexs;
        this->cgi = copy.cgi;
        this->_upload_path = copy._upload_path;
    }
    return (*this);
}
Location::~Location()
{
}

void Location::setPath(std::string path)
{
    if (this->_dir == "")
        this->_dir = path;
    else
        ft_Error(1, "error Duplicate");
}
void Location::setRoot(std::string root)
{
      if (this->_root == "")
          this->_root = root;
      else
          ft_Error(1, "error Duplicate");
}
void Location::setAllowedMethods(std::vector<std::string> allowed_methods)
{
    std::vector<std::string> allow;
    for (size_t i = 0; i < allowed_methods.size(); i++)
    {
        if (Check_semicolon(allowed_methods[i]) == 1)
            allowed_methods[i] = remove_semicolon(allowed_methods[i]);
        if (allowed_methods[i] == "GET" || allowed_methods[i] == "POST" || allowed_methods[i] == "DELETE")
            allow.push_back(allowed_methods[i]);
        if (allowed_methods[i] == ";")
            continue;
        if (allowed_methods[i] != "GET" && allowed_methods[i] != "POST" && allowed_methods[i] != "DELETE")
            ft_Error(2, allowed_methods[i]);
    }
    if (this->method.empty())
        this->method = allow;
    else
        ft_Error(1, "error Duplicate_method");
}

void Location::setRedirection(std::string redirection)
{
    if (this->_redir == "")
        this->_redir = redirection;
    else
        ft_Error(1, "error Duplicate");
}

void Location::setCgi(std::map<std::string, std::string> cgi)
{
    this->cgi = cgi;
}
void Location::setIndexs(std::vector<std::string> indexs)
{
     if (this->indexs.empty())
        this->indexs = indexs;
    else
        ft_Error(1, "error Duplicate_method");
}
void Location::setIndex(std::string word)
{
     if (this->_index == "")
        this->_index = word;
    else
        ft_Error(1, "error Duplicate");
}
void Location::setautoindex(std::string word)
{
    if (this->_autoindex == "")
        this->_autoindex = word;
    else
        ft_Error(1, "error Duplicate");
}
void Location::setUploadPath(std::string upload_path)
{
    if (this->_upload_path == "")
        this->_upload_path = upload_path;
    else
        ft_Error(1, "error Duplicate");
}

void Location::addCgi(std::string extension, std::string cgi)
{
    this->cgi.insert(std::pair<std::string, std::string>(extension, cgi));
}

std::string Location::getDir() const
{
    return (this->_dir);
}

std::string Location::getRoot() const
{
    return (this->_root);
}

std::vector<std::string> Location::getMethod() const
{
    return (this->method);
}

std::string Location::getRedir() const
{
    return (this->_redir);
}

std::map<std::string, std::string> const &Location::getCgi() const
{
    return (this->cgi);
}
std::vector<std::string> Location::getIndexs() const
{
    return (this->indexs);
}

std::string		Location::getIndex() const
{
    return (this->_index);
}
std::string		Location::getautoindex() const
{
    return (this->_autoindex);
}
std::string Location::getUploadPath() const
{
    return (this->_upload_path);
}
