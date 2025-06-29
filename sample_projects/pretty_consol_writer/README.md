# Pretty Console Writer Sample

## Overview

The Pretty Console Writer sample demonstrates how to use the C-CLON library with a custom console output formatter.
This sample project shows how to create nicely formatted, human-readable command-line JSON output with proper indentation and structure from CLON.

## Building the Sample

### Prerequisites

- CMake
- C compiler (gcc, clang, etc.)

### Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

Run the sample with command line arguments in the following format:

```bash
./pretty_console_writer <arg1> <arg2> ... <argn>
```

### Example

Input:
```bash
./pretty_console_writer name=John age=30 passion=c
```

Output:
```json
{
    "name" : "john",
    "age" : 30,
    "passion" : "c"
}
```
