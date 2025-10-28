#  **Overview**

This project implements Huffman coding, a lossless data compression algorithm that efficiently encodes data based on symbol frequency. It includes both compression and decompression functionality using a binary Huffman Tree built from frequency data.

The system reads an input file, builds a Huffman Tree to encode the content into compressed bit sequences, and later reconstructs the original file by decoding the compressed data.

#  **Features**

* Huffman Tree Construction: Builds a binary tree where frequent symbols have shorter bit codes.

* File Compression: Encodes input data into a smaller binary form using the generated Huffman codes.

* File Decompression: Reconstructs the original file from the compressed data by decoding bit sequences.

* Header Serialization: Stores the Huffman tree structure in the compressed file header for accurate decompression.

* Memory Management: Recursively deletes nodes and manages pointers to prevent memory leaks.

#  **Components**

* HCTree.cpp / HCTree.hpp
Implements the Huffman Tree, including:

* build() – Constructs the tree from frequency data using a priority queue.

* encode() – Writes the encoded bit sequence for a symbol to the output stream.

* decode() – Reads bits and returns the corresponding symbol.

* write_header() / read_tree() – Serializes and deserializes the Huffman tree for file storage.

* compress.cpp
Reads input files, computes symbol frequencies, builds the Huffman tree, and writes the compressed data with a header.

* decompress.cpp
Reads the compressed file header, reconstructs the Huffman tree, and decodes the bit stream back into the original file.

# To compress
./compress <input_file> <output_file>

# To decompress
./decompress <compressed_file> <output_file>

# **Technologies Used**

* C++ Programming Language

* Priority Queue (for efficient tree construction)

*  Bitstream I/O (via FancyInputStream and FancyOutputStream)

* Recursive Tree Traversal (for encoding, decoding, and cleanup)
