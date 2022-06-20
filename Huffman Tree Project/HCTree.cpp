/*
 * Name: Linh Nguyen
 * Email: lkn001@ucsd.edu
 *
 * Sources Used: Stepik.
 *
 * This file is the implementation to build, encode, and decode a Huffman tree
 * Using priority_queue as a container to store data
 */
#include "HCTree.hpp"
#include "Helper.hpp"

using namespace std;

#include <vector>
#include <queue>
#include <string>
#include <bitset>

/**
 * Recursive helper function to delete nodes of the tree
 * 
 * @param node the root node
 */
void HCTree::delete_node(HCNode* node){
    // Traverse through every node to delete 
    if(node != nullptr){
        // if left child exists, delete them recursively
        if(node->c0 != nullptr){
            delete_node(node->c0);
        }
        // if right child exists, delete them recursively
        if(node->c1 != nullptr){
            delete_node(node->c1);
        }
    }
    delete node;
    root = nullptr;
}
/**
 * Desstructor, which collapses everything including the tree itself
 */
HCTree::~HCTree(){
    if(root != nullptr){
        delete_node(root);
    }
}

/**
 * Use the Huffman algorithm to build a Huffman coding tree.
 * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
 *                frequency of occurrence of byte i in the input file.
 * POSTCONDITION: root points to the root of the trie, and leaves[i]
 *                points to the leaf node containing byte i.
 *
 * @param freqs frequency vector
 */
void HCTree::build(const vector<int>& freqs){
    // Create a priority_queue to store HCNode nodes
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    
    // Create a Huffman tree with the given freqs
    for(unsigned int i = 0; i < freqs.size(); i++){
        if(freqs[i] > 0){
            HCNode* newNode = new HCNode(freqs[i], i);
            leaves[i] = newNode; // add newNode to the leaves
            pq.push(leaves[i]); // push nodes to pq
        }
    }
    // Pop nodes from prority queue until size becomes 1 to set the root
    // at the end
    while(pq.size() > 1){
        // create parent node
        HCNode* parent = new HCNode(0, 0);

        // pop two smallest nodes in pq
        parent->c0 = pq.top();
        pq.pop();

        parent->c1 = pq.top();
        pq.pop();

        // getting sum of two smallest elements
        parent->count = parent->c1->count + parent->c0->count;
        // Keep track of the parent of the children
        parent->c1->p = parent;
        parent->c0->p = parent;
        pq.push(parent);
    }
    //Set root to highest node if not reading from empty file
    root = pq.top();
    pq.pop();
}

string HCTree::get_bits(unsigned char symbol) {
    if (symbol_codes_caluclated) {
        return symbol_code[symbol];
    }
    // access node at symbol index
    HCNode* node = leaves[(int) symbol];
    if (node == nullptr) return "";

    string code = "";
    if (node->p == nullptr) {
        code = "0";
    }
    else {
        // push 0 onto code for left child, 1 for right child
        while(node != nullptr && node->p != nullptr){
            // if there's a left node
            if(node->p->c0 == node){
                code.insert(0, 1, '0');
            }
            // if there's a right node
            else if(node->p->c1 == node){
                code.insert(0, 1, '1');
            }
            // else
            node = node->p;
        }
    }
    symbol_code[symbol] = code;

    return code;
}

/**
 * Write to the given FancyOutputStream the sequence of bits coding the
 * given symbol.
 * PRECONDITION: build() has been called, to create the coding tree,
 *               and initialize root pointer and leaves vector.
 *
 * @param symbol symbol to encode
 * @param out output stream for the encoded bits
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) {
   string code = get_bits(symbol);
    for (char& c : code) {
        out.write_bit(c - '0');
    }
}

/**
 * Return symbol coded in the next sequence of bits from the stream.
 * PRECONDITION: build() has been called, to create the coding tree, and
 *               initialize root pointer and leaves vector.
 *
 * @param in input stream to find encoded bits
 * @return a single char decoded from the input stream
 */
unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode* curr = root;
    unsigned char c; //individual bit

    // while curr is not a leaf node
    while(curr->c0 != nullptr || curr->c1 != nullptr){
        // getting a single bit from the file
        c = in.read_bit();
        // if the current bit is 1, move to the right
        if(c){
            curr = curr->c1;
        }
        // if the current bit is 0, move to the left
        else{
            curr = curr->c0;
        }
    }
    // return the current symbol
    return curr->symbol;
}

void write_header_node(HCNode* node, FancyOutputStream & out) {
    if (node->c0 != nullptr) {
        out.write_bit(0);
        write_header_node(node->c0, out);
        write_header_node(node->c1, out);
    }
    else {
        out.write_bit(1);
        bitset<8> bin_x(node->symbol);
        string bits = bin_x.to_string();
        for (char& c : bits) {
            out.write_bit(c - '0');
        }
    }
}
void HCTree::write_header(FancyOutputStream & out) {
    int num_of_bits = 0;
    for (int i = 0; i < 256; i++) {
        if (leaves[i] != nullptr) {
            num_of_bits += get_bits(i).length() * leaves[i]->count;
        }
    }
    symbol_codes_caluclated = true;
    out.write(num_of_bits);
    write_header_node(root, out);
}

HCNode* HCTree::read_node(FancyInputStream & in, HCNode* parent) {
    HCNode* node = new HCNode(0, 0);
    node->p = parent;

    if (in.read_bit() == 1) {
        string s = "";
        for (int i = 0; i < 8; i++) {
            s.append(to_string(in.read_bit()));
        }
        node->symbol = stoi(s, 0, 2);
        leaves[node->symbol] = node;
    } else {
        node->c0 = read_node(in, node);
        node->c1 = read_node(in, node);
    }
    return node;
}
void HCTree::read_tree(FancyInputStream & in) {
    num_bits = in.read<int>();
    root = read_node(in, nullptr);
}
