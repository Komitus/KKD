#include "header.hpp"

void lzwEncode(const std::vector<uint8_t> &bytes, std::vector<uint32_t> &result)
{
    std::map<std::string, uint32_t> dictionary;
    result.reserve(bytes.size());

    for (uint8_t i = 0; i <= 254; i++)
    {
        dictionary[std::string(1, i)] = i + 1;
        // std::cout<<static_cast<int>(dictionary[std::string(1, i)])<<'\n';
    }
    dictionary[std::string(1, (uint8_t)255)] = (uint32_t)256;
    // std::cout<<static_cast<int>(dictionary[std::string(1, (uint8_t)255)])<<'\n'<<"END OF CREATING DICT\n";

    std::string s, cs, c;
    c = bytes[0];
    uint32_t cnt = dictionary.size() + 1;

    for (auto it = std::next(bytes.begin()); it != bytes.end(); it++)
    {
        s = *it;
        cs = c + s;

        auto tmp_cs = dictionary.find(cs);
        if (tmp_cs != dictionary.end())
        {
            c += s;
        }
        else
        {
            result.push_back(dictionary[c]);
            dictionary[cs] = cnt;
            c = s;
            cnt++;
        }
    }
    result.push_back(dictionary[c]);
    dictionary.clear();
}

void lzwDecode(const std::vector<uint32_t> &codes, std::string &result)
{
    std::map<uint32_t, std::string> dictionary;

    for (uint8_t i = 0; i <= 254; i++)
    {
        dictionary[i + 1] = std::string(1, i);
    }
    dictionary[(uint32_t)256] = std::string(1, (uint8_t)255);

    uint32_t pk = codes[0];
    std::string pc, entry;
    uint32_t cnt = dictionary.size() + 1; // its 257
    
    result.reserve(codes.size()*2);
    result = dictionary[pk];

    for (auto k = std::next(codes.begin()); k != codes.end(); k++)
    {
        pc = dictionary[pk];

        auto tmp = dictionary.find(*k);
        if (tmp != dictionary.end())
        {
            entry = dictionary[*k];
            dictionary[cnt] = pc + entry[0];
            result.append(entry);
        }
        else
        {
            entry = pc + pc[0];
            dictionary[cnt] = entry;
            result.append(entry);
        }
        pk = *k;
        cnt++;
    }
}
