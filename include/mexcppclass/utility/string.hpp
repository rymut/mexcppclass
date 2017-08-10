/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_UTILITY_STRING_HPP_
#define MEXCPPCLASS_UTILITY_STRING_HPP_

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

namespace mexcppclass {
namespace utility {

// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string FormatString(const std::string& format, Args ... args) {
    size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
    //  We don't want the '\0' inside
}

std::string ToLowerCase(const std::string &text);

bool IsAlphaNumeric(const std::string &text);

}  // namespace utility
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_UTILITY_STRING_HPP_
