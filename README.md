# HexDump Utility

## Overview

The HexDump Utility is a program that reads a binary file and prints its hexadecimal representation along with the corresponding offsets. 
This utility is useful for inspecting the raw byte data in files. 

Code in RUST is a beginner friendly one which is ported from C++ code in this repository.

## Features

- Reads binary files and displays their hexadecimal dump.
- Allows optional length specification to limit the number of bytes dumped.
- Displays both hexadecimal values.

## Usage

./hexdump [-n LEN] FILE
