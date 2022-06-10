#ifndef _TYPES
#define _TYPES

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <climits> 
#include <cmath>
#include <fstream>
#include <iterator>
#include <cassert>
#include <string>

class Node
{
public:
    Node(unsigned int weight, uint16_t symbol, Node *parent, Node *left, Node *right);
    ~Node();
    static std::vector<bool> bitString;
    void appendCode();

    unsigned int weight;
    uint16_t symbol;
    Node *parent;
    Node *left;
    Node *right;   
};

class Tree 
{
public:
    Tree();
    Node *root;
    double entropy;
    size_t originalFileSize;
    void update(uint8_t c);
    void getCode(uint8_t c);
    void encode(std::string inFilename, std::string outFilename);
    void decode(std::string inFilename, std::string outFilename);

private:
    
    Node *NYT;
    uint16_t lastNYTNumb;
    uint16_t distinctChars = 0;
    Node *nodesOfSymbols[256] = {{nullptr}};
    Node *numberedNodes[512] = {{nullptr}};  // there are max 511 nodes but i have to deal with negative numb
    void rebalance(Node *q);
    uint16_t getLargestNode(uint16_t nodeNumber);
    void interchangeNode(Node* node, uint16_t &lastNodeIdx);
    void writeToFile(std::string outFilename, size_t lengthToWrite);
};


#endif