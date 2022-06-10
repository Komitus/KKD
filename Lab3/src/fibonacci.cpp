#include "header.hpp"

void fibonacciEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector)
{   
    std::cout << "FIB encoding\n";
    std::vector<uint32_t> fib;

    uint32_t N = 40;
    fib.resize(N + 1);
    fib[0] = 1; // orignialy f0 = 0, f1 = 1, f2 = 1, f3 = 2
    fib[1] = 1; // therefore shifted by 2
    uint32_t i;
    std::string output;

    for (i = 2; i <= N; i++)
        fib[i] = fib[i - 1] + fib[i - 2];

    for (auto x = codes.begin(); x != codes.end(); x++)
    {
        if (*x > fib.back())
        {
            i = fib.size();
            for (; fib[i - 1] <= *x; i++)
            {
                fib.insert(fib.end(), fib[i - 1] + fib[i - 2]);
            }
        }

        output = fibonacciSymbolEncode(*x, fib);
        for (uint32_t j = 0; j < output.size(); j++)
            outVector.push_back((char)output[j] - 48);
    }
}

std::string fibonacciSymbolEncode(uint32_t n, const std::vector<uint32_t> &fib)
{
    // getting largest index
    int index = 0;

    while (fib[index] <= n)
        index++;

    index--;

    std::string codeword;
    codeword.resize(index + 1);

    int i = index;

    while (n)
    {
        codeword[i - 1] = '1'; // -1 bcs we dont count F(0)

        n = n - fib[i];
        i = i - 1;

        while (i >= 1 && fib[i] > n) // zero is 1
        {
            codeword[i - 1] = '0';
            i = i - 1;
        }
    }

    codeword[index] = '1';
    return codeword;
}

void fibonacciDecode(std::string &inBits, std::vector<uint32_t> &codes)
{
    std::cout << "FIB decoding\n";
    std::vector<uint32_t> fib;
  
    uint32_t N = 40, i = 0, sum = 0;
    fib.resize(N+1);
    fib[0] = 1; // orignialy f0 = 0, f1 = 1, f2 = 1, f3 = 2
    fib[1] = 1; // therefore shifted by 2

    
    for (i = 2; i <= N; i++)
        fib[i] = fib[i - 1] + fib[i - 2];

    i = 0;
    std::string::iterator newIt;
    N++;
    
    for (auto bit = inBits.begin(); bit != inBits.end(); bit++)
    {   
        i++;
        if (i >= N)
        {
            for (; N <= i; N++)
                fib.insert(fib.end(), fib[i - 1] + fib[i - 2]);
        }

        if (*bit == '1')
        {
            sum += fib[i];

            newIt = std::next(bit);
            if (*newIt == '1')
            {
                codes.push_back(sum);
                i = 0;
                sum = 0;
                bit = newIt;
            }
        }
    }
}
