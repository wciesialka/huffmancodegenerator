/**
 * @file Encoder.hpp
 * @brief Contains definitions for encoding functions.
 *
 * @author Willow Ciesialka
 * @date 2023-12-05
 */

#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

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
    int calculate_frequencies(char* input, int input_length, char* characters, int* frequencies);

    /**
     * @brief Encode a string into a Huffman tree.
     * 
     * @param[in] input Input string.
     * @param[in] input_length Length of input string.
    */
    void create_tree(char* input, int input_length);
}

#endif