#pragma once
#include <string>
#include <cstdint>


class User {
private:
    std::int64_t id;
    std::string firstName;
    std::string name;
    bool authorized;
public:
    explicit User(std::int64_t userId, const std::string& userfirstName, const std::string& userName)
        : id(userId), firstName(userfirstName),  name(userName), authorized(false) {}
    std::int64_t getUserId() const
    {
        return id;
    }
    
    bool isAuthorized() const
    {
        return authorized == true;
    }
    
    void authorize() 
    {
        authorized = true;
    }
};