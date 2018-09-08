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
#include "dotenv.h"

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
