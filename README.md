# libbft

## A lightweight and multi-language library collection for byzantine fault tolerance

Byzantine Fault Tolerance (BFT) is the core mechanism for building attack-resistant and self-adaptive systems. It is a widespread concept since the proposal of a Practical BFT (by Miguel Castro and Barbara Liskov), with multiple implementations nowadays (such as dBFT on Neo Blockchain).
This project intends to allow easy implementation of BFT protocols on C++ and multiple languages (portability for cross-language is fundamental!). 

It is intended to be also used as the official dBFT for [neopt](https://github.com/neoresearch/neopt), a C++ implementation of Neo Blockchain components, focused on portability.
It may also help the development of other ecosystem tools, protocols, and even develop other blockchain and fail-safe storage technologies.


## Build Instructions

Currently, C++ spec is the reference one. After that, focus will be on Go, Python and C# ports.

`cmake` is your friend! Please install latest version (you can use Snappy to ensure that!): `sudo snap install cmake --classic`

(If you don't have snap, you can install it on debian-based using: `sudo apt install snapd`)


### Builds for C++
Existing frameworks use high-level languages that may not be suitable for very lightweight architectures,
such as microcontrollers with very limited computing capabilities.

C/C++ is interoperable with nearly all existing languages, so the idea is to provide modules that can be
reused on other projects (on other languages too).

On debian-based systems (or ubuntu), just type `make cpp && ./spec/cpp/build/app_test`.

Getting submodules: `git submodule update --init --recursive` and `git pull --recurse-submodules`.

Installing graphviz: `sudo apt install graphviz`

#### tests for C++

It will also configure test library (as long as you cloned this project with `--submodules` too).
To test, just run `make test`.

**Note:** tests are still under development.

#### C++ Standard
Currently, C++11 is adopted, in order to keep the best compatibility between conversors and compilers. However, it is recommended to migrate to C++17 as soon as possible, if this does not break compatibility with any existing modules and tools.

Let's please follow the [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines).

#### vscode IDE
If using vscode IDE, it is recommended to install the following extensions:
* C/C++ (currently 0.23.0-insiders2)
* C++ Intellisense (currently 0.2.2)
* GoogleTest Adapter (currently 1.8.3)

#### C++ Format Style
The currently adopted style for C++ is `Mozilla`, with indentation level set to 3.
Recommended configuration for vscode:
```json
{
    "[cpp]": {
        "editor.tabSize" : 3,
        "editor.detectIndentation": false
    },
    "C_Cpp.clang_format_fallbackStyle": "{ BasedOnStyle : Mozilla , ColumnLimit : 0, IndentWidth: 3, AccessModifierOffset: -3}"
}
```

#### Variable Naming Style

Let's please follow the [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines), and as usual, classic CamelCase `C++` naming is welcome :)

### Builds for Go

Still under development.

### Builds for C#

Still under development.

### Builds for Python

Still under development.



### License

Code follows `MIT License`.
