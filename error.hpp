#pragma once

#include<string>
#include<cstdio>

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
