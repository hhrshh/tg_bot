#pragma once
#include <map>
#include <memory>
#include "user.h"

class AuthManager {
private:
    std::map<int64_t, std::shared_ptr<User>> users;
public:
    std::shared_ptr<User> registerUser(int64_t id, const std::string& username) {
        if (users.find(id) == users.end()) {
            users[id] = std::make_shared<User>(id, username);
        }

        return users[id];
    }

    bool authorizeUser(int64_t id) {
        auto it = users.find(id);
        if (it != users.end()) {
            it->second->authorize(true);
            return true;
        }
        return false;
    }

    void cancelAuth()
    {}

    bool checkAuth(int64_t id) {
        auto it = users.find(id);
        return it != users.end() && it->second->isAuthorized();
    }

};