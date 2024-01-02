#include "Parsing.hpp"

// void debug(std::string const &msg)
// {
//     std::cerr << msg << std::endl;
// }

Conf::Conf()
    : file_name(""), _servers()
{
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
{
}
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
        // std::cout << "-- " << line << " --\n";
        if (line.empty() || line[0] == '#')
            continue;
        if (line[line.size() - 1] == ';')
            line = line.substr(0, line.size() - 1 - (line[line.size() - 2] == ' '));
        std::vector<std::string> tokens = split(line, ' ');
        // std::cout << "==> " << tokens[0] << " <==\n";
        if (tokens[0] == "{")
            continue;
        if (tokens[0] == "server" && currentServer == NULL)
        {
            currentServer = new Server();
            this->_servers.push_back(currentServer);
        }
        else if (tokens[0] == "}" && currentServer != NULL && currentLocation == NULL)
            currentServer = NULL;
        else if (currentServer != NULL && currentLocation == NULL && tokens[0] != "location")
            this->parseServerLine(tokens, currentServer);

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
            this->parseLocationLine(tokens, currentLocation);
    }
    file.close();
}

// ############## parsing to server #####################################
void Conf::parseServerLine(std::vector<std::string> tokens, Server *server)
{
    if (tokens[0] == "server_name" && tokens.size() > 1)
        server->setServerName(tokens[1]);
    else if (tokens[0] == "host" && tokens.size() > 1)
        server->setHost(tokens[1]);
    else if (tokens[0] == "listen" && tokens.size() > 1)
    {
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
        server->setRoot(tokens[1]);
    else if (tokens[0] == "error_page" && tokens.size() > 2)
        server->addErrorPage(tokens[1], tokens[2]);
    else if (tokens[0] == "index" && tokens.size() > 1)
        server->setIndex(tokens[1]);
    else if (tokens[0] == "client_max_body_size" && tokens.size() > 1)
        server->setMaxClientBodySize(tokens[1]);
    else if (tokens[0] == "cgi_path" && tokens.size() > 2)
        server->addCgi(tokens[1], tokens[2]);
    else if (tokens[0] == "allow_methods" && tokens.size() > 1)
    {
        tokens.erase(tokens.begin());
        std::vector<std::string> allowTokens = tokens;
        server->setMethods(allowTokens);
    }
    else if (tokens[0] == "autoindex" && tokens.size() > 1)
        server->setautoindex(tokens[1]);
    else if (tokens[0] == "upload" && tokens.size() > 1)
        server->setUploadPath(tokens[1]);
    else
        std::cerr << RED << "Error: " << RESET << "Unknown identifier: ana`" << tokens[0] << "`" << std::endl, exit(1);
}

// ############## parsing to location #####################################

void Conf::parseLocationLine(std::vector<std::string> tokens, Location *location)
{
    if (tokens[0] == "root" && tokens.size() > 1)
        location->setRoot(tokens[1]);
    else if (tokens[0] == "index" && tokens.size() > 1)
    {
        tokens.erase(tokens.begin());
        std::vector<std::string> indexTokens = tokens;

       // //std::cout << "==========> "<< tokens[1] << "===" << std::endl;
        location->setIndexs(indexTokens);
        location->setIndex(tokens[0]);
    }
    else if (tokens[0] == "allow_methods" && tokens.size() > 1)
    {

        // std::cout << tokens[0] << "<==\n";
        tokens.erase(tokens.begin());

        std::vector<std::string> allowTokens = tokens;
        location->setAllowedMethods(allowTokens);
    }
    else if (tokens[0] == "cgi_path" && tokens.size() > 2)
    {
        location->addCgi(tokens[1], tokens[2]);
        // std::cout << tokens[2] << "\n";
        // std::cout << "<<"<<tokens[1] << ">>\n";
    }
    else if (tokens[0] == "redirect" && tokens.size() > 2)
        location->setRedirection(tokens[2]);
    else if (tokens[0] == "autoindex" && tokens.size() > 1)
        location->setautoindex(tokens[1]);
    else if (tokens[0] == "upload" && tokens.size() > 1)
        location->setUploadPath(tokens[1]);
    else
        std::cerr << RED << "Error: " << RESET << "Unknown identifier: 1 `" << tokens[0] << "`" << std::endl, exit(1);
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
// void Conf::print_all_data()
// {
// 	for (size_t i = 0; i < _servers.size(); i++)
// 	{
// 		std::cout << "--- server "<< i << ":" << std::endl;
// 		std::cout << "name = " << _servers[i]->getName() << std::endl;
// 		std::cout << "host = " << _servers[i]->getHost() << std::endl;
// 		std::cout << "listen = " << _servers[i]->getListen() << std::endl;
// 		std::cout << "root = " << _servers[i]->getRoot() << std::endl;
// 		std::cout << "index = " << _servers[i]->getIndex() << std::endl;
// 		std::cout << "body = " << _servers[i]->getBody() << std::endl;
// 		std::cout << "autoindex = " << _servers[i]->getautoindex() << std::endl;
// 		std::cout << "methods = ";
// 		for (size_t len = 0; len < _servers[i]->getMethod().size(); len++)
// 			std::cout << _servers[i]->getMethod()[len] << " ";
// 		std::cout << std::endl;
// 		std::cout << "error pages:" << std::endl;
// 		std::map<std::string, std::string> copy = _servers[i]->getError();
// 		std::map<std::string, std::string>::iterator it = copy.begin();
// 		for (size_t len = 0; len < _servers[i]->getError().size(); len++)
// 		{
// 		 	std::cout << "error " << it->first << " = " << it->second << std::endl;
// 			it++;
// 		}

// 		for (size_t x = 0; x < _servers[i]->getLocation().size(); x++)
// 		{
// 			std::cout << "- location "<< x << ":" << std::endl;
// 			std::cout << "dir = " << _servers[i]->getLocation()[x]->getDir() << std::endl;
// 			std::cout << "root = " << _servers[i]->getLocation()[x]->getRoot() << std::endl;
// 			std::cout << "index = " << _servers[i]->getLocation()[x]->getIndex() << std::endl;

// 			std::cout << "autoindex = " << _servers[i]->getLocation()[x]->getautoindex() << std::endl;
// 			std::cout << "upload = " << _servers[i]->getLocation()[x]->getUploadPath() << std::endl;
// 			std::cout << "methods = ";
// 			for (size_t len = 0; len < _servers[i]->getLocation()[x]->getMethod().size(); len++)
// 				std::cout << _servers[i]->getLocation()[x]->getMethod()[len] << " ";
// 			std::cout << std::endl;
// 		}
// 	}
// }


// void Conf::print() const/ WebServ serv(envp, config_file.getServers());
    // serv.run();
    // free fds;<< std::endl;
//     for (std::vector<Server*>::const_iterator it = this->servers.begin(); it != this->servers.end(); ++it)
//     {
//         //std::cout << std::endl << "--- [SERVER] ---" << std::endl;
//         if (!(*it)->getServerName().empty())
//             //std::cout << "server_name: " << (*it)->getServerName() << std::endl;
//         if (!(*it)->getIp().empty())
//             //std::cout << "ip: " << (*it)->getIp() << std::endl;
//         if ((*it)->getPort() != -1)
//             //std::cout << "port: " << (*it)->getPort() << std::endl;
//         if (!(*it)->getRoot().empty())
//             //std::cout << "root: " << (*it)->getRoot() << std::endl;
//         if (!(*it)->getIndex().empty())
//             //std::cout << "index: " << (*it)->getIndex() << std::endl;
//         if ((*it)->getMaxClientBodySize() != 0)
//             //std::cout << "max_client_body_size: " << (*it)->getMaxClientBodySize() << std::endl;
//         if ((*it)->getErrorPages().size() > 0)
//         {
//             //std::cout << "error_pages: ";
//             const std::map<int, std::string>& errorPages = (*it)->getErrorPages();
//             for (std::map<int, std::string>::const_iterator it2 = errorPages.begin(); it2 != errorPages.end(); ++it2)
//                 //std::cout << it2->first << " " << it2->second << " ";
//             //std::cout << std::endl;
//         }
//         if ((*it)->getCgi().size() > 0)
//         {
//             //std::cout << "cgi: ";
//             const std::map<std::string, std::string>& cgi = (*it)->getCgi();
//             for (std::map<std::string, std::string>::const_iterator it2 = cgi.begin(); it2 != cgi.end(); ++it2)
//                 //std::cout << it2->first << " " << it2->second << " ";
//             //std::cout << std::endl;
//         }
//         if ((*it)->getMethods().size() > 0)
//         {
//             //std::cout << "allowed_methods: ";
//             const std::vector<std::string>& methods = (*it)->getMethods();
//             for (std::vector<std::string>::const_iterator it2 = methods.begin(); it2 != methods.end(); ++it2)
//                 //std::cout << *it2 << " ";
//             //std::cout << std::endl;
//         }
//         if ((*it)->getLocations().size() > 0)
//         {
//             //std::cout << "locations: " << std::endl;
//             const std::vector<Location*> &locations = (*it)->getLocations();
//             for (std::vector<Location*>::const_iterator it2 = locations.begin(); it2 != locations.end(); ++it2)
//             {
//                 //std::cout << std::endl << "    --- [LOCATION] ---" << std::endl;
//                 //std::cout << "    path: " << (*it2)->getDir() << std::endl;
//                 if (!(*it2)->getRoot().empty())
//                     //std::cout << "    root: " << (*it2)->getRoot() << std::endl;
//                 if ((*it2)->getIndexs().size() > 0)
//                 {
//                     //std::cout << "    index: ";
//                     const std::vector<std::string>& indexs = (*it2)->getIndexs();
//                     for (std::vector<std::string>::const_iterator it3 = indexs.begin(); it3 != indexs.end(); ++it3)
//                         //std::cout << *it3 << " ";
//                     //std::cout << std::endl;
//                 }
//                 if ((*it2)->getMethod().size() > 0)
//                 {
//                     //std::cout << "    allowed_methods: ";
//                     const std::vector<std::string>& allowedMethods = (*it2)->getMethod();
//                     for (std::vector<std::string>::const_iterator it3 = allowedMethods.begin(); it3 != allowedMethods.end(); ++it3)
//                         //std::cout << *it3 << " ";
//                     //std::cout << std::endl;
//                 }
//                 if (!(*it2)->getRedir().empty())
//                     //std::cout << "    redirection: " << (*it2)->getRedir() << std::endl;
//             }
//         }
//     }
// }
