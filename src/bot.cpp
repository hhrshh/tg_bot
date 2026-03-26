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
