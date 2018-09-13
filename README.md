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
#include "dotenv/dotenv.h"

int main()
{
    dotenv::init();

    std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
    std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;

    return 0;
}
```

```bash
cpp example.cpp -o example --std=c++11
```

the output is:

```
user
antipasto
```

### Default values

`dotenv::getenv()` is a wrapper for `std::getenv()` but also takes a default value, in case the variable is empty:

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
