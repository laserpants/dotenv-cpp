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
