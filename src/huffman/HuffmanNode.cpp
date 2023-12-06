/**
 * @file HuffmanNode.cpp
 * @brief Contains definitions for the HuffmanNode class.
 *
 * @author Willow Ciesialka
 * @date 2023-12-05
 */

#include "HuffmanNode.hpp"
#include <iostream>

std::string huffman::HuffmanNode::encode_character(const char needle) const
{
    if (this->key == needle)
    {
        return "";
    }
    if (this->left_child)
    {
        std::string child_code = this->left_child->encode_character(needle);
        if (child_code != "x")
        {
            return "0" + child_code;
        }
    }
    if (this->right_child)
    {
        std::string child_code = this->right_child->encode_character(needle);
        if (child_code != "x")
        {
            return "1" + child_code;
        }
    }
    return "x";
}

char huffman::HuffmanNode::decode_sequence(const char *sequence, const int sequence_length, int &index) const
{
    if (index >= sequence_length)
    {
        return 0;
    }
    char instruction = sequence[index];
    HuffmanNode* child_node;
    switch (instruction){
        case '0':
            child_node = this->left_child;
            break;
        case '1':
            child_node = this->right_child;
            break;
        default:
            return 0;
    }
    // If we have a child node matching our instruction, then the key must
    // be further down the tree. Otherwise, we're looking at the start of a
    // new sequence, and we have thus found the corresponding key! 
    if(child_node){
        return child_node->decode_sequence(sequence, sequence_length, ++index);
    }
    return this->key;
}