#pragma once
#include <map>
#include <memory>
#include "user.h"

class AuthManager {
private:
    std::map<int64_t, std::shared_ptr<User>> users;
public:

};