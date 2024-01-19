

#include "./Config/Parsing.hpp"

int main(int ac, char **av)
{
    Conf config_file;
    if (ac == 2)
    {
        config_file.setFileName(av[1]);
        config_file.parse();
        config_file.print_all_data();
        // Delete_all("/nfs/homes/mkatfi/Desktop/webserv/mar");
    }
    else if (ac == 1)
    {
        config_file.setFileName("./Config/default.conf");
        config_file.parse();
        config_file.print_all_data();
    }
    else
        ft_Error(1, "many argments ");
    return (1);

}
