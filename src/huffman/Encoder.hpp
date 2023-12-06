/**
 * @file Encoder.hpp
 * @brief Contains definitions for encoding functions.
 *
 * @author Willow Ciesialka
 * @date 2023-12-05
 */

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "HuffmanNode.hpp"

namespace huffman
{
    /**
     * @brief Calculate the frequencies of characters from a given input.
     * 
     * For a given string input, count the occurences of each character
     * and save it in a list.
     * 
     * @param[in] input Input string.
     * @param[in] input_length Length of input.
     * @param[out] characters Unique characters from the input.
     * @param[out] frequencies Occurences of each character. Indices match. 
     * @returns Length of frequencies table.
     */
    int calculate_frequencies(const char* input, const int input_length, char* characters, int* frequencies);

    /**
     * @brief Encode a frequency table into a Huffman tree.
     * 
     * @param[in] characters Unique characters from input string.
     * @param[in] frequencies Frequencies of unique characters.
     * @param[in] frequency_length Length of frequency table.
    */
    huffman::HuffmanNode* create_tree(const char* characters, const int* frequencies, const int frequency_length);
}

#endif