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
#include <utility>

///
/// Utility class for loading environment variables from a file.
///
/// ### Typical use
///
/// Given a file `.env`
///
/// \code
/// DATABASE_HOST=localhost
/// DATABASE_USERNAME=user
/// DATABASE_PASSWORD="antipasto"
/// \endcode
///
/// and a program `example.cpp`
///
/// \code
/// // example.cpp
/// #include <iostream>
/// #include <dotenv.h>
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
/// c++ example.cpp -o example -I/usr/local/include/laserpants/dotenv-0.9.1 && ./example
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
    static std::pair<std::string,bool> expand(size_t iline, const std::string& str);
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

#ifdef _MSC_VER

// https://stackoverflow.com/questions/17258029/c-setenv-undefined-identifier-in-visual-studio
int setenv(const char *name, const char *value, int overwrite)
{
    int errcode = 0;
    
    if (!overwrite) 
    {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, NULL, 0, name);
        if (errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}
#endif // _MSC_VER

///
/// Expand pre-defined variables on the form ${name} in a variable definition
///
/// \param iline line number in .env file
/// \param str   the string to be expanded, containing 0 or more ${name} definitions
/// \param ok    true on return if no variables found or all variables expanded ok
///
/// \returns the expanded definition when ok=true, or else str is returned unchanged
///
inline std::pair<std::string,bool>  dotenv::expand(size_t iline, const std::string& str)
{
   std::string expanded;

   size_t pos = 0;
   size_t pre_pos = pos;
   size_t nvar = 0;

   bool finished=false;
   while(!finished) {

      // look for start of variable
      pos = str.find("${",pos);
      if(pos != std::string::npos) {

         // a variable definition detected
         nvar++;
         expanded += str.substr(pre_pos,pos-pre_pos);  // add substring since last variable
         size_t p1 = pos;

         // look for end of variable
         pos = str.find('}',pos);
         if(pos != std::string::npos) {

            // end of variable found
            std::string var = str.substr(p1,pos-p1+1);
            if(const char* env_str = std::getenv(var.substr(2,var.length()-3).c_str())) {
               expanded += env_str;
               nvar--; // decrement to indicate all ok
            }
            else {
               // could not expand the variable, so don't decrement
               std::cout << "dotenv: Variable " << var << " is not defined on line " << iline << std::endl;
            }
            pre_pos = pos+1;
         }
      }
      else {
         // no more variables
         finished = true;
      }
   }

   // add trailing string after last variable
   expanded += str.substr(pre_pos);

   // nvar must be 0, or else we have an error
   return std::make_pair(expanded,(nvar==0));
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
                const auto line_stripped = strip_quotes(line.substr(pos + 1));

                // expand any contained variables on the form ${variable} in 'line_stripped'
                auto p = expand(i,line_stripped);
                bool ok = p.second;
                if(!ok) {
                   std::cout << "dotenv: Ignoring ill-formed assignment on line "
                   << i << ": '" << line << "'" << std::endl;
                }
                else {

                   // variable expansion ok, set as environment variable
                   const auto& val = p.first;
                   setenv(name.c_str(), val.c_str(), ~flags & dotenv::Preserve);
                }
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
