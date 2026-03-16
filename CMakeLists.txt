cmake_minimum_required(VERSION 3.15)

project(TelegramBotProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(TgBot CONFIG REQUIRED)

add_executable(MyTelegramBot
    src/main.cpp
    src/bot.cpp
    src/handlers.cpp
    src/database.cpp
)

target_include_directories(MyTelegramBot PRIVATE include)

target_compile_definitions(MyTelegramBot PRIVATE _WIN32_WINNT=0x0A00)

target_link_libraries(MyTelegramBot PRIVATE TgBot::TgBot)

if(MINGW OR UNIX)
    target_link_libraries(MyTelegramBot PRIVATE pthread)
endif()