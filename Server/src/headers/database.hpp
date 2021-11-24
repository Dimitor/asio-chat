#pragma once
#include <string>
#include <fstream>
#include <exception>
#include <boost/algorithm/string/predicate.hpp>

struct database
{
    enum class user_state
    {
        result_found,
        result_incorrect_password,
        result_not_found
    };

    ~database(){ifs.close(); ofs.close();}

    database(std::string const &filename)
    : ifs{filename}
    , ofs{filename, std::ios::app}
    {if (!ifs.is_open() || !ofs.is_open()) { throw std::runtime_error("Database " + filename + " does not exists!"); }}

    user_state check_user(std::string const &username, std::string const &password);

    void add_user(std::string const &username, std::string const &password);

private:

    std::ifstream ifs;
    std::ofstream ofs;
};