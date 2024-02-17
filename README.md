# BF16 Arithmetic and Casting Operations

This project implements a set of arithmetic and casting operations for the bfloat16 floating-point format in C, focusing on bit-level manipulations without the use of floating-point data types and their arithmetic operations. The bfloat16 format is a 16-bit floating point that represents a subset of the IEEE 754 single-precision floating-point standard, offering reduced precision while maintaining the same exponent range.

## Features

- **Conversion Functions**: Converts integers and standard `float` data types to bfloat16, and vice versa, ensuring accurate representation within the bfloat16 format's constraints.
- **Arithmetic Operations**: Implements basic arithmetic operations (addition, multiplication) between two bfloat16 numbers, utilizing bit-level manipulation to handle the operations.

## Implementation

The implementation is structured around the `bf16.c` file, which includes detailed implementations of the following functions:
- `int2bf16`: Converts an integer to bfloat16.
- `bf162int`: Converts a bfloat16 value back to an integer.
- `float2bf16`: Converts a standard IEEE 754 single-precision floating point number to bfloat16.
- `bf162float`: Converts a bfloat16 number back to a standard float.
- `bf16_add`: Performs addition between two bfloat16 numbers.
- `bf16_mul`: Performs multiplication between two bfloat16 numbers.

## Learning Outcomes

Through this project, I deepened my understanding of:
- The binary representation of integers and floating-point numbers.
- Bitwise operations in C for manipulating data at the bit level.
- The intricacies of floating-point arithmetic and precision trade-offs in computer systems.
