# @file Makefile
# @brief Build and run the executable
#
# @author Willow Ciesialka
# @date 2023-12-05

EXEC = huffman
CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -Werror
SDIR = ./src
BDIR = ./build

CLASSES=main huffman/Encoder huffman/HuffmanNode
OBJECTS = $(addprefix $(BDIR)/,$(addsuffix .o, $(CLASSES)))

.PHONY: all clean

# Build everything - default
all: $(EXEC).out

# Clean - Delete build files and executables
clean:
	rm -rf $(BDIR)
	rm -f $(EXEC).out

# Executable
$(EXEC).out: $(OBJECTS)
	$(CC) $^ -o $@

# Build files
$(BDIR)/%.o: $(SDIR)/%.cpp
	@# Make the build directory if it doesn't exist
	@if ! [ -d $(@D) ]; then mkdir -p $(@D); fi
	$(CC) -c $^ -o $@ $(CFLAGS) 