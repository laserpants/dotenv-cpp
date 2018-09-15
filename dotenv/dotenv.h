#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>

class dotenv
{
public:
    static const unsigned char DontOverwrite = 1 << 0;

    static const int OptionsNone = 0;

    static void init(const char* filename = ".env");
    static void init(int flags, const char* filename = ".env");

    static std::string getenv(const char* name, const std::string& def = "");

private:
    static void do_init(int flags, const char* filename);
    static std::string strip_quotes(const std::string& str);
};

void dotenv::init(const char* filename)
{
    dotenv::do_init(OptionsNone, filename);
}

void dotenv::init(int flags, const char* filename)
{
    dotenv::do_init(flags, filename);
}

std::string dotenv::getenv(const char* name, const std::string& def)
{
    const char* str = std::getenv(name);
    return str ? std::string(str) : def;
}

void dotenv::do_init(int flags, const char* filename)
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
                setenv(name.c_str(), val.c_str(), ~flags & dotenv::DontOverwrite);
            }
            ++i;
        }
    }
}

std::string dotenv::strip_quotes(const std::string& str)
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
