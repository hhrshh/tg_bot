# Telegram Bot Project (C++)

Минимальный Telegram-бот на C++ с использованием [TgBot](https://github.com/reo7sp/tgbot-cpp) и сборкой через CMake.

Сейчас в проекте есть:
- обработка команд `/start` и `/login`;
- echo-ответ только для авторизованных пользователей;
- базовое потокобезопасное консольное логирование;
- явная передача зависимостей (`ILogger`, `IAuthManager`) без глобального состояния в хендлерах.

## Структура проекта

```text
tg_bot/
├─ include/
│  ├─ authManager.h
│  ├─ bot.h
│  ├─ config.h        # локально, не добавляется в git
│  ├─ database.h
│  ├─ handlers.h
│  ├─ logger.h
│  └─ user.h
├─ src/
│  ├─ bot.cpp
│  ├─ database.cpp
│  ├─ handlers.cpp
│  ├─ logger.cpp
│  └─ main.cpp
├─ CMakeLists.txt
└─ readme.md
```

Ключевые модули:
- `src/main.cpp` — точка входа, вызывает `runBot()`.
- `src/bot.cpp` — инициализирует `TgBot::Bot`, создает `ConsoleLogger` и `AuthManager`, регистрирует хендлеры.
- `src/handlers.cpp` — команды и сообщения; зависимости принимаются через параметры `registerHandlers(...)`.
- `include/authManager.h` — интерфейс `IAuthManager` и текущая in-memory реализация `AuthManager`.
- `src/logger.cpp` — реализация `ILogger` с timestamp и уровнями логирования.
- `src/database.cpp` — заглушка инициализации БД.

## Установка и сборка

### 1. Клонирование
```bash
git clone https://github.com/hhrshh/tg_bot
cd tg_bot
```

### 2. Создание `include/config.h`
```cpp
#pragma once

#define BOT_TOKEN "ВАШ_ТОКЕН_ЗДЕСЬ"
```

### 3. Сборка
```bash
cmake -S . -B build
cmake --build build
```

### 4. Запуск
```bash
./build/MyTelegramBot
```

## Использование

1. Отправьте `/start`.
2. Авторизуйтесь командой `/login`.
3. После этого обычные текстовые сообщения будут возвращаться как `Echo: ...`.

Если пользователь не авторизован, бот попросит сначала выполнить `/login`.

## Планы по развитию

- Перенести `IAuthManager` на БД-backed реализацию.
- Добавить unit-тесты для хендлеров с `FakeAuthManager`/`MockAuthManager`.
- Реализовать полноценный persistence для пользователей.
- Добавить inline-кнопки и callback-обработчики.

## Лицензия

MIT
