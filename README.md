# Ordnat

Ordnat generates C code for joint memory allocations of data structures defined in TOML files.

## Requirements

- Any C compiler such as gcc, clang, or msvc.
- Python3
- pip3 for python package installation

## Quick Start

```
# Install packages
pip3 install -r requirements.txt

# Generate header file
python3 main.py -o jointmesh.h test.toml

# Replace gcc with applicable c compiler
gcc main.c
./a.out
```

We recommend using Makefiles or CMake files to generate the necessary headers from this script before compiling c libraries/source files.

## TOML Guide

See `test.toml` for examples of data structure definitions.

```
# This is a TOML document.
# Standard libs with '<lib.h>'
# User libs with '"lib.h"'
[header]
lib = ['"math.h"', '<stdio.h>']

# Example field in data struct:
# { type = "int", name = "foo" }
[data]
mesh = [
  { type = "Vector3", name = "position" },
  { type = "int", name = "index" },
  { type = "Vector2", name = "uv" }
]
```

## Resources

- http://www.nirfriedman.com/2015/10/04/multiple-arrays-one-allocation-generically-multiarray/
- https://blog.mattnewport.com/raii/
- https://www.youtube.com/watch?v=TH9VCN6UkyQ
