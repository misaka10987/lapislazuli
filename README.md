# LapisLazuli
*LapisLazuli* is a small yet useful collection of OI code.
## Install & Usage
For common uses, simply
```c++
#include "lapislazuli"
```
in your code. This library has all its functionalities contained in this single file.

Sometimes you may be required to submit a single file as solution. In such cases, you may run `build.sh` in the project root. This automated script will compress the source code into one line and write into `target/lapislazuli`. You may copy the code and replace the `#include` command in your code.
## C++ Standard
This project is targeting GNU C++17, under which standard the library guarantee to compile. Yet, a large number of C++23 features is used in this library and there is therefore warnings during compilation. The ultimate target is to migrate all the code into C++23.
