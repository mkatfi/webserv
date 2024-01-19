#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include "../Server.hpp"

# define RED	"\x1B[31m"
# define RESET	"\x1B[0m"
struct pars_conf
{
	int server;
	int listen;
	int server_name ;
	int host ;
	int client_max_body_size ;
	int root ;
	int allow_methods ;
	int index ;
	int location ;
	int autoindex ;
	int upload ;
	int error_page;
	int redirection;
};

class Conf
{
private:
	std::string file_name;
	std::vector<Server *> _servers;
	std::vector<std::string> pars_server;

public:
	pars_conf p;
	Conf(void);
	Conf(const Conf &copy);
	Conf &operator=(const Conf &copy);
	~Conf(void);

	void setFileName(std::string file_name);
	void setServers(std::vector<Server *> servers);

	std::string getFileName(void) const;
	std::vector<Server *> getServers(void) const;

	void print_all_data();
	int Check_semicolon_line(std::string line);
	// int check_context(std::string line);
	int check_context(pars_conf p);

	void parse(void);
	void parseServerLine(std::vector<std::string> tokens, Server *server);
	void parseLocationLine(std::vector<std::string> tokens, Location *location);

};
