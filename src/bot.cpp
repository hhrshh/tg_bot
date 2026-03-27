#include <tgbot/tgbot.h>
#include "config.h"
#include <tgbot/net/CurlHttpClient.h>

#include "bot.h"
#include "authManager.h"
#include "handlers.h"
#include "logger.h"

void runBot()
{
    ConsoleLogger logger;
    logger.info("bot", "initializing");
    
    try {

    AuthManager authManager;
    TgBot::CurlHttpClient httpClient;

    TgBot::Bot bot(BOT_TOKEN, httpClient);

    bot.getApi().deleteMyCommands();
    std::vector<TgBot::BotCommand::Ptr> commands;
    auto startCmd = std::make_shared<TgBot::BotCommand>();
    startCmd->command = "start";
    startCmd->description = "Запустить бота";
    commands.push_back(startCmd);
    auto loginCmd = std::make_shared<TgBot::BotCommand>();
    loginCmd->command = "login";
    loginCmd->description = "Авторизоваться";
    commands.push_back(loginCmd);
    auto logoutCmd = std::make_shared<TgBot::BotCommand>();
    logoutCmd->command = "logout";
    logoutCmd->description = "Выйти из аккаунта";
    commands.push_back(logoutCmd);
    bot.getApi().setMyCommands(commands);

    registerHandlers(bot, logger, authManager);


        logger.info("bot", "bot working");
        TgBot::TgLongPoll longPoll(bot);

        while (true)
        {
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        logger.error("bot", std::string("exception: ") + e.what());
    }

    catch (const std::exception& e) {
        logger.error("bot", std::string("std exception: ") + e.what());
    }
}
