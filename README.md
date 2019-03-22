# propria

A general property customisation mechanism, implementing
[P1393r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1393r0.html)
(with fixes).

## Using propria

Propria is header only. Simply add the `include` directory to your compiler's
include path and `#include <propria.hpp>`.

Supports C++03, but requires [Boost](http://www.boost.org) to emulate standard
type traits.

Supports C++11 or later, with no dependencies other than the standard library.

## Building and running the tests

### GCC or Clang

For C++03:

    ./autogen.sh
    ./configure --with-boost=/path/to/boost
    make check

For C++11 or later, specify the standards version when running configure:

    ./autogen.sh
    CXX="g++ -std=c++11" ./configure
    make check

### Microsoft Visual Studio

Microsoft Visual Studio 2012 supports only the C++03 features of propria:

    cd tests
    nmake -f Makefile.msc check

Microsoft Visual Studio 2015 additionally supports the C++11 features of
propria:

    cd tests
    nmake -f Makefile.msc CPP11=1 check

Microsoft Visual Studio 2017 additionally supports the C++14 (and later)
features of propria:

    cd tests
    nmake -f Makefile.msc CPP11=1 CPP14=1 check
