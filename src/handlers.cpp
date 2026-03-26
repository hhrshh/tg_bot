#include "handlers.h"
#include "authManager.h"
#include "logger.h"

#include <string>

namespace {
std::string describeUser(const TgBot::Message::Ptr& message) {
    if (!message || !message->from) {
        return "userName=<empty> userId=<none>";
    }

    const std::string userName = message->from->username.empty() ? "<empty>" : message->from->username;
    return "userName=" + userName + " userId=" + std::to_string(message->from->id);
}
} // namespace

void registerHandlers(TgBot::Bot& bot, ILogger& logger, IAuthManager& authManager)
{
    bot.getEvents().onCommand("start", [&bot, &logger](TgBot::Message::Ptr message) {
        logger.info("handlers", "/start received " + describeUser(message));

        if (!message || !message->chat) {
            logger.warn("handlers", "/start skipped: message/chat is null");
            return;
        }

        logger.info("handlers", "/start activation message sent chatId=" + std::to_string(message->chat->id));
        bot.getApi().sendMessage(message->chat->id, "Бот активирован!");
    });

    bot.getEvents().onCommand("login", [&bot, &logger, &authManager](TgBot::Message::Ptr message) {
        logger.info("handlers", "/login received " + describeUser(message));

        if (!message || !message->chat || !message->from) {
            logger.warn("handlers", "/login skipped: message/chat/from is null");
            return;
        }

        auto user = authManager.registerUser(message->from->id, message->from->username);
        authManager.authorizeUser(user->getUserId());
        logger.info("handlers", "/login success userId=" + std::to_string(user->getUserId()));
        bot.getApi().sendMessage(message->chat->id, "Вы успешно авторизованы!");
    });

    bot.getEvents().onAnyMessage([&bot, &logger, &authManager](TgBot::Message::Ptr message) {
        if (!message || !message->chat || !message->from) {
            logger.warn("handlers", "message skipped: message/chat/from is null");
            return;
        }

        if (message->text.empty()) {
            logger.info("handlers", "message skipped: empty text " + describeUser(message));
            return;
        }

        if (message->text[0] == '/') {
            logger.info("handlers", "message skipped: command text routed to onCommand " + describeUser(message));
            return;
        }

        if (!authManager.checkAuth(message->from->id)) {
            logger.warn("handlers", "message denied: unauthorized userId=" + std::to_string(message->from->id));
            bot.getApi().sendMessage(message->chat->id, "Сначала авторизуйтесь через /login");
            return;
        }

        logger.info("handlers", "echo sent userId=" + std::to_string(message->from->id) + " text=" + message->text);
        bot.getApi().sendMessage(message->chat->id, "Echo: " + message->text);
    });
}
