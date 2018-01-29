#pragma once

#include<string>
#include<cstdio>

static void Error(
    const std::string& message,
    const std::string& error)
{
    std::printf("ERROR: %s [ %s ]\n", message.c_str(), error.c_str());
}

static void Error( const std::string& message)
{
    std::printf("ERROR: %s \n", message.c_str());
}
