/*
 * -----------------------------------------------------------------------------
 * Program: HexDump Utility
 * Author: Neha Pradeep
 * Ownership: This code is owned by Neha Pradeep
 * Date: September 2024
 * Description: 
 *   A program that reads a binary file and prints its hexadecimal dump, 
 *   with the hexadecimal representation.
 * 
 * Usage: 
 *   ./hexdump [-n LEN] FILE
 *   -n LEN : Optional, specifies the maximum number of bytes to dump.
 *   FILE   : Path to the file for which the hex dump is to be generated.
 * 
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <cstring>

using namespace std;

void print_hexdump(const unsigned char* buffer, size_t length, size_t start_offset) {
    /* 
     * bytes_per_line = Hex Dump we need per line
     */
    const size_t bytes_per_line = 16;

    for (size_t i = 0; i < length; i += bytes_per_line) {
        /* 
         * The offset address (starting point of data block) 
         */
        cout << setw(8) << setfill('0') << hex << (start_offset + i) << " ";

        for (size_t j = 0; j < bytes_per_line; j += 2) {
            if (i + j < length) {
                cout << setw(2) << setfill('0') << hex << static_cast<int>(buffer[i + j]);
                
                if (i + j + 1 < length) {
                    cout << setw(2) << setfill('0') << hex << static_cast<int>(buffer[i + j + 1]);
                } 

                cout << " "; 
            } else {
                /* 
                 * If the buffer has less than 16 bytes, pad with spaces to align output 
                 */
                cout << "     ";
            }
        }
        cout << endl; 
    }
}

int main(int argc, char* argv[]) {
    size_t max_bytes = SIZE_MAX;
    const char* file_path = nullptr; 

    /* 
     * Argument parsing for "-n LEN" 
     * -n is considered optional here. It dumps the entire hex if omitted.
     */
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                max_bytes = atoi(argv[i + 1]);
                i++;
            } else {
                cerr << "Error: -n requires a length argument." << endl;
                return 1;
            }
        } else {
            /* 
             * If it's not '-n', assume it's the file path 
             */
            file_path = argv[i];
        }
    }

    /* 
     * Ensure a file path was provided 
     */
    if (!file_path) {
        cerr << "Error! Use the below format: " << argv[0] << " [-n LEN] FILE" << endl;
        return 1;
    }

    /* 
     * Open the file in binary mode to prevent any newline transformations 
     */
    ifstream file(file_path, ios::binary);
    if (!file) {
        cerr << "Error: Could not open file " << file_path << endl;
        return 1; 
    }

    /* 
     * Allocate a fixed-size buffer on the stack to hold chunks of the file 
     */
    const size_t buffer_size = 4096; 
    unsigned char buffer[buffer_size]; 

    size_t offset = 0; /* Tracks the number of bytes processed (used for the hex offset) */

    
    while (file.read(reinterpret_cast<char*>(buffer), buffer_size) || file.gcount()) {
        size_t bytes_read = file.gcount();

        /* 
         * If we've read more than the allowed max_bytes, trim the output 
         */
        if (offset + bytes_read > max_bytes) {
            bytes_read = max_bytes - offset; /* Limit the number of bytes to display */
        }

        /* 
         * Pass the data to print_hexdump for formatted output 
         */
        print_hexdump(buffer, bytes_read, offset);
        
        /* 
         * Update the offset (logical memory address in the dump) 
         */
        offset += bytes_read;

        /* 
         * Break if we've already printed the maximum number of allowed bytes 
         */
        if (offset >= max_bytes) {
            break;
        }
    }

    file.close(); 
    return 0;
}
