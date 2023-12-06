/**
 * @file main.cpp
 * @brief Run the main driver.
 * 
 * @author Willow Ciesialka
 * @date 2023-12-05
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "huffman/Encoder.hpp"
#include <fstream>

/**
 * @brief Create the freq.txt file from input.
 * 
 * @param[in] characters List of characters.
 * @param[in] frequencies Corresponding frequencies.
 * @param[in] frequency_length Length of frequency list.
*/
void create_freq_txt(const char* characters, const int* frequencies, const int frequency_length){
    std::ofstream file("freq.txt");
    for(int i = 0; i < frequency_length; i++){
        char c = characters[i];
        if(c == '\n'){
            file << "LF";
        } else {
            file << c;
        }
        file << ' ' << frequencies[i] << std::endl;
    }
}

/**
 * @brief Create the codetable.txt file from input.
 * 
 * @param[in] characters List of characters.
 * @param[in] codes Corresponding character codes.
 * @param[in] character_length Length of character list.
*/
void create_codetable_txt(const char* characters, const std::string* codes, const int characters_length){
    std::ofstream file("codetable.txt");

    for(int i = 0; i < characters_length; i++){
        char c = characters[i];
        if(c == '\n'){
            file << "LF";
        } else {
            file << c;
        }
        file << ' ' << codes[i] << std::endl;
    }
}

/**
 * @brief Create encoded file.
 * 
 * @param[in] input Input string
 * @param[in] input_length Length of input.
 * @param[in] characters Character table
 * @param[in] code_table Code table
 * @param[in] table_size Size of character/code table
*/
void create_encoded_file(const char* input, const int input_length, const char* characters, const std::string* code_table, const int table_size){
    std::ofstream file("encoded.txt");
    for(int i = 0; i < input_length; i++){
        char c = input[i];
        for(int j = 0; j < table_size; j++){
            if(characters[j] == c){
                file << code_table[j];
                break;
            }
        }
    }
}

/**
 * @brief Read the input from a text file.
 * 
 * @param[out] input Input string.
 * @returns Length of input string.
*/
int read_text_input(char** input){
    int block_size = 256;

    std::ifstream file("input.txt");

    int input_length = 0;

    char c;
    while(file.get(c)){
        // Resize array as neccesary
        if(input_length >= block_size){
            char* tmp = new char[block_size*2];
            memcpy(tmp, *input, block_size);
            delete[] *input;
            *input = tmp;
            block_size *= 2;
        }

        (*input)[input_length] = c;
        input_length++;
    }
    file.close();

    return input_length;
}

const char* get_help_message(){
    return "usage: huffman [options]\n\toptions:\n\t\t-m, --mode {encode|decode}\tRun program in either encode or decode mode.\n\t\t-h, --help\tDisplay this message and quit.";
}

int main(int argc, char** argv){
    static struct option options[] = {
        {"mode", required_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'}
    };

    // Parse CLI options
    int c;
    int optindex;
    int mode = 0;

    while((c = getopt_long(argc, argv, "m:h", options, &optindex)) != -1){
        switch(c){
            case 'm':
                if(strcmp(optarg, "encode") == 0){
                    mode = 1;
                }
                if(strcmp(optarg, "decode") == 0){
                    mode = 2;
                }
                break;
            case 'h':
                std::cout << get_help_message() << std::endl;
                return 0;
            case '?':
                std::cerr << get_help_message() << std::endl;
                return 1;
            default:
                break;
        }
    }

    if(mode == 0){
        std::cerr << argv[0] << ": Invalid mode type or no mode supplied." << std::endl;
        std::cerr << get_help_message() << std::endl;
        return 1;
    }

    // Encode
    if(mode == 1){
        char* input = new char[256];
        int input_length = read_text_input(&input);

        char* characters = new char[256];
        int* frequencies = new int[256];

        int frequency_length = huffman::calculate_frequencies(input, input_length, characters, frequencies);

        create_freq_txt(characters, frequencies, frequency_length);

        huffman::HuffmanNode** working_array = new huffman::HuffmanNode*[255];
        huffman::HuffmanNode* tree = huffman::create_tree(characters, frequencies, frequency_length, working_array);

        std::string* code_table = new std::string[frequency_length];
        huffman::create_code_table(characters, frequency_length, tree, code_table);

        create_codetable_txt(characters, code_table, frequency_length);

        create_encoded_file(input, input_length, characters, code_table, frequency_length);

        delete[] code_table;
        delete[] working_array;
        delete[] characters;
        delete[] frequencies;
        delete[] input;
    }

    return 0;
}