#include "entropy_analizer.hpp"

#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>

typedef std::pair<unsigned int, unsigned int> pairOfChars;

EntropyAnalizer::EntropyAnalizer(std::string filename)
{
    this->filename = filename;
}

void EntropyAnalizer::performAnalysis()
{
    unsigned int occurences[256] = {0};
    std::map<pairOfChars, unsigned int> condOccurences;

    std::ifstream input(filename, std::ios::binary);

    std::vector<unsigned char> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    if (bytes.size() > 0)
    {
        occurences[bytes.at(0)]++;
        pairOfChars tmp(bytes.at(0), 0);
        condOccurences.insert(
            std::pair<pairOfChars, unsigned int>(tmp, 1));
    }
    else
    {
        std::cout << "File lenght: 0" << std::endl;
    }

    unsigned int prevByte = bytes.at(0);

    for (auto it = std::next(bytes.begin()); it != bytes.end(); ++it)
    {
        unsigned int currByte = static_cast<unsigned int>(*it);
        // std::cout << currByte << std::endl;

        occurences[currByte]++;
        pairOfChars key(currByte, prevByte);

        if (condOccurences.find(key) == condOccurences.end())
        {
            condOccurences.insert(std::pair<pairOfChars, unsigned int>(key, 1));
        }
        else
        {
            condOccurences[key]++;
        }
        prevByte = currByte;
    }

    this->entropy = 0.0;
    this->condEntropy = 0.0;
    double cbLog;
    double sizeLog = log2(bytes.size());
    this->numOfOccuredBytes = 0;

    for (size_t currByte = 0; currByte < 256; currByte++)
    {
        // std::cout<<"occ["<<currByte<<"] : " << occurences[currByte]<< std::endl;
        if (occurences[currByte] != 0)
        {   
            numOfOccuredBytes++;
            cbLog = log2(occurences[currByte]);
            entropy += (sizeLog - cbLog) * occurences[currByte];

            for (auto const &keyPair : condOccurences)
            {
                if (keyPair.first.first == currByte)
                {
                    condEntropy += (cbLog - log2(keyPair.second)) * keyPair.second; 
                }
            }
        }
    }

    this->entropy /= bytes.size();
    this->condEntropy /= bytes.size();
}

void EntropyAnalizer::printResults()
{
    std::cout << "The analysis carried out for  " << this->filename << "  showed: " << std::endl;
    std::cout << "Entropy                 : " << this->entropy << std::endl;
    std::cout << "Conditional Entropy     : " << this->condEntropy << std::endl;
    std::cout << "Difference (in abs)     : " << fabs(this->entropy-this->condEntropy) << std::endl;
    std::cout << "Number of occured bytes (/256): " << this->numOfOccuredBytes << std::endl;
}

int main(int argc, char *argv[])
{   
    if(argc > 1){
        EntropyAnalizer analizer(argv[1]);
        analizer.performAnalysis();
        analizer.printResults();
    } else {
        std::cout<<"Please write filename" << std::endl;
    }
}