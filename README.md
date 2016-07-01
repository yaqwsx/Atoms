# Atoms

C++14 support library containing various features useful in my projects. Many
parts of the library target to application in MCUs (AVR and Cortex ARM), thus
there can be two versions of the same thing - one resolves as much as possible
during compile time, the second one does not push so hard on compile time
resolving and tries to be simple or does not contain limitation (e.g. no dynamic
memory).

## Usage

For now, the library is header only (may change in the future) and uses
namespace `atoms`. To use it, just add directory `include` to the include path of
your compiler. Then your can simply write e.g `#include <atoms/type/tagged.h>` 
to use tagged types.

The library is meant to be compiled using C++14 standard, however many parts
of the library can be used with C++11. Library is tested using GCC 5, Clang 3.7
and Visual Studio 2015.

## Library structure

There is currently no pretty documentation e.g. in form of Doxygen (I hope this
will change in the future), however every part of the library features an example
in the [examples](examples) directory. All examples have their own makefile and
Visual Studio project and should demonstrate usage of library feature with short
comment.

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
  tagged types

## Contributions & issues

Feel free to submit issues, enhancement requests and pull requests.
