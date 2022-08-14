CC = clang++

CXX_FLAGS= -std=c++20

all: examples

examples:
	$(CC) $(CXX_FLAGS) example/simple_example.cpp -o simple_example
