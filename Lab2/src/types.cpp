#include "types.hpp"

std::vector<bool> Node::bitString;

Node::Node(unsigned int weight, uint16_t symbol, Node *parent, Node *left, Node *right) : weight(weight), symbol(symbol), parent(parent), left(left), right(right)
{
}

Node::~Node()
{}

void Node::appendCode()
{
    if (this->parent)
    {
        if (this == this->parent->left)
        {
            this->parent->appendCode();
            Node::bitString.push_back(0);
        }
        else
        {
            this->parent->appendCode();
            Node::bitString.push_back(1);
        }
    }
    return;
}

Tree::Tree()
{
    NYT = new Node(256, 0, nullptr, nullptr, nullptr);
    root = this->NYT;
    lastNYTNumb = 511; // sum 2^k from k = 0 to log2(256) == 511 (without nyt)
}

uint16_t Tree::getLargestNode(uint16_t nodeNumber)
{
    uint16_t l = nodeNumber;
    uint16_t r = 511;
    uint16_t m = nodeNumber;

    unsigned int weight = numberedNodes[nodeNumber]->weight;

    // binsearch
    while (l <= r)
    {
        m = l + (r - l) / 2;

        if (this->numberedNodes[m]->weight == weight)
            break;
        if (this->numberedNodes[m]->weight < weight)
            l = m + 1;
        else
            r = m - 1;
    }

    for (uint16_t i = r; i >= m; i--)
    {
        if (this->numberedNodes[i]->weight == weight)
        {
            m = i;
            break;
        }
    }

    return m;
}

void Tree::interchangeNode(Node *node, uint16_t &lastNodeIdx)
{
    uint16_t idx;

    for (idx = lastNodeIdx; idx < 511; idx++)
    {
        if (this->numberedNodes[idx] == node)
        {
            break;
        }
    }
    lastNodeIdx = idx;

    uint16_t largestIdx = getLargestNode(idx);

    Node *largest = this->numberedNodes[largestIdx];

    if (node != largest && node->parent && largest->parent && node->parent != largest)
    {

        std::swap(this->numberedNodes[idx], this->numberedNodes[largestIdx]);
        std::swap(node->parent, largest->parent);

        if (node->parent->left == largest)
            node->parent->left = node;
        else
            node->parent->right = node;

        if (largest->parent->left == node)
            largest->parent->left = largest;
        else
            largest->parent->right = largest;
    }
}

void Tree::rebalance(Node *q)
{
    uint16_t idx = 0;

    for (idx = lastNYTNumb + 1; idx <= 511; idx++)
    {
        if (this->numberedNodes[idx] == q)
        {
            break;
        }
    }

    while (q != nullptr)
    {
        this->interchangeNode(q, idx);
        q->weight += 1;
        q = q->parent;
    }
}

void Tree::update(uint8_t c)
{
    // static bool tmpArr[8];
    Node *q = this->nodesOfSymbols[c];

    if (q == nullptr)
    {
        /*
        for (uint8_t i = 0; i < 8; i++)
            tmpArr[i] = (((c >> i) & 1) == 1);

        for (uint8_t i = 1; i <= 8; i++)
            Node::bitString.push_back(tmpArr[8 - i]);
        */

        if (this->distinctChars < 255)
        {
            // case 1 from update pseudocode
            Node *internalNode = new Node(1, 256, this->NYT->parent, nullptr, nullptr);
            Node *symbolNode = new Node(1, c, internalNode, nullptr, nullptr);
            this->NYT->parent = internalNode;
            internalNode->right = symbolNode;
            internalNode->left = NYT;

            if (internalNode->parent)
                internalNode->parent->left = internalNode;
            else
                this->root = internalNode;

            this->numberedNodes[lastNYTNumb - 1] = symbolNode;
            this->numberedNodes[lastNYTNumb] = internalNode;
            this->nodesOfSymbols[c] = symbolNode;

            this->lastNYTNumb -= 2;
            q = internalNode->parent;
            this->distinctChars += 1;
        }
        else
        {
            Node *lastSymbolNode = new Node(1, c, this->NYT->parent, nullptr, nullptr);
            this->NYT->parent->left = lastSymbolNode;
            this->numberedNodes[lastNYTNumb] = lastSymbolNode;
            this->nodesOfSymbols[c] = lastSymbolNode;
            lastNYTNumb -= 1;
            q = lastSymbolNode->parent;
            this->distinctChars += 1;
        }
    }

    this->rebalance(q);
}

void Tree::getCode(uint8_t c)
{

    Node *q = this->nodesOfSymbols[c];
    if (q)
    {
        q->appendCode();
    }
    else
    {
        this->NYT->appendCode();
        // append original symbol code
        for (uint8_t i = 8; i > 0; i--)
            Node::bitString.push_back((((c >> (i - 1)) & 1) == 1));
    }
}

void Tree::encode(std::string inFilename, std::string outFilename)
{
    std::ifstream input(inFilename, std::ios::binary);

    std::vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    if (bytes.size() == 0)
    {
        std::cerr << "Length of file == 0\n";
        return;
    }
    this->originalFileSize = bytes.size();

    Node::bitString.reserve(bytes.size() * 3 / 2);

    uint32_t occurences[256] = {{0}};

    for (auto it = bytes.begin(); it != bytes.end(); ++it)
    {
        occurences[*it]++;
        this->getCode(*it);
        this->update(*it);
    }

    std::cout << "END of creating tree\n";

    this->entropy = 0.0;
    double cbLog;
    double sizeLog = log2(bytes.size());

    for (uint16_t currByte = 0; currByte < 256; currByte++)
    {
        if (occurences[currByte] != 0)
        {
            cbLog = log2(occurences[currByte]);
            entropy += (sizeLog - cbLog) * occurences[currByte];
        }
    }

    this->entropy /= (double)bytes.size();

    size_t prevSize = Node::bitString.size();
    size_t toAdd = 8 - prevSize % 8;
    size_t lengthToWrite = prevSize + toAdd % 8;

    if (toAdd != 8)
    {
        unsigned int minWeight = UINT32_MAX;
        uint16_t lastId = 0;
        for (uint16_t i = 1; i < 256; i++)
        {
            if (nodesOfSymbols[i] && nodesOfSymbols[i]->weight < minWeight)
            {
                minWeight = nodesOfSymbols[i]->weight;
                lastId = i;
            }
        }

        // case 1 no NYT
        if (this->distinctChars == 256)
        {
            // std::cout << "ALL CHARS OCCURED\n";
            nodesOfSymbols[lastId]->appendCode();
        } // case 2 NYT, even when not long enough
        else
        {
            this->NYT->appendCode();
            nodesOfSymbols[lastId]->appendCode();
        }
    }

    assert(lengthToWrite % 8 == 0);
    writeToFile(outFilename, lengthToWrite);
}

void Tree::writeToFile(std::string outFilename, size_t lengthToWrite)
{
    std::ofstream outFile(outFilename, std::ios::out | std::ios::binary);

    uint8_t ret = 0;
    uint8_t tmp;
    std::cout << "Bits to write: " << lengthToWrite << '\n';
    std::cout << "VECTOR SIZE: " << Node::bitString.size() << '\n';
    size_t counter = 0;

    for (size_t i = 0; i < lengthToWrite; i += 8)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            tmp = Node::bitString.at(i + j);
            // std::cout<<static_cast<int>(tmp);
            ret |= tmp << (8 - j - 1);
        }
        // std::cout<<" ret: "<<static_cast<int>(ret)<<'\n';
        outFile << ret;
        counter++;
        ret = 0;
    }
    assert(lengthToWrite / 8 == counter);

    outFile.close();
    Node::bitString.clear();
    Node::bitString.shrink_to_fit();

    // TODO cleanup
    for(uint16_t i = 0; i < 512; i++){
        delete this->numberedNodes[i];
    }

    std::cout << "Bytes written: " << counter << '\n';
    std::cout << "Size of original file (in bytes): " << this->originalFileSize << '\n';
    std::cout << "Size of compressed file (in bytes): " << counter << '\n';
    std::cout << "##################################################################\n";
    std::cout << "# Entropy: " << this->entropy << '\n';
    std::cout << "# Avg code lenght (in bits): " << 8.0 * (double)counter / (double)this->originalFileSize << '\n';
    std::cout << "# Compression ratio (original/compressed) in bytes): " << (double)this->originalFileSize / (double)counter * 100.0 << "%\n";
}

void Tree::decode(std::string inFilename, std::string outFilename)
{
    std::ifstream input(inFilename, std::ios::binary);

    std::vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    std::vector<bool> bits;
    bits.reserve(bytes.size() * 8);
    uint8_t firstSymbol = bytes[0];

    for (size_t i = 1; i < bytes.size(); i++) //first byte is original symbol
    {

        // std::cout<<static_cast<int>(bytes[i])<<" bools: ";
        for (size_t j = i + 8; j > i; j--)
        {
            bits.push_back((((bytes[i] >> (j - i - 1)) & 1) == 1));
            // std::cout<<static_cast<int>(bits[bits.size()-1]);
        }
        // std::cout<<'\n';
    }

    assert(bytes.size()-1 == bits.size() / 8);

    std::vector<uint8_t> result;
    result.push_back(firstSymbol);

    bytes.clear();
    bytes.shrink_to_fit();

    this->update(firstSymbol);

    Node *q = this->root;

    uint8_t tmp = 0, ret = 0;
    size_t i = 0;
    uint16_t symbol = 256; // 256 is out of our range 0-255
    // int counter = 1;
    // std::cout<<"SIZE "<<bits.size()<<'\n';

    while (i < bits.size() - 1)
    {

        // std::cout<<"i :"<<i<<'\n';
        if (bits[i])
            q = q->right;
        else
            q = q->left;

        symbol = q->symbol;

        if (q == this->NYT)
        {
            // counter++;
            // std::cout<<"in nyt \n";
            ret = 0;

            i++;
            for (uint8_t j = 0; j < 8; j++)
            {
                // std::cout<<i+j<<'\n';
                tmp = bits.at(i + j);
                // std::cout<<static_cast<int>(tmp);
                ret |= tmp << (8 - j - 1);
            }
            // std::cout<<"  (bin), in nyt : "<<static_cast<char>(ret)<<'\n';
            result.push_back(ret);
            this->update(ret);
            i += 7;
            q = this->root;
        }
        else if (symbol < 256)
        {
            // std::cout<<static_cast<char>(*symbol)<<std::endl;
            result.push_back(symbol);
            this->update(symbol);
            q = this->root;
        }
        // std::cout<<"Po updacie\n";
        i++;
    }

    // std::cout<<"counter : "<<counter<<'\n';

    std::ofstream outFile(outFilename, std::ios::out | std::ios::binary);

    for (auto it = result.begin(); it != result.end(); ++it)
    {
        outFile << *it;
    }

    std::cout << "DECOMPRESSION FINISHED\n";

    for(uint16_t i = 0; i < 512; i++){
        delete this->numberedNodes[i];
    }
    outFile.close();
    bits.clear();
    bits.shrink_to_fit();
}