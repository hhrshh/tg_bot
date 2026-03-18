#include <iostream>

#include "handlers.h"
#include "authManager.h"

AuthManager authManager;


void registerHandlers(TgBot::Bot& bot)
{
    bot.getEvents().onCommand("start",
        [&bot](TgBot::Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Бот запущен!");
    });

    bot.getEvents().onAnyMessage(
        [&bot](TgBot::Message::Ptr message)
    {
        if (message->text.empty())
            return;

        if (message->text[0] == '/')
            return;

        bot.getApi().sendMessage(
            message->chat->id,
            "Echo: " + message->text
        );
    });
}