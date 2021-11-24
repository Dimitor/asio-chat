#include "headers/session.hpp"

void session::start(msg_handler &&msg_func, err_handler &&err_func, auth_handler &&auth_func)
{
    msg_h = std::move(msg_func);
    err_h = std::move(err_func);
    auth_h  = std::move(auth_func);

    read_record();
}

void session::send_msg(std::string const &msg)
{
    msg_queue.push(msg);

    if(msg_queue.size() == 1)
    {
        write();
    }
}

std::string session::str_from_buf(size_t strsize)
{
    auto bufdata = buf.data();
    std::string res(asio::buffers_begin(bufdata), asio::buffers_begin(bufdata) + strsize);
    buf.consume(strsize);
    return res;
}

void session::read_record()
{
    asio::async_read_until(socket, buf, "\n", std::bind(&session::record_reader, shared_from_this(), _1, _2));
}

void session::read()
{
    asio::async_read_until(socket, buf, "\n", std::bind(&session::reader, shared_from_this(), _1, _2));
}

void session::write()
{
    asio::async_write(socket, asio::buffer(msg_queue.front()), std::bind(&session::writer, shared_from_this(), _1, _2));
}

void session::record_reader(err_code err, size_t bytes_count)
{
    if (!err)
    {
        auto record = str_from_buf(bytes_count);
        record.pop_back();
        auto [name, password] = split_record(record);
        if (auth_h(name, password))
        {
            username = name;
            read();
        }
        else
        {
            err_h(false);
        }
    }
    else
    {
        close(err);
        err_h(true);
    }
}

void session::reader(err_code err, size_t bytes_count)
{
    if (!err)
    {
        auto bufdata = buf.data();
        std::string message(asio::buffers_begin(bufdata), asio::buffers_begin(bufdata) + bytes_count);
        buf.consume(bytes_count);
        msg_h(message);
        read();
    }
    else
    {
        close(err);
        err_h(true);
    }
}

void session::writer(err_code err, size_t bytes_count)
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
        close(err);
        err_h(true);
    }
}

std::pair<std::string, std::string> session::split_record(std::string const &record)
{
    std::vector<std::string> record_parts;
    boost::split(record_parts, record, boost::is_any_of(":"));
    return std::make_pair(record_parts[0], record_parts[1]);
}
