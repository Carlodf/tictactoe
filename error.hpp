#pragma once

#include<string>
#include<cstdio>

//TODO deprecate in favor of Error::log()
static void error(
    const std::string& message,
    const std::string& error = "")
{
    if(error.empty())
    {
        std::printf("ERROR: %s\n", message.c_str());
        return;
    }
    std::printf("ERROR: %s [ %s ]\n", message.c_str(), error.c_str());
}

struct Error
{
    Error() = default;
    Error(
        std::size_t code,
        const std::string& message,
        const char* external_error) :
        code(code),
        message(message + std::string(external_error))
    {}

    Error(std::size_t code, const std::string& message) :
        code(code),
        message(message)
    {}

    void log() const
    {
        error(message);
    }
    std::size_t code = 0u;
    std::string message;
};
