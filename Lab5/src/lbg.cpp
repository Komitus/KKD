#include "lbg.hpp"
#include <map>
#include <limits>
#include <algorithm>
#include <iostream>
//#include <cstdlib>
// include <ctime>

const double _doubleMax = std::numeric_limits<double>::max();

CodeBook *generate(const PixelVec &data, size_t desiredSize, double eps)
{
    std::cout << "DESIRED SIZE: " << desiredSize << '\n';
    CodeBook *codeBook = new CodeBook();
    codeBook->clear();
    codeBook->reserve(data.size());
    Pixel c0 = _avgPixOfPixs(data);
    codeBook->push_back(c0);

    double avgDist = _avgDistortionC0(c0, data);

    while (codeBook->size() < desiredSize)
        codeBook = _split(data, codeBook, eps, avgDist);

    return codeBook;
}

CodeBook *_split(const PixelVec &data, CodeBook *codeBook, double eps, double &initialAvgDist)
{

    // enlarging Codebook
    PixelVec *enlargedCodePixels = new PixelVec();
    enlargedCodePixels->reserve(codeBook->size() * 2);

    for (auto c : *codeBook)
    {
        enlargedCodePixels->push_back(_newCodePixel(c, eps));
        enlargedCodePixels->push_back(_newCodePixel(c, -eps));
    }

    delete codeBook;

    codeBook = enlargedCodePixels;

    // end of enlarging Codebook

    double avgDist = 0.0, prevAvgDist;
    double err = eps + 1.0;

    std::cout << "Current codebook size: " << codeBook->size() << '\n';

    while (err > eps)
    {
        std::vector<Pixel> closestCList(data.size(), Pixel(0.0, 0.0, 0.0));

        std::map<uint32_t, PixelVec> pixelCNeighbours;
        std::map<uint32_t, std::vector<uint32_t>> pixelCNeighboursIdxs;

        // clusterization
        for (uint32_t i = 0; i < data.size(); i++)
        {
            double minDist = _doubleMax;
            uint32_t closestCIndex = std::numeric_limits<uint32_t>::max();

            for (uint32_t i_c = 0; i_c < codeBook->size(); i_c++)
            {
                double d = euclidSquared(data[i], (*codeBook)[i_c]);
                if (minDist == _doubleMax || d < minDist)
                {
                    minDist = d;
                    closestCList[i] = (*codeBook)[i_c];
                    closestCIndex = i_c;
                }
            }

            // adding pixels to codeword (pixel) - both value and index
            // i treat pixel as 3-dimensional vector in algorithm
            auto it = pixelCNeighbours.find(closestCIndex);
            if (it != pixelCNeighbours.end())
                pixelCNeighbours[closestCIndex].push_back(data[i]);
            else
                pixelCNeighbours.emplace(std::make_pair(closestCIndex, PixelVec()));

            it = pixelCNeighbours.find(closestCIndex);
            if (it != pixelCNeighbours.end())
                pixelCNeighboursIdxs[closestCIndex].push_back(i);
            else
                pixelCNeighboursIdxs.emplace(std::make_pair(closestCIndex, std::vector<uint32_t>()));
        }

        // end clusterization

        // generating centroids and changing values to them
        for (uint32_t i_c = 0; i_c < codeBook->size(); i_c++)
        {
            PixelVec pixels =
                (pixelCNeighbours.find(i_c) != pixelCNeighbours.end()) ? pixelCNeighbours[i_c] : PixelVec();

            uint32_t numOfPixelsNearC = pixels.size();

            if (numOfPixelsNearC > 0)
            {
                Pixel newC = _avgPixOfPixs(pixels);
                (*codeBook)[i_c] = newC;
                for (auto i = pixelCNeighboursIdxs[i_c].begin(); i != pixelCNeighboursIdxs[i_c].end(); ++i)
                    closestCList[*i] = newC;
            }
            /* trick when we have no input points assigned to output point
               but it give worse results
            else
            {
                auto highestPopulated = std::max_element(
                    std::begin(pixelCNeighbours), std::end(pixelCNeighbours),
                    [](const  std::pair<const unsigned int, PixelVec>  &p1,
                        const  std::pair<const unsigned int, PixelVec>  &p2)
                    {
                        return p1.second.size() < p2.second.size() ;
                    });

                uint32_t randFromHighestPupulatedArea =
                    std::rand()/((RAND_MAX + 1u)/((*highestPopulated).second.size()-1));

                std::cout<<"BAD CASE, idx: " << randFromHighestPupulatedArea << '\n';
                (*codeBook)[i_c] = (*highestPopulated).second[randFromHighestPupulatedArea];
            }
            */
        }
        // end of centroids

        if (avgDist > 0)
            prevAvgDist = avgDist;
        else
            prevAvgDist = initialAvgDist;

        avgDist = _avgDistortionCList(closestCList, data);

        err = (prevAvgDist - avgDist) / prevAvgDist;
    }

    initialAvgDist = avgDist;

    return codeBook;
}

Pixel _avgPixOfPixs(const PixelVec &pixels)
{
    double color1 = 0.0;
    double color2 = 0.0;
    double color3 = 0.0;

    for (auto pixel = std::begin(pixels); pixel != std::end(pixels); ++pixel)
    {
        color1 += std::get<0>(*pixel);
        color2 += std::get<1>(*pixel);
        color3 += std::get<2>(*pixel);
    }
    double size = pixels.size();
    return Pixel(color1 / size, color2 / size, color3 / size);
}

Pixel _newCodePixel(Pixel c, double e)
{
    return Pixel(
        std::get<0>(c) * (1.0 + e),
        std::get<1>(c) * (1.0 + e),
        std::get<2>(c) * (1.0 + e));
}

double _avgDistortionC0(Pixel c0, const PixelVec &data)
{
    double toRet = 0.0;

    for (auto pixel = data.begin(); pixel != std::end(data); ++pixel)
    {
        toRet += euclidSquared(c0, *pixel);
    }
    return toRet / (double)data.size();
}

double _avgDistortionCList(const PixelVec &c_list, const PixelVec &data)
{
    double toRet = 0.0;

    for (uint32_t i = 0; i < c_list.size(); i++)
    {
        toRet += euclidSquared(c_list[i], data[i]);
    }
    return toRet / (double)data.size();
}

double euclidSquared(Pixel px1, Pixel px2)
{
    double sum = 0.0;
    sum += pow(std::get<0>(px1) - std::get<0>(px2), 2);
    sum += pow(std::get<1>(px1) - std::get<1>(px2), 2);
    sum += pow(std::get<2>(px1) - std::get<2>(px2), 2);

    return sum;
}

/**
 * Quantization
 */

double MSE(const PixelVec &bitMap1, const PixelVec &bitMap2)
{
    if (bitMap1.size() != bitMap2.size())
        return -1.0;

    double toRet = 0.0;

    for (uint32_t i = 0; i < bitMap1.size(); i++)
    {
        toRet += pow(euclidSquared(bitMap1[i], bitMap2[i]), 2);
    }

    return toRet / (bitMap1.size() * 3);
}

double SNR(const PixelVec &bitMap, double mse)
{
    double sum = 0.0;

    for (uint32_t i = 0; i < bitMap.size(); i++)
    {
        sum += pow(std::get<0>(bitMap[i]), 2);
        sum += pow(std::get<1>(bitMap[i]), 2);
        sum += pow(std::get<2>(bitMap[i]), 2);
    }

    return sum / (mse * bitMap.size() * 3);
}

void quantify(PixelVec &outBitmap, const PixelVec &inBitmap, uint32_t colors)
{
    // std::srand(std::time(nullptr));
    outBitmap.clear();
    outBitmap.reserve(inBitmap.size());
    PixelVec *codeBook = generate(inBitmap, colors, 0.001);

    for (uint32_t i = 0; i < inBitmap.size(); i++)
    {
        std::vector<double> diff;
        for (auto codeBookPx = codeBook->begin(); codeBookPx != codeBook->end(); ++codeBookPx)
        {
            diff.push_back(euclidSquared(inBitmap[i], *codeBookPx));
        }
        uint32_t idx = std::min_element(diff.begin(), diff.end()) - diff.begin();
        outBitmap.push_back((*codeBook)[idx]);
    }

    delete codeBook;
}
