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

use std::env;
use std::fs::File;
use std::io::{self, Read};
use std::process;
use std::usize;

/*
 * Function to print the hexadecimal dump of the buffer
 */
fn print_hexdump(buffer: &[u8], length: usize, start_offset: usize) {
    let bytes_per_line = 16; 

    for i in (0..length).step_by(bytes_per_line) {
        // Print the offset (starting point of data block)
        print!("{:08x} ", start_offset + i);

        for j in (0..bytes_per_line).step_by(2) {
            if i + j < length {
                print!("{:02x}", buffer[i + j]);

                if i + j + 1 < length {
                    print!("{:02x} ", buffer[i + j + 1]);
                }
            } else {
                // If buffer has less than 16 bytes, pad with spaces
                print!("     ");
            }
        }
        println!();
    }
}

fn main() {
    
    let mut max_bytes: usize = usize::MAX;
    let mut file_path: Option<String> = None;

    let args: Vec<String> = env::args().collect();

    /*
     * Argument parsing for "-n LEN"
     * -n is optional, meaning it dumps the entire file if omitted
     */
    let mut i = 1;
    while i < args.len() {
        if args[i] == "-n" {
            if i + 1 < args.len() {
                max_bytes = args[i + 1].parse().unwrap_or_else(|_| {
                    eprintln!("Error: -n requires a length argument.");
                    process::exit(1);
                });
                i += 1; // Skip the length argument
            } else {
                eprintln!("Error: -n requires a length argument.");
                process::exit(1);
            }
        } else {
            file_path = Some(args[i].clone());
        }
        i += 1;
    }

    let file_path = file_path.unwrap_or_else(|| {
        eprintln!("Error! Use the format: {} [-n LEN] FILE", args[0]);
        process::exit(1);
    });

    // Open the file in binary mode
    let mut file = File::open(&file_path).unwrap_or_else(|_| {
        eprintln!("Error: Could not open file {}", file_path);
        process::exit(1);
    });

    let buffer_size = 2048; // Say buffer size is 2048 bytes
    let mut buffer = vec![0; buffer_size];

    let mut offset = 0; // Tracks the number of bytes processed

    
    loop {
        match file.read(&mut buffer) {
            Ok(bytes_read) if bytes_read > 0 => {
                // Trim output if we've read more than max_bytes
                let bytes_to_print = if offset + bytes_read > max_bytes {
                    max_bytes - offset
                } else {
                    bytes_read
                };

                // Call the print_hexdump function
                print_hexdump(&buffer, bytes_to_print, offset);

                offset += bytes_to_print;

                if offset >= max_bytes {
                    break;
                }
            }
            _ => break,
        }
    }
}


