#include <tgbot/tgbot.h>
#include "config.h"

#include "bot.h"
#include "authManager.h"
#include "handlers.h"
#include "logger.h"

void runBot()
{
    ConsoleLogger logger;
    logger.info("bot", "initializing");
    AuthManager authManager;

    TgBot::Bot bot(BOT_TOKEN);

    bot.getApi().deleteMyCommands();

    std::vector<TgBot::BotCommand::Ptr> commands;

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

    try
    {
        logger.info("bot", "bot working");
        TgBot::TgLongPoll longPoll(bot);

        while (true)
        {
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e)
    {
        logger.error("bot", std::string("exception: ") + e.what());
    }
}
