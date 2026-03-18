# Telegram Bot Project (C++)

Минимальный Telegram-бот на C++ с использованием библиотеки [TgBot](https://github.com/reo7sp/tgbot-cpp).  
Проект организован с CMake, поддерживает обработку команд и echo-сообщения.

---

## Структура проекта

```
TelegramBotProject/
├─ include/
│  ├─ bot.h
│  ├─ config.h # токен бота (не добавляется в git)
│  ├─ database.h
│  ├─ handlers.h
|  ├─ User.h
|  └─ AuthManager.h
├─ src/
│  ├─ main.cpp
│  ├─ bot.cpp
│  ├─ database.cpp
│  └─ handlers.cpp
├─ CMakeLists.txt
└─ .gitignore
```

- `main.cpp` — точка входа, вызывает функцию запуска бота.
- `bot.cpp/h` — инициализация и запуск TgBot.
- `handlers.cpp/h` — обработка команд и сообщений.
- `database.cpp/h` — инициализация базы данных (пока заглушка).
- `config.h` — содержит токен бота (не хранить в репозитории).

---

## Установка и сборка

### 1. Клонирование репозитория
```bash
git clone https://github.com/hhrshh/tg_bot
cd TelegramBotProject
```

### 2. Создание файла с токеном
Создайте файл `include/config.h` с вашим токеном:
```cpp
#pragma once

#define BOT_TOKEN "ВАШ_ТОКЕН_ЗДЕСЬ"
```

### 3. Сборка с CMake
```bash
mkdir build
cmake -B build
cmake --build build
```

### 4. Запуск бота
```bash
# Linux
./MyTelegramBot

# Windows
MyTelegramBot.exe
```

---

## Использование

- `/start` — запускает бота и выводит приветственное сообщение.
- Все остальные сообщения — бот отвечает echo-сообщением.

Пример:
```
Пользователь: Привет!
Бот: Echo: Привет!
```

---

## Планы по развитию

- Подключение реальной базы данных (SQLite/MySQL/PostgreSQL)
- Добавление inline-кнопок и callback-обработчиков
- Логирование и обработка ошибок
- Поддержка команд через карту команд для удобного расширения

---

## Лицензия

MIT License

