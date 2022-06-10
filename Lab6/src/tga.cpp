#include "tga.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>

TGA::TGA(std::string inFilename)
{
    std::ifstream input(inFilename, std::ios::binary);
    if (!input)
    {
        std::cerr << "No such file\n";
        exit(1);
    }
    
    BytesVec bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    width = (bytes[13] << 8) + bytes[12]; // little endian
    height = (bytes[15] << 8) + bytes[14];

    // std::cout<<width << '\t' <<height <<'\n';
    header = BytesVec(bytes.begin(), bytes.begin() + 18);
    footer = BytesVec(bytes.end() - 26, bytes.end());

    //std::cout<<"Header: "<<header.size() << "\nFooter: " << footer.size() << '\n';
    bitMap.reserve(width*height);

    size_t orginalSizePx =  width*height*pixelSize;

    for(size_t i = 18; i < orginalSizePx + 18; i+=pixelSize)
        bitMap.emplace_back(bytes[i], bytes[i+1], bytes[i+2]);

    std::cout<< "Size: " << width*height << "\n w*h*pixelSize = " << orginalSizePx << '\n';
    std::cout<< "BitMap Size: " << bitMap.size() << "\n in pixels: " << bitMap.size()*3 << '\n';
    /*
    if(bitMap.size() != width * height * pixelSize){ std::cerr<<"Wrong size\n"; exit(1); }
    */
    
}

void TGA::newImage(BitMap &inBitmap, std::string fileName)
{   
    BytesVec bytes(header);
    bytes.reserve(inBitmap.size() + footer.size());

    for(auto pix : inBitmap){
        bytes.push_back(pix.blue);
        bytes.push_back(pix.green);
        bytes.push_back(pix.red);
    }
    bytes.insert(bytes.end(), footer.begin(), footer.end());

    std::ofstream outFile(fileName, std::ios::binary);
    outFile.write((const char *)&bytes[0], bytes.size());
    outFile.close();
}

void TGA::newImage(BytesVec &inBytesVec, std::string fileName){

    inBytesVec.reserve(header.size() + inBytesVec.size() + footer.size());
    inBytesVec.insert(inBytesVec.begin(), header.begin(), header.end());
    inBytesVec.insert(inBytesVec.end(), footer.begin(), footer.end());

    for(size_t i = 0; i < 1; i++){
        if(inBytesVec[i] != header[i]){
            std::cerr<<"ERROR IN WRITING HEADER\n";
            exit(0);
        }
    }
    std::ofstream outFile(fileName, std::ios::binary);
    outFile.write((const char *)&inBytesVec[0], inBytesVec.size());
    outFile.close();
}
