#include "headers/client.hpp"

using namespace std::placeholders;

void client::send_msg(std::string msg)
{
    msg_queue.push(msg);

    if(msg_queue.size() == 1)
    {
        write();
    }
}

void client::connect(tcp::endpoint &endpoint, std::string const &username, 
                     std::string const &password)
{
    socket.async_connect(endpoint, 
        [&](err_code const &err)
        {
            if (!err)
            {
                send_msg(username + ":" + password + "\n");
                read();
            }
        });
}

void client::read()
{
    asio::async_read_until(socket, buf, '\n', std::bind(&client::reader, this, _1, _2));
}

void client::write()
{
    asio::async_write(socket, asio::buffer(msg_queue.front()), std::bind(&client::writer, this, _1, _2));
}

void client::reader(err_code err, size_t bytes_count)
{
     if (!err)
    {
        auto bufdata = buf.data();
        std::string msg(asio::buffers_begin(bufdata), asio::buffers_begin(bufdata) + bytes_count);
        buf.consume(bytes_count);
        
        std::cout << msg << "\n";
        read();
    }
    else
    {
        socket.close();
    }
}

void client::writer(err_code err, size_t bytes_count)
{
    if (!err)
    {
        msg_queue.pop();
        if (!msg_queue.empty())
        {
            write();
        }
    }
    else
    {
        socket.close();
    }
}