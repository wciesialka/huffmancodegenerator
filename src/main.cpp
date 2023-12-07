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
    file.close();
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
    file.close();
}

/**
 * @brief Create encoded file.
 * 
 * @param[in] input Input string
 * @param[in] input_length Length of input.
 * @param[in] characters Character table
 * @param[in] code_table Code table
 * @param[in] table_size Size of character/code table
 * 
 * @returns Size of the file, in bytes.
*/
int create_encoded_file(const char* input, const int input_length, const char* characters, const std::string* code_table, const int table_size){
    std::ofstream file("encoded.bin", std::ios::out | std::ios::binary);
    int bit_count = 0;
    int byte_size = 0;
    char current_byte = 0;
    for(int i = 0; i < input_length; i++){
        char c = input[i];
        for(int j = 0; j < table_size; j++){
            if(characters[j] == c){
                std::string code = code_table[j];
                for(unsigned long k = 0; k < code.length(); k++){
                    if(code[k] == '1'){
                        current_byte |= 1 << (8 - (bit_count % 8) - 1);
                    }
                    bit_count ++;
                    if(bit_count % 8 == 0){
                        file << current_byte;
                        current_byte = 0;
                        byte_size++;
                    }
                }
                break;
            }
        }
    }
    if(bit_count % 8 > 0){
        file << current_byte;
        byte_size++;
    }
    file.write(reinterpret_cast<const char*>(&bit_count), sizeof(int));
    byte_size += sizeof(int);
    file.close();
    return byte_size;
}

/**
 * @brief Create decoded file.
 * 
 * @param[in] input Encoded input.
 * @param[in] input_length Length of input.
 * @param[in] characters Characters in code table.
 * @param[in] code_table Strings in code table.
 * @param[in] table_size Length of code table.
*/
void create_decoded_file(const char* input, const int input_length, const char* characters, const std::string* code_table, const int table_size){
    std::ofstream file("decoded.txt");

    int byte_size = 8;

    int bit_count;
    int size_difference = (int)(sizeof(int)/sizeof(char));
    int int_offset = input_length - size_difference;
    memcpy(&bit_count, input + int_offset, sizeof(int));
    std::string current_code = "";
    for(int i = 0; i < bit_count; i++){
        // Get the current byte being read.
        int character_index = i / byte_size;
        char c = input[character_index];
        // Get the current bit from that byte.
        char bit_offset = i % byte_size;
        char shift = (byte_size) - bit_offset - 1;
        char check_bit = 1 << shift;

        if(c & check_bit){
            current_code += '1';
        } else {
            current_code += '0';
        }

        // Find current code in table. If not found,
        // keep building code. Otherwise, clear current code
        // and append character to file.
        for(int j = 0; j < table_size; j++){
            if(current_code.compare(code_table[j]) == 0){
                file << characters[j];
                current_code = "";
                break;
            }
        }
    }

    file.close();
}

/**
 * @brief Read codetable.txt and turn into usable output.
 * 
 * @param[out] characters Character list.
 * @param[out] codes Code table.
 * @returns Length of code table.
*/
int read_code_table(char** characters, std::string** codes){
    std::ifstream file("codetable.txt");
    int index = 0;
    std::string line;
    while(getline(file, line)){
        char c = line[0];
        std::string code = line.substr(2, line.length() - 1);
        if(line.substr(0,2).compare("LF") == 0){
            c = '\n';
            code = code.substr(1);
        }
        (*characters)[index] = c;
        (*codes)[index] = code;
        index++;
    }

    file.close();
    return index;
}

/**
 * @brief Read the input from a text file.
 * 
 * @param[in] path Path to input file.
 * @param[out] input Input string.
 * @returns Length of input string.
*/
int read_data_from_file(const char* path, char** input){
    int block_size = 256;
    *input = new char[block_size];

    std::ifstream file(path, std::ios::in | std::ios::binary);

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

/**
 * @brief Return program usage message.
 * 
 * @returns Program usage message.
*/
const char* get_help_message(){
    return "usage: huffman [options]\n\toptions:\n\t\t-m, --mode {encode|decode}\tRun program in either encode or decode mode.\n\t\t-h, --help\tDisplay this message and quit.";
}

int main(int argc, char** argv){
    // Parse CLI options
    static struct option options[] = {
        {"mode", required_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'}
    };

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

    // Invalid mode: boot out
    if(mode == 0){
        std::cerr << argv[0] << ": Invalid mode type or no mode supplied." << std::endl;
        std::cerr << get_help_message() << std::endl;
        return 1;
    }

    // Encode
    if(mode == 1){
        char* input;
        int input_length = read_data_from_file("input.txt", &input);

        char* characters = new char[256];
        int* frequencies = new int[256];

        int frequency_length = huffman::calculate_frequencies(input, input_length, characters, frequencies);

        create_freq_txt(characters, frequencies, frequency_length);

        huffman::HuffmanNode** working_array = new huffman::HuffmanNode*[255];
        huffman::HuffmanNode* tree = huffman::create_tree(characters, frequencies, frequency_length, working_array);

        std::string* code_table = new std::string[frequency_length];
        huffman::create_code_table(characters, frequency_length, tree, code_table);

        create_codetable_txt(characters, code_table, frequency_length);

        int encoded_size = create_encoded_file(input, input_length, characters, code_table, frequency_length);
        float percent_difference = (float)(encoded_size)/(float)(input_length);
        int efficiency = (int)(percent_difference*100);
        std::cout << "File Encoded!" << std::endl;
        std::cout << "Input File Size: " << input_length << " bytes" << std::endl;
        std::cout << "Encoded File Size: " << encoded_size << " bytes (" << efficiency << "%)" << std::endl;

        delete[] code_table;
        delete[] working_array;
        delete[] characters;
        delete[] frequencies;
        delete[] input;
    }
    // Decode
    if(mode == 2){
        std::string* code_table = new std::string[256];
        char* characters = new char[256];
        int table_size = read_code_table(&characters, &code_table);
        char* input;
        int input_length = read_data_from_file("encoded.bin", &input);
        create_decoded_file(input, input_length, characters, code_table, table_size);
    }

    return 0;
}