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

    return 0;
}