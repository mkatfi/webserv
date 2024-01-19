
#include "Parsing.hpp"
#include "Confi_Pars.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>


std::string trim(const std::string &str)
{
    std::string trimmedStr = str;
    std::string::iterator start = trimmedStr.begin();
    while (start != trimmedStr.end() && std::isspace(*start))
        ++start;
    std::string::iterator end = trimmedStr.end();
    while (end != start && std::isspace(*(end - 1)))
        --end;
    trimmedStr.erase(end, trimmedStr.end());
    trimmedStr.erase(trimmedStr.begin(), start);
    std::string::iterator it = trimmedStr.begin();
    while (it != trimmedStr.end())
    {
        if (std::isspace(*it))
        {
            std::string::iterator next = it + 1;
            if (next != trimmedStr.end() && std::isspace(*next))
                trimmedStr.erase(next);
            else
                ++it;
        }
        else
            ++it;
    }
    return trimmedStr;
}

std::vector<std::string> split(std::string const &str, char delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delim, start);
    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

std::string remove_semicolon(std::string message)
{
     // Check if the last character of tokens[1] is a space
    size_t lastCharIndex = message.size() - 1;
    if (lastCharIndex > 0 && message[lastCharIndex] == ' ')
    {
        // If so, remove the space and the semicolon
        message = message.substr(0, lastCharIndex - 1);
    }
    else
    {
        // If not, remove only the semicolon
        message = message.substr(0, lastCharIndex);
    }
    return (message);
}
int Check_semicolon(std::string message)
{
    if (message == ";")
        return (2);
    for (size_t i = 0; i <= message.size(); i++)
    {
        if (message[i] == ';')
            return (1);
    }
    return (0);
}

void ft_Error(int i, std::string message)
{
    if (i == 1)
    {
        std::cerr << RED << "Error: " << RESET << message << std::endl;
        exit(1);
    }
    if (i == 2)
    {
        std::cerr << RED << "Error: " << RESET << "Unknown identifier: < " << message << " >" << std::endl;
        exit(1);
    }
}

bool isPathValid(const std::string& path)
{
    struct stat fileInfo;

    if (stat(path.c_str(), &fileInfo) == 0)
        return true;
    else
        return false;
}
