#pragma once


#include "../Server.hpp"
#include "../Location.hpp"
#include "Confi_Pars.hpp"

#include <cstring>
#include <dirent.h>
#include <sys/stat.h>


# define RED	"\x1B[31m"
# define RESET	"\x1B[0m"

void Delete_all(const char *path);
std::string trim(const std::string &str);
std::vector<std::string> split(std::string const &str, char delim);

std::string remove_semicolon(std::string message);
int Check_semicolon(std::string message);

void ft_Error(int i, std::string message);
int check_server(std::vector<std::string> str);
int Check_semicolon_line();
bool isPathValid(const std::string &path);
