#pragma once

#include "error.hpp"

#include <memory>
#include <string>
#include <tuple>

template<class T>
class Result
{
public:
    Result<T>() = default;
    Result<T>(T t, Error* e):
        tuple_(std::make_tuple(t, e))
    {}

    T& value()
    {
        return std::get<0>(tuple_);
    }

    const std::unique_ptr<Error>& error()
    {
        return std::get<1>(tuple_);
    }

    void set_value(T t)
    {
        std::get<0>(tuple_) = std::move(t);
    }

    void set_error(std::unique_ptr<Error>&& e)
    {
        std::get<1>(tuple_) = std::move(e);
    }

private:
    std::tuple<T, std::unique_ptr<Error>> tuple_;
};

template<typename T>
static std::tuple<T, std::unique_ptr<Error>> make_result()
{
    return std::make_tuple(T(), std::unique_ptr<Error>());
}

