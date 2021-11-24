//#include <thread>
#include <vector>
#include <iostream>
#include "headers/server.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: server <port>";
        return 1;
    }

    try
    {
        std::string port{argv[1]};
        io_context context;
        server serv{context, std::stoi(port), "users.txt"};
        serv.accept();
        context.run();
    }
    catch (std::exception const &err)
    {
        std::cerr << "Error!!! " << err.what();
    }
    return 0;
}
