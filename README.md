# libbft

<p align="center">
  <a href="https://travis-ci.com/NeoResearch/libbft">
    <img src="https://travis-ci.com/NeoResearch/libbft.svg?branch=master" alt="Current TravisCI build status.">
  </a>
  <a href="https://github.com/NeoResearch/libbft/releases">
    <img src="https://badge.fury.io/gh/NeoResearch%2Flibbft.svg" alt="Current neo version.">
  </a>
  <a href='https://coveralls.io/github/NeoResearch/libbft'>
    <img src='https://coveralls.io/repos/github/NeoResearch/libbft/badge.svg' alt='Coverage Status' />
  </a>
  <a href="https://github.com/NeoResearch/libbft">
      <img src="https://tokei.rs/b1/github/NeoResearch/libbft?category=lines" alt="Current total lines.">
    </a>
  <a href="https://github.com/NeoResearch/libbft/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="License.">
  </a>
</p>

## A lightweight and multi-language library collection for byzantine fault tolerance

Byzantine Fault Tolerance (BFT) is the core mechanism for building attack-resistant and self-adaptive systems.
It is a widespread concept since the proposal of a Practical BFT (by Miguel Castro and Barbara Liskov), with multiple implementations nowadays (such as dBFT on Neo Blockchain).
This project intends to allow easy implementation of BFT protocols on C++ and multiple languages (portability for cross-language is fundamental!). 

It is intended to be also used as the official dBFT for [neopt](https://github.com/neoresearch/neopt), a C++ implementation of Neo Blockchain components, focused on portability.
It may also help the development of other ecosystem tools, protocols, and even develop other blockchain and fail-safe storage technologies.


## Build Instructions

Currently, C++ spec is the reference one. After that, focus will be on Go, Python and C# ports.

`cmake` is your friend! Please install latest version (you can use Snappy to ensure that!):
```sh
sudo snap install cmake --classic
```

(If you don't have snap, you can install it on debian-based using: `sudo apt install snapd`)


### Builds for C++
Existing frameworks use high-level languages that may not be suitable for very lightweight architectures,
such as microcontrollers with very limited computing capabilities.

C/C++ is interoperable with nearly all existing languages, so the idea is to provide modules that can be
reused on other projects (on other languages too).

On debian-based systems (or ubuntu), just type: 
```sh
make cpp && ./spec/cpp/build/app_test
```

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

## Citation

Cite this in your paper as:

```bibtex
@article{libbft2019,
    author = {Rodolfo Pereira Araujo and Igor Coelho and Luiz Satoru Ochi and Vitor Nazario Coelho},
    year = {2019},
    month = {10},
    title = "LibBFT: A High-Performace Timed Automata Library Collection for Byzantine Fault Tolerance",
    doi = "10.1109/SBAC-PAD.2019.00045",
    isbn = "978-1-7281-4194-7",
    issn = "2643-3001",
    publisher = "IEEE",
    series = "2019 31st International Symposium on Computer Architecture and High Performance Computing (SBAC-PAD)"
}
```

### License

Code follows `MIT License`.
