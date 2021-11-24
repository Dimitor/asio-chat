#pragma once
#include <optional>
#include <iostream>
#include <vector>
#include "boost/asio.hpp"
#include "session.hpp"
#include "database.hpp"

struct server
{
    server(io_context &context, unsigned int port, std::string &&dbname)
    : context{context}
    , acceptor{context, tcp::endpoint(tcp::v4(), port)}
    , db{std::move(dbname)}
    {}

    void accept();

    void send_to_all(std::string const &msg);

    bool check_user(std::string const &username, std::string const &password);

private:

    io_context &context;
    tcp::acceptor acceptor;
    std::optional<socket_t> socket;
    std::unordered_set<std::shared_ptr<session>> users;
    database db;
};