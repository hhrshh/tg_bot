#pragma once
#include <string>
#include <cstdint>
#include <utility>


class User {
private:
    std::int64_t id;
    std::string name;
    bool authorized;
public:
    explicit User(std::int64_t userId, std::string userName)
        : id(userId), name(std::move(userName)), authorized(false) {}

    std::int64_t getUserId() const { return id; }
    const std::string& getUsername() const { return name; }
    bool isAuthorized() const { return authorized; }
    
    void authorize(bool value = true) { authorized = value; }

};
