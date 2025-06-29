# C-CLON - Command Line Object Notation for C

A lightweight library for parsing command line arguments into structured data formats like JSON.
C-CLON provides a simple, human-readable syntax for command line interfaces while maintaining powerful data structure capabilities.

## Overview

C-CLON is a library that allows parsing CLON (Command Line Object Notation) into structured data formats.
It bridges the gap between simple command line interfaces and complex data structures.

## Features

- Simple, intuitive syntax for command line arguments
- Support for complex data structures:
    - Key-value pairs
    - Arrays
    - Nested objects
    - Multiple data types (strings, numbers, booleans, null)
- Extensible writer system for different output formats
- Memory efficient C implementation
- No external dependencies
- Comprehensive error handling
- Thread-safe operations

## CLON Language Syntax

### Basic Types

#### Strings

Strings are used for object keys and can also be values.
The syntax is the same for both.

For simple strings using only characters, numbers and underscores parentheses can be omitted.

```clon
some_name
```

For complex strings with special characters, single or double quotes can be used to enclose the string.

```
"wild_string'n"
```

#### Numbers

Both integers and floating point numbers are supported.

```clon
30
```

```clon
0.3e-10
```

#### Booleans

Booleans are represented by the keywords `true` and `false`.

```clon
true
```

```clon
false
```

#### Null

Null is represented by the keyword `null`.

```clon
null
```

#### Arrays

Arrays are represented by square brackets and only contain values separated by commas.

```clon
[name_1,name_2,name_3]
```

Also, Arrays can be nested.

```clon
[[1,2],[3,4]]
```

#### Objects

Objects are represented by square brackets and contain only key-value pairs separated by commas.

```clon
[name=john,age=30]
```

For highly nested objects, other key notations can be used.
The following examples produce equivalent results.

```clon
[db=[user=[name=john]]]
```

```clon
db[user[name]]=john
```

```clon
db.user.name=john
```

### CLON Language Concept
![clon-lang-concept.drawio.svg](./clon-lang-concept.drawio.svg)
