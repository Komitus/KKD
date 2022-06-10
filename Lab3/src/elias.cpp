#include "header.hpp"

void eliasGammaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector)
{
    std::cout << "GAMMA encoding\n";
    uint16_t length;
    std::bitset<32> binary;

    for (auto idx = codes.begin(); idx != codes.end(); idx++)
    {
        if (*idx > 0)
        {
            length = (uint16_t)log2(*idx);
            binary = std::bitset<32>(*idx); // to binary
            // std::cout<<binary<<'\n';
            // std::cout<<length<<'\n';

            for (uint16_t i = 0; i < length; i++)
                outVector.push_back(0);

            for (int i = length; i >= 0; i--)
                outVector.push_back(binary[i]);
        }
    }
}

void eliasGammaDecode(std::string &inBits, std::vector<uint32_t> &codes)
{

    std::cout << "GAMMA decoding\n";
    size_t currPos = 0, distance = 0, i = 0;

    while (i < inBits.size())
    {

        currPos = inBits.find_first_of('1', i);

        if (currPos == std::string::npos)
        {
            break;
        }
        distance = currPos - i + 1; // +1 bcs it was n-1
        // std::cout<<std::stoi(inBits.substr(currPos, distance), nullptr, 2)<<'\n';
        codes.push_back(std::stoi(inBits.substr(currPos, distance), nullptr, 2));
        i = currPos + distance;
    }
}

void eliasDeltaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector)
{
    std::cout << "DELTA encoding\n";

    uint16_t n, k;
    std::bitset<16> lengthBinary;
    std::bitset<32> inputBinary;

    for (auto x = codes.begin(); x != codes.end(); x++)
    {
        if (*x > 0)
        {
            n = (uint16_t)log2(*x) + 1; // lenght of input (x) in bin
            k = (uint16_t)log2(n) + 1;  // length of n in bin
            // binary = std::bitset<32>(*x); // to binary

            for (uint16_t i = 1; i <= k - 1; i++)
                outVector.push_back(0);

            lengthBinary = std::bitset<16>(n);

            for (int i = k - 1; i >= 0; i--)
                outVector.push_back(lengthBinary[i]);

            inputBinary = std::bitset<32>(*x);

            // without first
            for (int i = n - 2; i >= 0; i--)
                outVector.push_back(inputBinary[i]);
        }
    }
}

void eliasDeltaDecode(std::string &inBits, std::vector<uint32_t> &codes)
{

    std::cout << "DELTA decoding\n";
    size_t currPos = 0, distance = 0, i = 0, N = 0;
    std::string x;

    while (i < inBits.size())
    {

        currPos = inBits.find_first_of('1', i);

        if (currPos == std::string::npos)
        {
            break;
        }
        distance = currPos - i + 1; // +1 bcs it was k-1

        N = std::stoi(inBits.substr(currPos, distance), nullptr, 2) - 1; // bcs withot first "1"
        i = currPos + distance;

        x = "1" + inBits.substr(i, N);
        codes.push_back(std::stoi(x, nullptr, 2));

        i += N;
    }
}

void eliasOmegaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector)
{
    std::cout << "OMEGA encoding\n";

    uint32_t N, len;
    std::bitset<32> k;

    for (auto x = codes.rbegin(); x != codes.rend(); x++)
    { // i start from end

        N = *x;
        outVector.push_back(0);

        while (N > 1)
        {
            k = std::bitset<32>(N);
            len = (uint32_t)log2(N) + 1;
            for (uint32_t i = 0; i < len; i++) // bitset is in reversed position
                outVector.push_back(k[i]);
            N = len - 1;
        }
    }

    std::reverse(outVector.begin(), outVector.end());
}

void eliasOmegaDecode(std::string &inBits, std::vector<uint32_t> &codes)
{
    std::cout << "OMEGA decoding\n";

    size_t i = 0, N = 1;
    std::string binaryN;

    while (i < inBits.size())
    {
        if (inBits[i] == '0')
        {
            codes.push_back(N);
            N = 1;
            i++;
        }
        else
        {
            binaryN = inBits.substr(i, N + 1);
            i += N + 1;
            N = std::stoi(binaryN, nullptr, 2);
        }
    }
}
