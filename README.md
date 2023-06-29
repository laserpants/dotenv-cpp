# dotenv-cpp

## Installation

The library is header-only, so there is nothing to build. You could simply merge the `include` directory into your project root, entering something like 

```bash
mkdir -p include
cp -rn <path-to-this-repo>/dotenv-cpp/include/ .
```

and compile, adding `-I include/laserpants/dotenv` to your build command, which then allows you to `#include "dotenv.h"`.

Although the above method is sufficient for many use cases; for proper CMake support, you should install the library using a sequence of command such as

```bash
cd <path-to-this-repo>
mkdir -p build
cd build
cmake ..
make
sudo make install
```

By default, the headers will then be installed to `include/laserpants/dotenv-<VERSION>/`, relative to the CMake install prefix (`/usr/local/` on Linux/Unix). To compile *without* CMake, you can then use a flag

```
-I /usr/local/include/laserpants/dotenv-<VERSION>
```

or the equivalent path on other platforms. 

For CMake-based projects, your project's `CMakeLists.txt` file could look something like the following:

```cmake
cmake_minimum_required(VERSION 3.2)
project(test)

find_package(laserpants_dotenv)

add_executable(example example.cpp)
target_link_libraries(example laserpants::dotenv)
```

Then, in `example.cpp`, just

```cpp
#include <dotenv.h>
```

## Usage

### Example

Given a file `.env`

```shell
DATABASE_HOST=localhost
DATABASE_USERNAME=user
DATABASE_PASSWORD="antipasto"
```

and a program `example.cpp`

```cpp
// example.cpp
#include <iostream>
#include <dotenv.h>

int main()
{
    dotenv::init();

    std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
    std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;

    return 0;
}
```

the output is:

```
user
antipasto
```

### Default values

`dotenv::getenv()` is a wrapper for `std::getenv()` that also takes a default value, in case the variable is empty:

```cpp
std::cout << dotenv::getenv("DATABASE_USERNAME", "anonymous") << std::endl;
```

### Referencing other variables

Other variables can be referenced using either `${VARIABLE}` or `$VARIABLE`.

#### Example: 

Given this program

```cpp
#include <iostream>
#include <string>
#include "dotenv.h"

int main(int argc, char **argv)
{
   dotenv::init("resolve.env");
   std::cout << std::getenv("MESSAGE") << std::endl;
   return 0;
}
```

and this `resolve.env`

```
TEMPERATURE = cold
EXTENT      =    "  long  "
SEASON      = '$EXTENT $TEMPERATURE winter'
MORE        =    and $TEMPERATURE ice
CONTAINS    = lots of ${TEMPERATURE} snow $MORE
MESSAGE     =    I wish you a ${SEASON}, with $CONTAINS
```

the output becomes

```
I wish you a   long   cold winter, with lots of cold snow and cold ice
```

### Options

By default, if a name is already present in the environment, `dotenv::init()` will replace it with the new value. To preserve existing variables, you must pass the `Preserve` flag.

```cpp
dotenv::init(dotenv::Preserve);

std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;
```

Updating the previous example, recompiling and running the program again, after setting the `DATABASE_USERNAME` variable, e.g. as

```
DATABASE_USERNAME=root ./example
```

the output this time is:

```
root
antipasto
```

## Changelog

### 0.9.3

#### Added
- Add cctype header needed by std::isspace

### 0.9.2

#### Added
- Add support for referencing other variables in variable definitions

### 0.9.1

#### Added
- Add wrapper for setenv to support MSVC

### 0.9.0

- Initial version
