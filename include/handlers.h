#pragma once

#include <tgbot/tgbot.h>

class ILogger;
class IAuthManager;

void registerHandlers(TgBot::Bot& bot, ILogger& logger, IAuthManager& authManager);
