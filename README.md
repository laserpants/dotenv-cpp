# dotenv-cpp

## Example

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
#include "dotenv.h"

int main()
{
    dotenv::init();

    std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
    std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;

    return 0;
}
```

assuming you have merged the contents of `include/` into your project root (e.g., `cp -r dotenv-cpp/include .`),

```bash
cpp example.cpp -o example -I include/laserpants/dotenv/ && ./example
```

the output is:

```
user
antipasto
```

### Installation

The library is header-only, so there is nothing to build. Although the above method of copying `include` into your source tree is sufficient for many use-cases, for proper CMake support, you can install the library using, e.g.,

```bash
mkdir -p build
cd build
cmake ..
make
sudo make install
```

By default, the headers will then be installed to `include/laserpants/dotenv-<VERSION>/`, relative to the CMake install prefix (`/usr/local/` on Linux/Unix). To compile the example program without CMake, you should use, e.g., `cpp example.cpp -o example -I/usr/local/include/laserpants/dotenv-<VERSION>/`.

For CMake-based projects, your project's `CMakeLists.txt` file could look something like the following:

```cmake
cmake_minimum_required(VERSION 3.2)
project(test)

find_package(laserpants_dotenv)

add_executable(example example.cpp)
target_link_libraries(example laserpants::dotenv)
```

Then, in `example.cpp`:

```cpp
#include <dotenv.h>
```

### Default values

`dotenv::getenv()` is a wrapper for `std::getenv()` that also takes a default value, in case the variable is empty:

```cpp
std::cout << dotenv::getenv("DATABASE_USERNAME", "anonymous") << std::endl;
```

### Options

By default, if a name is already present in the environment, `dotenv::init()` will replace it with the new value. To preserve existing variables, you must pass the `DontOverwrite` flag.

```cpp
dotenv::init(dotenv::DontOverwrite);

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
