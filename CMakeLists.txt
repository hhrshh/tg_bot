cmake_minimum_required(VERSION 3.15)

project(TelegramBotProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT UNIX)
    message(FATAL_ERROR "This project currently supports Linux/Unix only.")
endif()

find_package(TgBot CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

add_executable(MyTelegramBot
    src/main.cpp
    src/bot.cpp
    src/handlers.cpp
    src/logger.cpp
    src/database.cpp
)

target_include_directories(MyTelegramBot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(MyTelegramBot PRIVATE HAVE_CURL)

target_link_libraries(MyTelegramBot PRIVATE TgBot::TgBot CURL::libcurl Threads::Threads)
