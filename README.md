# Atoms

Atoms is a general support C++14 library containing various useful features used
in my projects. Many parts of the library target to application in MCUs (AVR and
Cortex ARM).

## Usage

For now, the library is header only (may change in the future) and uses the
namespace `atoms`. To use it, just add directory `include` to the include path
of your compiler. Then your can simply write e.g `#include <atoms/type/tagged.h>`
to use tagged types.

The library is meant to be compiled using C++14 standard, however many parts of
the library can be used with C++11. Library is tested in GCC 5, Clang 3.7 using
Travis CI.

## Current Status

[![Build Status](https://travis-ci.org/yaqwsx/Atoms.svg?branch=master)](https://travis-ci.org/yaqwsx/Atoms)

## Examples & Tests

There is currently no pretty documentation e.g. in form of Doxygen (I hope this
will change in the future), however every part of the library features an
example in the [examples](examples) directory.

Examples and tests are built using CMake. If you use Linux-like environment,
just issue these commands to build the examples:

```
mkdir build; cd build
cmake ..; make
```
Example executables can be found in `build/examples`. Tests sources are located
in the `tests` directory. The test binary can be found in `build/tests`.

If you use Windows environment, use an IDE supporting CMake, Cygwin or generate
Visual Studio project using CMake.

## Library structure

The library contains following features:

- **bit** - bit manipulation, e.g. for registry manipulation in MCU

- **communication** - support for serialization and deserialization of
  communication packets, that are used for communication with MCUs. Currently, 
  Avakar and general packet, whose format can be specified via template, are
  supported.

- **control** - regulation loops, PID regulators etc.

- **numeric** - fixed point numbers, 2D vectors, values with enforced constraints,
  etc.

- **type** - various traits (int type of at least size, has member function),
  tagged types, etc.

## Contributions & issues

Feel free to submit issues, enhancement requests and pull requests.
