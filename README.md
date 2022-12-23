# malg 

Portable, header-only linear algebra library written in C++

---

### Goals

- Create a portable and efficient class template with a clean interface
- Flexibility to use generic scalar types for matrix value (e.g. int, float)
- Allocate memory contiguously (no 'vector of vectors' implementation)
- Dynamic allocation of memory at runtime (no built-in C++ 2D arrays)
- Easy to use M[ i ][ j ] element access and list-initialization syntax
- Implement multiplication operations (matrix * matrix) and (scalar * matrix)
- Implement transpose operation (square and NxM matrices)
- Implement move semantics (move constructor, move assignment)

### Dependencies
- **cmake** minimum 3.14

### Instructions
**Linux**

1.  Download & install dependencies
3.  `$ cd malg`
4.  `$ mkdir build`
5.  `$ cd build`
6.  `$ cmake -G "Unix Makefiles" ..`
7.  `$ make`
8.  `$ ./test/runtest`

### Usage

See /test/test.cpp for usage examples.

**Windows / Mac**

Note: The project has not been tested on Windows or Mac, but has been coded with portability in mind. 

Written by Antonio Redekop