/**
 * Name: Linh Nguyen
 * Email: lkn001@ucsd.edu
 * Source: None.
 * 
 * This file is to take in input files from the command line argument
 * Reading the file and getting the frequency of the data then use it
 * to write the header file, build a Huffman tree and encode it.
 * 
 */

#include "HCTree.hpp"
#include "Helper.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


/**
 * Main function to take in command line input and compress the file
 *
 */

int main(int argc, char* argv[]){
    // checking input argument errors
    if(argc != 3){
        cout << "Invalid number of arguments" << endl;
        exit(1);
    }
    // initialize variables
    FancyInputStream inFile(argv[1]);
    FancyOutputStream outFile(argv[2]);
    
    if (inFile.filesize() > 0) {
        HCTree* tree = new HCTree();
        vector<int> freqs(256,0);
        vector<unsigned char> char_vec;
        int char_byte = 0;

        // reading input file, and getting frequency of characters
        while((char_byte = inFile.read_byte()) >= 0){ 
            char_vec.push_back(char_byte);
            //if we don't find the same byte in our map then it's a new byte
            if(freqs[char_byte] == 0){ 
                freqs[char_byte] = 1;
            }
            else{
                //increment the value
                freqs[char_byte] += 1; 
            }
        }

        // build Huffman tree
        tree->build(freqs);

        // write header
        tree->write_header(outFile);
        
        // encoding
        for(unsigned int i = 0;i < char_vec.size();i++){
            tree->encode(char_vec[i], outFile); 
        }

        outFile.flush();
        delete(tree);
    }
}


