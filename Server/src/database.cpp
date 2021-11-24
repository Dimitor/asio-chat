#include "headers/database.hpp"

database::user_state 
database::check_user(std::string const &username, std::string const &password)
{
    std::string record{username + ":" + password};
    std::string line;
    ifs.seekg(0);

    while (ifs >> line)
    {
        if (boost::starts_with(line, username))
        {
            ifs.clear();
            if (boost::ends_with(line, password))
            {
                return user_state::result_found;
            }
            else
            {
                return user_state::result_incorrect_password;
            }
        }
    }
    ifs.clear();
    return user_state::result_not_found;
}

void database::add_user(std::string const &username, std::string const &password)
{
    ofs << username << ":" << password << std::endl;
}