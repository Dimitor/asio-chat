#include <string>
#include <thread>
#include "headers/client.hpp"
#include "boost/asio.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: client <port> <name> <password>";
        return 1;
    }

    try
    {
        std::string port{argv[1]};
        std::string username{argv[2]};
        std::string password{argv[3]};
        io_context context;
        tcp::endpoint endpoint{asio::ip::make_address("127.0.0.1"), std::stoi(port)};
        
        client user{context, endpoint, username, password};

        std::thread context_thread([&context](){ context.run(); });

        char msg[1024];
        while (std::cin.getline(msg, 1024))
        {
            user.send_msg(username + ": " + std::string(msg) + "\n");
        }

        user.close();
        context_thread.join();
    }
    catch(std::exception const &err)
    {
        std::cerr << "Error!!! " << err.what();
    }
    return 0;
}