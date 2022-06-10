#ifndef _entropy
#define _entropy

#include <string>

class EntropyAnalizer {
    private:
        std::string filename;
        double entropy;
        double condEntropy;
        unsigned int numOfOccuredBytes;
    public:
        EntropyAnalizer(std::string filename);
        void performAnalysis();
        void printResults();
};

#endif