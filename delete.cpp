/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkatfi <mkatfi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 10:04:42 by mkatfi            #+#    #+#             */
/*   Updated: 2024/01/17 14:50:38 by mkatfi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Config/Parsing.hpp"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

bool has_Permission(const std::string& filePath)
{
    // Check if the file exists
    if (access(filePath.c_str(), F_OK) != 0)
        return false;
    // Check if the process has write permission
    return access(filePath.c_str(), W_OK) == 0;
}
void Delete_all(const char* path)
{
    DIR* dir = opendir(path);
    if (!dir)
	{
        std::cerr << "Error opening directory: " << path << std::endl;
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
	{
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
            std::string entryPath = std::string(path) + "/" + entry->d_name;

            // Check entry is a directory
            struct stat entryStat;
            if (stat(entryPath.c_str(), &entryStat) == 0)
			{
                if (S_ISDIR(entryStat.st_mode))
				{
                    Delete_all(entryPath.c_str());
                }
				else
				{
                    if (has_Permission(entryPath))
                    {
                        // Delete the file
                        if (std::remove(entryPath.c_str()) == 0)
                            std::cout << "Deleted file: " << entryPath << std::endl;
                        else
                            std::cerr << "Error deleting file: " << entryPath << std::endl;
                    }
                    else
                            std::cerr << "No has permission for file : " << entryPath << std::endl;
                }
            }
        }
    }
    closedir(dir);
    // Delete the directory itself
    if (std::remove(path) == 0)
        std::cout << "Deleted directory: " << path << std::endl;
    else
        std::cerr << "Error deleting directory: " << path << std::endl;
}


