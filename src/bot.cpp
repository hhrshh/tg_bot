#include <tgbot/tgbot.h>
#include <iostream>
#include "../include/config.h"

#include "../include/bot.h"
#include "../include/handlers.h"

void runBot()
{
    TgBot::Bot bot(BOT_TOKEN);

    registerHandlers(bot);

    try
    {
        std::cout << "Bot working..." << std::endl;

        TgBot::TgLongPoll longPoll(bot);

        while (true)
        {
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}