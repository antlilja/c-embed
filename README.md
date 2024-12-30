# c-embed
A CMake utility for embedding binary files in C programs.

## Usage

### CMake
```cmake
include(FetchContent)
FetchContent_Declare(c-embed
  GIT_REPOSITORY https://github.com/antlilja/c-embed.git
  GIT_TAG SOME_GIT_HASH
)
FetchContent_MakeAvailable(c-embed)

embed_files(some_files "file1" "file2")

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE some_files)
```

### C
```c
#include "some_files.h"

int main() {
  const unsigned char* file1_ptr = LOAD_DATA(some_files_file1);
  const unsigned long file1_len = LOAD_LEN(some_files_file1);

  return 0;
}
```
