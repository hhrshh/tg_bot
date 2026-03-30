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

    std::string normalizeCommand(const std::string& text) {
        if (text.empty() || text[0] != '/') {
            return "";
        }

        std::string cmd = text.substr(1);
        auto spacePos = cmd.find(' ');
        if (spacePos != std::string::npos) {
            cmd = cmd.substr(0, spacePos);
        }

        auto atPos = cmd.find('@');
        if (atPos != std::string::npos) {
            cmd = cmd.substr(0, atPos);
        }

        return cmd;
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
        authManager.authorizeUser(user->getUserId(), true);
        logger.info("handlers", "/login success userId=" + std::to_string(user->getUserId()));
        bot.getApi().sendMessage(message->chat->id, "Вы успешно авторизованы!");
    });


    bot.getEvents().onCommand("logout", [&bot, &logger, &authManager](TgBot::Message::Ptr message) {
        logger.info("handlers", "/logout received " + describeUser(message));

        if (!message || !message->chat || !message->from) {
            logger.warn("handlers", "/logout skipped: message/chat/from is null");
            return;
        }

        const auto userId = message->from->id;

        if (!authManager.checkAuth(userId)) {
            logger.info("handlers", "/logout skipped: already unauthorized userId=" + std::to_string(userId));
            bot.getApi().sendMessage(message->chat->id, "Вы уже не авторизованы. Авторизуйтесь /login");
            return;
        }

        if (!authManager.authorizeUser(userId, false)) {
            logger.warn("handlers", "/logout failed: user not found userId=" + std::to_string(userId));
            bot.getApi().sendMessage(message->chat->id, "Не удалось выполнить выход.");
            return;
        }

        logger.info("handlers", "/logout success userId=" + std::to_string(userId));
        bot.getApi().sendMessage(message->chat->id, "Вы вышли!");
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
            const std::string cmd = normalizeCommand(message->text);

            if (cmd != "start" && cmd != "login" && cmd != "logout") {
                logger.warn("handlers", "unknown command: " + cmd);
                bot.getApi().sendMessage(message->chat->id, "Неизвестная команда");
            }

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
