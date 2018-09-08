# dotenv-cpp

## Example

Given a file `.env`

```shell
DATABASE_HOST=localhost
DATABASE_USERNAME=user
DATABASE_PASSWORD="antipasto"
```

and a program

```cpp
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

the output is:

```
user
antipasto
```

### Options

By default, if a name is already present in the environment, `dotenv::init()` will replace it with the new value. To preserve existing variables, set the `DontOverwrite` flag.

```cpp
dotenv::init(dotenv::DontOverwrite);

std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;
```

Updating the previous example and running the program again, first setting the `DATABASE_USERNAME` variable, e.g. as

```
DATABASE_USERNAME=root ./main
```

the output this time is:

```
root
antipasto
```
