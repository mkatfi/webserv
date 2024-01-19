#include "Parsing.hpp"
#include <cstdlib>


Conf::Conf(): file_name(""), _servers()
{
    p.server = 0;
	p.listen = 0;
	p.server_name = 0;
	p.host = 0;
	p.client_max_body_size = 0;
	p.root = 0;
	p.allow_methods = 0;
	p.index = 0;
	p.location = 0;
	p.autoindex = 0;
	p.upload = 0;
    p.error_page = 0;
    p.redirection = 0;
}

Conf::Conf(const Conf &copy)
{
    *this = copy;
}

Conf &Conf::operator=(const Conf &copy)
{
    if (this != &copy)
    {
        this->file_name = copy.file_name;
        this->_servers = copy._servers;
    }
    return (*this);
}

Conf::~Conf()
{}
// ############# setter #############

void Conf::setFileName(std::string file_name)
{
    this->file_name = file_name;
}
void Conf::setServers(std::vector<Server *> servers)
{
    this->_servers = servers;
}

//############# parsing #######################

void Conf::parse(void)
{
    std::ifstream file(this->file_name.c_str());
    if (!file.is_open())
        std::cerr << RED << "Error: " << RESET << this->file_name << ": " << std::endl, exit(1);
    std::string line;
    Server *currentServer = NULL;
    Location *currentLocation = NULL;
    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.empty() || line[0] == '#')
            continue;
        this->pars_server.push_back(line);
        std::vector<std::string> tokens = split(line, ' ');
        if (tokens[0] == "{")
            continue;
        if (tokens[0] == "server" && currentServer == NULL)
        {
            p.server++;
            currentServer = new Server();
            this->_servers.push_back(currentServer);
        }
        else if (tokens[0] == "}" && currentServer != NULL && currentLocation == NULL)
            currentServer = NULL;
        else if (currentServer != NULL && currentLocation == NULL && tokens[0] != "location")
        {
            if ( Check_semicolon_line(line) == 1)
            {
                this->parseServerLine(tokens, currentServer);
            }
            else
                ft_Error(1, "is not < ; > ");
        }

        if (tokens[0] == "location" && currentServer == NULL)
            ft_Error(1, "HHQQ******Mcha");
        if (tokens[0] == "location" && currentServer != NULL)
        {
            currentLocation = new Location();
            currentServer->addLocation(currentLocation);
            if (tokens.size() > 1 && tokens[1] != "{")
                currentLocation->setPath(tokens[1]);
            else
                currentLocation->setPath("/");
        }
        else if (tokens[0] == "}" && currentServer != NULL && currentLocation != NULL)
            currentLocation = NULL;
        else if (currentLocation != NULL && currentServer != NULL)
        {
            if (Check_semicolon_line(line) == 1)
                this->parseLocationLine(tokens, currentLocation);
            else
                ft_Error(1, "is not < ; > ");
        }
    }
    if (check_context(p) == 1 || (this->pars_server.back() != "}" && this->pars_server.back() != ""))
        ft_Error(2, "There is something missing");
    file.close();
}

// ############## parsing to server #####################################
void Conf::parseServerLine(std::vector<std::string> tokens, Server *server)
{
    if (tokens[0] == "server_name" && tokens.size() > 1)
    {
        p.server_name++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of server_name");
        }
        if (Check_semicolon(tokens[1]) == 2)
                ft_Error(1, "not value of server_name");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        server->setServerName(tokens[1]);
    }
    else if (tokens[0] == "host" && tokens.size() > 1 )
    {
        p.host++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of Host");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of HOST");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        server->setHost(tokens[1]);
    }
    else if (tokens[0] == "listen" && tokens.size() > 1)
    {
        p.listen++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value ... of listen");
        }
       if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Listen");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        std::vector<std::string> listenTokens = split(tokens[1], ':');
        if (listenTokens.size() == 1)
        {
            // server->setHost("127.0.0.1");
            server->setPort(listenTokens[0]);
        }
        else
        {
            // server->setHost(listenTokens[0]);
            server->setPort(listenTokens[1]);
        }
    }
    else if (tokens[0] == "root" && tokens.size() > 1)
    {
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of ROOT");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of ROOT");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        server->setRoot(tokens[1]);
        // if (isPathValid(tokens[1]))
        // else
        //     ft_Error(1, "Path is not valid (0*0).") ;
    }
    else if (tokens[0] == "error_page" && tokens.size() > 2)
    {
        p.error_page++;
        if (Check_semicolon(tokens[2]) == 0)
        {
            if (tokens[3] != ";")
                ft_Error(1, "many value of error Page");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of ERROR_page");
        if (Check_semicolon(tokens[2]) == 1)
            tokens[2] = remove_semicolon(tokens[2]);
        server->addErrorPage(tokens[1], tokens[2]);
    }
    else if (tokens[0] == "index" && tokens.size() > 1)
    {
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of INDEX");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        server->setIndex(tokens[1]);
    }
    else if (tokens[0] == "client_max_body_size" && tokens.size() > 1)
    {
        p.client_max_body_size++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of Client Max Body Size");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Client Max Body Size");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        server->setMaxClientBodySize(tokens[1]);
    }
    else if (tokens[0] == "cgi_path" && tokens.size() > 2)
    {
        if (Check_semicolon(tokens[2]) == 0)
        {
            if (tokens[3] != ";")
                ft_Error(1, "many value of AUtoindex");
        }
        if (Check_semicolon(tokens[1]) == 2 || Check_semicolon(tokens[2]) == 2)
            ft_Error(1, "not value of CGI");
        if (Check_semicolon(tokens[2]) == 1)
            tokens[2] = remove_semicolon(tokens[2]);
        server->addCgi(tokens[1], tokens[2]);
    }
    else if (tokens[0] == "allow_methods" && tokens.size() > 1)
    {
         if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Allow Methods");
        tokens.erase(tokens.begin());
        std::vector<std::string> allowTokens = tokens;
        server->setMethods(allowTokens);
    }
    else if (tokens[0] == "autoindex" && tokens.size() > 1)
    {
         if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of AUtoindex");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of AUtoindex");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        if (tokens[1] == "on" || tokens[1] == "off")
            server->setautoindex(tokens[1]);
        else
            ft_Error(1, "is not sure in server <...>");
    }
    else if (tokens[0] == "upload" && tokens.size() > 1)
    {
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of Upload");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Upload");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        if (tokens[1] == "on" || tokens[1] == "off")
            server->setUploadPath(tokens[1]);
        else
            ft_Error(1, "is not sure in server <...>");
    }
    else
        ft_Error(2, tokens[0]);
}

// ############## parsing to location #####################################

void Conf::parseLocationLine(std::vector<std::string> tokens, Location *location)
{
    if (tokens[0] == "root" && tokens.size() > 1)
    {
        p.root++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (tokens[2] != ";")
                ft_Error(1, "many value of ROOT");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of ROOT");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        location->setRoot(tokens[1]);
    }
    else if (tokens[0] == "index" && tokens.size() > 1)
    {
        p.index++;
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Index");
        tokens.erase(tokens.begin());
        std::vector<std::string> indexTokens = tokens;
        for (size_t i = 0; i < indexTokens.size();i++)
        {
            if (Check_semicolon(indexTokens[i]) == 1)
                indexTokens[i] = remove_semicolon(indexTokens[i]);
        }
        location->setIndexs(indexTokens);
        if (Check_semicolon(tokens[0]) == 1)
                tokens[0] = remove_semicolon(tokens[0]);
        location->setIndex(tokens[0]);
    }
    else if (tokens[0] == "allow_methods" && tokens.size() > 1)
    {
        p.allow_methods++;
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Allow Methods");
        tokens.erase(tokens.begin());

        std::vector<std::string> allowTokens = tokens;
        location->setAllowedMethods(allowTokens);
    }
    else if (tokens[0] == "cgi_path" && tokens.size() > 2)
    {
        if (Check_semicolon(tokens[2]) == 0)
        {
            if (tokens[3] != ";")
                ft_Error(1, "many value of AUtoindex");
        }
        if (Check_semicolon(tokens[1]) == 2 || Check_semicolon(tokens[2]) == 2)
            ft_Error(1, "not value of CGI");
        if (Check_semicolon(tokens[2]) == 1)
            tokens[2] = remove_semicolon(tokens[2]);
        location->addCgi(tokens[1], tokens[2]);
    }
    else if (tokens[0] == "return" && tokens.size() > 2)
    {
        p.redirection++;
        if (Check_semicolon(tokens[2]) == 0)
        {
            if (Check_semicolon(tokens[3]) != 2)
                ft_Error(1, "many value of AUtoindex");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Redirection");
        if (tokens[1] != "" && Check_semicolon(tokens[2]) == 2)
            ft_Error(1, "not value");
        if (Check_semicolon(tokens[2]) == 1)
            tokens[2] = remove_semicolon(tokens[2]);
        location->setRedirection(tokens[2]);
    }
    else if (tokens[0] == "autoindex" && tokens.size() > 1)
    {
        p.autoindex++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (Check_semicolon(tokens[2]) != 2)
                ft_Error(1, "many value of AUtoindex");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of AUtoindex");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        if (tokens[1] == "on" || tokens[1] == "off")
            location->setautoindex(tokens[1]);
        else
            ft_Error(1, "is not sure in locaction autoindex <...>");
    }
    else if (tokens[0] == "upload" && tokens.size() > 1)
    {
        p.upload++;
        if (Check_semicolon(tokens[1]) == 0)
        {
            if (Check_semicolon(tokens[2]) != 2)
                ft_Error(1, "many value of Upload");
        }
        if (Check_semicolon(tokens[1]) == 2)
            ft_Error(1, "not value of Upload");
        if (Check_semicolon(tokens[1]) == 1)
            tokens[1] = remove_semicolon(tokens[1]);
        if (tokens[1] == "on" || tokens[1] == "off")
            location->setUploadPath(tokens[1]);
        else
            ft_Error(1, "is not sure in location  up<...>");
    }
    else
        ft_Error(2, tokens[0]);
}

int Conf::check_context(pars_conf p)
{
    if (p.server <= 0 ||p.server_name  <= 0 || p.listen <= 0 ||
        p.host <= 0 || p.error_page <= 0)
        return (1);
    return (0);
}

int Conf::Check_semicolon_line( std::string line)
{
    if (line[line.length() - 1] == ';')
        return(1);
    return (0);
}
// ############## getters #####################################

std::string Conf::getFileName() const
{
    return (this->file_name);
}
std::vector<Server *> Conf::getServers() const
{
    return (this->_servers);
}

// ###########for print data befor seve data ###############################
void Conf::print_all_data()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "--- server "<< i << ":" << std::endl;
		std::cout << "name = " << _servers[i]->getName() << std::endl;
		std::cout << "host = " << _servers[i]->getHost() << std::endl;
		std::cout << "listen = " << _servers[i]->getListen() << std::endl;
		std::cout << "root = " << _servers[i]->getRoot() << std::endl;
		std::cout << "index = " << _servers[i]->getIndex() << std::endl;
		std::cout << "body = " << _servers[i]->getBody() << std::endl;
		std::cout << "autoindex = " << _servers[i]->getautoindex() << std::endl;

        std::cout << "methods = ";
		for (size_t len = 0; len < _servers[i]->getMethod().size(); len++)
			std::cout << _servers[i]->getMethod()[len] << " ";
		std::cout << std::endl;

		std::cout << "error pages:" << std::endl;
		std::map<std::string, std::string> copy = _servers[i]->getError();
        std::map<std::string, std::string>::iterator it = copy.begin();
        for (size_t len = 0; len < _servers[i]->getError().size(); len++)
        {
		 	std::cout << "error " << it->first << " = " << it->second << std::endl;
			it++;
		}

		for (size_t x = 0; x < _servers[i]->getLocation().size(); x++)
		{
			std::cout << "- location "<< x << ":" << std::endl;
			std::cout << "dir = " << _servers[i]->getLocation()[x]->getDir() << std::endl;
			std::cout << "root = " << _servers[i]->getLocation()[x]->getRoot() << std::endl;
			std::cout << "index = " << _servers[i]->getLocation()[x]->getIndex() << std::endl;
			std::cout << "redirction = " << _servers[i]->getLocation()[x]->getRedir() << std::endl;
			std::cout << "autoindex = " << _servers[i]->getLocation()[x]->getautoindex() << std::endl;
			std::cout << "upload = " << _servers[i]->getLocation()[x]->getUploadPath() << std::endl;
			std::cout << "methods = ";
			for (size_t len = 0; len < _servers[i]->getLocation()[x]->getMethod().size(); len++)
				std::cout << _servers[i]->getLocation()[x]->getMethod()[len] << " ";
			std::cout << std::endl;
		}
	}
}

