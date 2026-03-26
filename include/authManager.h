#pragma once
#include <map>
#include <memory>
#include "user.h"

class IAuthManager {
public:
    virtual ~IAuthManager() = default;
    virtual std::shared_ptr<User> registerUser(int64_t id, const std::string& username) = 0;
    virtual bool authorizeUser(int64_t id, bool auth_status) = 0;
    virtual bool checkAuth(int64_t id) = 0;

};

class AuthManager final : public IAuthManager {
private:
    std::map<int64_t, std::shared_ptr<User>> users;
public:
    std::shared_ptr<User> registerUser(int64_t id, const std::string& username) override {
        if (users.find(id) == users.end()) {
            users[id] = std::make_shared<User>(id, username);
        }

        return users[id];
    }

    bool authorizeUser(int64_t id, bool auth_status) override {
        auto it = users.find(id);
        if (it != users.end()) {
            it->second->authorize(auth_status);
            return true;
        }
        return false;
    }

    bool checkAuth(int64_t id) override {
        auto it = users.find(id);
        return it != users.end() && it->second->isAuthorized();
    }

};
