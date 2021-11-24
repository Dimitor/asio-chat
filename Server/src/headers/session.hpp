#pragma once
#include <iostream>
#include <queue>
#include <utility>
#include <unordered_set>
#include <boost/algorithm/string.hpp>
#include "boost/asio.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

using io_context = asio::io_context;
using socket_t = tcp::socket;
using err_code = boost::system::error_code;

using msg_handler = std::function<void (std::string)>;
using err_handler = std::function<void (bool is_authorized)>;
using auth_handler = std::function<bool (std::string const&, std::string const&)>;

using namespace std::placeholders;

struct session: std::enable_shared_from_this<session>
{
    session(socket_t &&socket)
    : socket{std::move(socket)}
    {}

    void start(msg_handler &&msg_func, err_handler &&err_func, auth_handler &&auth_func);

    //void set_name(std::string const &name){username = name;}

    void send_msg(std::string const &msg);

    std::string get_username() const {return username;}

    void close() {socket.close();}

    void close(err_code &err) {socket.close(err);}

private:
    std::string str_from_buf(size_t bufsize);

    void read_record();

    void read();

    void write();

    void record_reader(err_code err, size_t bytes_count);

    void reader(err_code err, size_t bytes_count);

    void writer(err_code err, size_t bytes_count);

    static std::pair<std::string, std::string> split_record(std::string const &record);

    socket_t socket;
    asio::streambuf buf;
    std::queue<std::string> msg_queue;
    msg_handler msg_h;
    err_handler err_h;
    auth_handler auth_h;
    std::string username;
    //std::weak_ptr<server> srv;
};
