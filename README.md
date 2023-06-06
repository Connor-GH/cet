## CET - C Explicit Types.
An extension upon C that adds more explicit typing to the language.


download with:

``git clone https://github.com/Connor-GH/cet``

``cd cet``

compile with:

# [gcc]

``make CC=gcc TARGET_ARCH=X``, where X is an architecture supported.
For the backend, currently only x86 (amd64) is supported. You would
specify "x86" in TARGET_ARCH.

# [clang]

``make CC=clang TARGET_ARCH=X``, where X is an architecture supported.
For the backend, currently only x86 (amd64) is supported. You would
specify "x86" in TARGET_ARCH.


# Mandatory Dependencies:
- clang/clang++ or gcc/g++
- gmake

# Optional Dependencies
- clang-format

[Debugging]
- valgrind, gdb, etc
