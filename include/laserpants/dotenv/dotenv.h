// Copyright (c) 2018 Heikki Johannes Hildén <hildenjohannes@gmail.com>
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
//     * Neither the name of copyright holder nor the names of other
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file dotenv.h
///
#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>

///
/// Utility class for loading environment variables from a `.env` file.
///
/// \code
/// DATABASE_HOST=localhost
/// DATABASE_USERNAME=user
/// DATABASE_PASSWORD="antipasto"
/// \endcode
///
/// \code
/// // example.cpp
/// #include <iostream>
/// #include "dotenv/dotenv.h"
///
/// int main()
/// {
///     dotenv::init();
///
///     std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
///     std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;
///
///     return 0;
/// }
/// \endcode
///
/// Compile and run the program, e.g. using,
///
/// \code
/// c++ example.cpp -o example && ./example
/// \endcode
///
/// and the output is:
///
/// \code
/// user
/// antipasto
/// \endcode
///
/// \see https://github.com/laserpants/dotenv-cpp
///
class dotenv
{
public:
    dotenv() = delete;
    ~dotenv() = delete;

    static const unsigned char Preserve = 1 << 0;

    static const int OptionsNone = 0;

    static void init(const char* filename = ".env");
    static void init(int flags, const char* filename = ".env");

    static std::string getenv(const char* name, const std::string& def = "");

private:
    static void do_init(int flags, const char* filename);
    static std::string strip_quotes(const std::string& str);
};

///
/// Read and initialize environment variables from the `.env` file, or a file
/// specified by the \a filename argument.
///
/// \param filename a file to read environment variables from
///
inline void dotenv::init(const char* filename)
{
    dotenv::do_init(OptionsNone, filename);
}

///
/// Read and initialize environment variables using the provided configuration
/// flags.
///
/// By default, if a name is already present in the environment, `dotenv::init()`
/// will replace it with the new value. To preserve existing variables, you
/// must pass the `Preserve` flag.
///
/// \code
/// dotenv::init(dotenv::Preserve);
/// \endcode
///
/// \param flags    configuration flags
/// \param filename a file to read environment variables from
///
inline void dotenv::init(int flags, const char* filename)
{
    dotenv::do_init(flags, filename);
}

///
/// Wrapper for std::getenv() which also takes a default value, in case the
/// variable turns out to be empty.
///
/// \param name the name of the variable to look up
/// \param def  a default value
///
/// \returns the value of the environment variable \a name, or \a def if the
///          variable is not set
///
inline std::string dotenv::getenv(const char* name, const std::string& def)
{
    const char* str = std::getenv(name);
    return str ? std::string(str) : def;
}

inline void dotenv::do_init(int flags, const char* filename)
{
    std::ifstream file;
    std::string line;

    file.open(filename);

    if (file)
    {
        unsigned int i = 1;

        while (getline(file, line))
        {
            const auto pos = line.find("=");

            if (pos == std::string::npos) {
                std::cout << "dotenv: Ignoring ill-formed assignment on line "
                          << i << ": '" << line << "'" << std::endl;
            } else {
                const auto name = line.substr(0, pos);
                const auto val = strip_quotes(line.substr(pos + 1));
                setenv(name.c_str(), val.c_str(), ~flags & dotenv::Preserve);
            }
            ++i;
        }
    }
}

inline std::string dotenv::strip_quotes(const std::string& str)
{
    const std::size_t len = str.length();

    if (len < 2)
        return str;

    const char first = str[0];
    const char last = str[len - 1];

    if (first == last && ('"' == first || '\'' == first))
        return str.substr(1, len - 2);

    return str;
}
