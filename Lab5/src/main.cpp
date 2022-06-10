#include "lbg.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    TGA tgaImage(argv[1]);
    PixelVec newBitmap;

    uint32_t colors = pow(2, std::stoi(argv[3]));
    quantify(newBitmap, tgaImage.bitMap, colors);

    double mse = MSE(tgaImage.bitMap, newBitmap);
    double snr = SNR(tgaImage.bitMap, mse);
    tgaImage.newImage(newBitmap, argv[2]);

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Elapsed time (in ms): " << time.count() << '\n'
              << "MSE: " << mse << "\nSNR: " << snr << '\n';
}

// ./bin/main ./tests/example1.tga ./tests/result.tga 6