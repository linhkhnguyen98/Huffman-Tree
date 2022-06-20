/*
 * Name: Linh Nguyen
 * Email: lkn001@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file provides a skeleton for a huffman tree. For students, feel free
 * to implement the tree in HCTree.cpp (you can create a new file).
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include <stack>
#include "Helper.hpp"
using namespace std;

/**
 * A Huffman Code Tree class
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;
    void delete_node(HCNode* node);
    vector<string> symbol_code;
    bool symbol_codes_caluclated;
    //void serialization(HCNode* curr, char* argv);
public:
    int num_bits;
    // HCNode* root;

    // void serialization(HCNode* curr, FancyOutputStream& outFile);
    // HCNode* deserialization(int& index, int length);

    /**
     * Constructor, which initializes everything to null pointers
     */
    HCTree() : root(nullptr) {
        leaves = vector<HCNode*>(256, nullptr);
        symbol_code = vector<string>(256, "");
        symbol_codes_caluclated = false;
    }

    ~HCTree();

    /**
     * Use the Huffman algorithm to build a Huffman coding tree.
     * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
     *                frequency of occurrence of byte i in the input file.
     * POSTCONDITION: root points to the root of the trie, and leaves[i]
     *                points to the leaf node containing byte i.
     *
     * @param freqs frequency vector
     */
    void build(const vector<int>& freqs);

    /**
     * Write to the given FancyOutputStream the sequence of bits coding the
     * given symbol.
     * PRECONDITION: build() has been called, to create the coding tree,
     *               and initialize root pointer and leaves vector.
     *
     * @param symbol symbol to encode
     * @param out output stream for the encoded bits
     */
    void encode(unsigned char symbol, FancyOutputStream & out);

    /**
     * Return symbol coded in the next sequence of bits from the stream.
     * PRECONDITION: build() has been called, to create the coding tree, and
     *               initialize root pointer and leaves vector.
     *
     * @param in input stream to find encoded bits
     * @return a single char decoded from the input stream
     */
    unsigned char decode(FancyInputStream & in) const;

    string get_bits(unsigned char symbol);
    void write_header(FancyOutputStream & out);

    HCNode* read_node(FancyInputStream & in, HCNode* parent);
    void read_tree(FancyInputStream & in);
};
#endif // HCTREE_HPP
