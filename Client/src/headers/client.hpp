#pragma once
#include <queue>
#include <iostream>
#include "boost/asio.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

using io_context = asio::io_context;
using socket_t = tcp::socket;
using err_code = boost::system::error_code;

struct client
{
    client(io_context &context, tcp::endpoint &endpoint,
           std::string const &username, std::string const &password)
    : context{context}
    , socket{context}
    {connect(endpoint, username, password);}

    void send_msg(std::string msg);

    void close(){ asio::post(context, [&](){ socket.close(); }); }

private:
    void connect(tcp::endpoint &endpoint, std::string const &username, std::string const &password);

    void read();

    void write();

    void reader(err_code err, size_t bytes_count);

    void writer(err_code err, size_t bytes_count);

    io_context &context;
    socket_t socket;
    asio::streambuf buf;
    std::queue<std::string> msg_queue;
};