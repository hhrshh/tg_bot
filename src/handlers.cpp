#include "handlers.h"
#include "authManager.h"

AuthManager authManager;


void registerHandlers(TgBot::Bot& bot)
{
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        if (!message || !message->chat) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Bot activate!");
    });

    bot.getEvents().onCommand("login", [&bot](TgBot::Message::Ptr message) {
        if (!message || !message->chat || !message->from) {
            return;
        }
        auto user = authManager.registerUser(message->from->id, message->from->username);
        authManager.authorizeUser(user->getUserId());
        bot.getApi().sendMessage(message->chat->id, "Вы успешно авторизованы!");
    });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (!message || !message->chat || !message->from) {
            return;
        }

        if (message->text.empty() || message->text[0] == '/') {
            return;
        }

        if (!authManager.checkAuth(message->from->id)) {
            bot.getApi().sendMessage(message->chat->id, "Сначала авторизуйтесь через /login");
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Echo: " + message->text);
    });
}
