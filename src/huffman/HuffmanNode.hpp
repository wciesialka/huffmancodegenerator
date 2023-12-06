/**
 * @file HuffmanNode.hpp
 * @brief Contains declerations for the HuffmanNode class
 *
 * @author Willow Ciesialka
 * @date 2023-12-05
 */

#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <string>

namespace huffman
{
    class HuffmanNode
    {
    public:
        HuffmanNode(const int frequency) : frequency(frequency){};
        HuffmanNode(const int frequency, const char key) : frequency(frequency), key(key){};
        ~HuffmanNode() {
            if(this->left_child){
                delete this->left_child;
            }
            if(this->right_child){
                delete this->right_child;
            }
        }

        inline int get_frequency() const { return this->frequency; }

        /**
         * @brief Find the character in the Huffman Tree.
         *
         * Find the character in the Huffman Tree, with the root
         * node being the node being called.
         *
         * @param[in] needle Character to find.
         * @returns Huffman Code of the character. "x" if could not be found.
         */
        std::string encode_character(const char needle) const;

        /**
         * @brief Decode a Huffman code into a character.
         * 
         * @param[in] sequence Huffman code to decode.
         * @param[in] sequence_length Length of sequence to decode.
         * @param[in] index Current index pointer. Initialize to zero.
         * @returns Decoded character. Returns null-character 0 on error. 
         */
        char decode_sequence(const char* sequence, const int sequence_length, int& index) const;

        inline void set_left_child(HuffmanNode *child)
        {
            this->left_child = child;
        }

        inline void set_right_child(HuffmanNode *child)
        {
            this->right_child = child;
        }

    private:
        const int frequency;
        const char key{0};

        HuffmanNode *left_child{nullptr};
        HuffmanNode *right_child{nullptr};
    };
}

#endif