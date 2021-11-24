#include "headers/server.hpp"

void server::accept()
{
    socket.emplace(context);

    acceptor.async_accept(*socket, 
        [&](err_code err)
        {
            auto user = std::make_shared<session>(std::move(*socket));

            users.insert(user);

                    user->start(
                        std::bind(&server::send_to_all, this, _1),
                        [&, user_weak = std::weak_ptr(user)](bool is_authorized)
                        {
                            if (auto user_shared = user_weak.lock())
                            {
                                auto username = user_shared->get_username(); 

                                if (is_authorized)
                                {
                                    users.erase(user_shared);
                                    send_to_all(username + " has disconnected!\n");
                                }
                                else
                                {
                                    user_shared->send_msg("Your password is incorrect! Please, reconnect with right password.\n");
                                    user_shared->close();
                                    users.erase(user_shared);
                                }
                            }
                        },
                        std::bind(&server::check_user, this, _1, _2)
                    );
                    accept();

        });
}

void server::send_to_all(std::string const &msg)
{
    for (auto &user: users)
    {
        user->send_msg(msg);
    }
}

bool server::check_user(std::string const &username, std::string const &password)
{
    using state = database::user_state;

    auto res = db.check_user(username, password);
    
    if (res == state::result_not_found)
    {
        db.add_user(username, password);
        return true;
    }
    else 
    {
        return res == state::result_found;
    }
}