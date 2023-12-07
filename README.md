# huffmancodegenerator
Generate Huffman Codes, encode data, and decode data.

## Setup

### Prerequisites

- C++11
- GNU Make[^1]

[^1]: Build requirement only.

### Building

Run `make` or `make all`.

## Running

After building, you may run the program from the command line. Run `huffman --mode encode` to encode, or `huffman --mode decode` to decode.

Usage:
```
usage: huffman [options]
	options:
		-m, --mode {encode|decode}	Run program in either encode or decode mode.
		-h, --help	Display this message and quit.
```

### Encoding

Encoding expects the following input in the same directory as being run:
- `input.txt`, containing the input data to be encoded.

It will generate three output files:
- `freq.txt` contains frequencies of characters.
- `codetable.txt` contains the code table.
- `encoded.bin` contains the encoded data.

### Decoding

Encoding expects the following input in the same directory as being run:
- `encoded.bin`, containing the encoded data.
- `codetable.txt`, containing the code table for the Huffman tree.

It will produce one output file:
- `decoded.txt`, containing the decoded data.

## Author

- Willow Ciesialka