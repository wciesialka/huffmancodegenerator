/**
 * @file Encoder.cpp
 * @brief Encode a Huffman table.
 * 
 * @author Willow Ciesialka
 * @date 2023-12-05
*/

#include "Encoder.hpp"
#include <iostream>

int huffman::calculate_frequencies(const char* input, const int input_length, char* characters, int* frequencies){
    int frequency_length = 0;

    for(int i = 0; i < input_length; i++){
        char current_character = input[i];
        // Find char in frequencies
        bool character_found = false;
        for(int j = 0; j < frequency_length; j++){
            char check_character = characters[j];
            if(current_character == check_character){
                frequencies[j]++;
                character_found = true;
                break;
            }
        }
        if(!character_found){
            characters[frequency_length] = current_character;
            frequencies[frequency_length] = 1;
            frequency_length++;
        }
    }

    // Sort the array using Bubblesort, too lazy to do a better method.
    // It don't matter. None of this matters.
    int n = frequency_length;
    while(!(n <= 1)){
        int new_n = 0;
        for(int i=1; i<n; i++){
            if(frequencies[i-1] > frequencies[i]){
                // Swap. I know there's a better way, but I don't care.
                int a = frequencies[i-1];
                int b = frequencies[i];
                frequencies[i-1] = b;
                frequencies[i] = a;
                char c = characters[i-1];
                char d = characters[i];
                characters[i-1] = d;
                characters[i] = c;

                new_n = i;
            }
        }
        n = new_n;
    }

    return frequency_length;
}

huffman::HuffmanNode* huffman::create_tree(const char* characters, const int* frequencies, const int frequency_length, huffman::HuffmanNode*** working_array){
    int list_size = frequency_length;
    int buffer_size = 256;
    int array_size = 0;
    for(int i = 0; i < frequency_length; i++){
        (*working_array)[i] = new HuffmanNode(frequencies[i], characters[i]);
        array_size++;
    }
    int start = 1;
    while(start < list_size){
        // Resize working array as necessary
        if(list_size >= buffer_size){
            buffer_size *= 2;
            HuffmanNode** tmp = new HuffmanNode*[buffer_size];
            for(int i = 0; i < list_size; i++){
                tmp[i] = (*working_array)[i];
            }
            delete[] *working_array;
            *working_array = tmp;
        }

        HuffmanNode* x = (*working_array)[start-1];
        HuffmanNode* y = (*working_array)[start];
        start += 2;

        HuffmanNode* z = new HuffmanNode(x->get_frequency() + y->get_frequency());
        z->set_left_child(x);
        z->set_right_child(y);

        bool z_inserted = false;
        // Insert z in-order
        for(int i = start; i < list_size; i++){
            if(z->get_frequency() < (*working_array)[i]->get_frequency()){
                for(int j = list_size; j > i; j--){
                    (*working_array)[j] = (*working_array)[j-1];
                }
                (*working_array)[i] = z;
                z_inserted = true;
                break;
            }
        }
        if(!z_inserted){
            (*working_array)[list_size] = z;
        }
        list_size++;
    }
    return (*working_array)[list_size-1];
}

void huffman::create_code_table(const char* characters, const int characters_length, const HuffmanNode* tree, std::string* code_table){
    for(int i = 0; i < characters_length; i++){
        std::string code = tree->encode_character(characters[i]);
        code_table[i] = code;
    }
}