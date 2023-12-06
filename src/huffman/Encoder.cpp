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

    // Sort the array
    int n = frequency_length;
    while(!(n <= 1)){
        int new_n = 0;
        for(int i=1; i<n; i++){
            if(frequencies[i-1] > frequencies[i]){
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

huffman::HuffmanNode* huffman::create_tree(const char* characters, const int* frequencies, const int frequency_length){
    HuffmanNode* L[255];
    int list_size = frequency_length;
    for(int i = 0; i < frequency_length; i++){
        L[i] = new HuffmanNode(frequencies[i], characters[i]);
    }
    int index = 1;
    while(index < list_size){
        HuffmanNode* x = L[index-1];
        HuffmanNode* y = L[index];
        HuffmanNode* z = new HuffmanNode(x->get_frequency() + y->get_frequency());
        z->set_left_child(x);
        z->set_left_child(y);
        // Insert z in-order
        for(int i = index; i < list_size; i++){
            if(z->get_frequency() < L[i]->get_frequency()){
                // Resize list and shift it
                for(int j = list_size-1; j >= i; j--){
                    L[j+1] = L[j];
                }
                list_size++;
                L[i] = z;
                break;
            }
        }
        index += 2;
    }
    return L[list_size-1];
}