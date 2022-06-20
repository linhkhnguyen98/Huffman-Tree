/**
 * Name: Linh Nguyen
 * Email: lkn001@ucsd.edu
 * Source: None.
 * 
 * This file is to take in input files from the command line argument
 * Reading the header file, then use the infomation to
 * build and Huffman tree and decode it.
 */


#include "HCTree.hpp"
#include "Helper.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <string>

using namespace std;

/**
 * Main function to take in command line input and decompress the file
 *
 */
int main(int argc, char* argv[]){
    int max = 3;
    if(argc != max){
        cout << "Invalid syntax" << endl;
        exit(1);
    }

    FancyInputStream inFile(argv[1]);
    FancyOutputStream outFile(argv[2]);

    if (inFile.filesize() > 0) {
        HCTree* tree = new HCTree();
        map<string, int> symbol_code;
        int size = 256;

        // build Huffman tree
        tree->read_tree(inFile);
        // decoding
        for (int i = 0; i < size; i++) {
            string code = tree->get_bits(i);
            symbol_code[code] = i;
        }
        
        string temp = "";
        for (int i = 0; i < tree->num_bits; i++) {
            temp.append(to_string(inFile.read_bit()));
            if (symbol_code.count(temp) > 0) {
                outFile.write_byte((char) symbol_code[temp]);
                temp = "";
            }
        }
        outFile.flush();
        delete(tree);
    }

}
