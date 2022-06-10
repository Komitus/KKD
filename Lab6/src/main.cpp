#include "encoding.hpp"
#include <stdio.h>
#include <getopt.h>
#include <chrono>

int main(int argc, char **argv)
{
    bool cflag = 0;
    bool dflag = 0;
    bool sflag = 0;
    uint8_t qSize = 4;
    char *inFilename = NULL;
    char *outFilename = NULL;
    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "c:d:k:s:h")) != -1)
    {
        switch (c)
        {
        case 'c':
            cflag = 1;
            inFilename = optarg;
            break;
        case 'd':
            dflag = 1;
            inFilename = optarg;
            break;
        case 's':
            sflag = 1;
            inFilename = optarg;
            break;
        case 'k':
            qSize = std::stoi(std::string(optarg));
            break;
        case 'h':
            fprintf(stderr, "-c --compress\n-d -- decompress\n-s --stats");
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'd')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 's')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort();
        }
    }

    if (optind < argc)
    {
        outFilename = argv[optind];
    }

    if (cflag)
    {
        std::cout << "k = " << (int)qSize << '\n';
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        encode(std::string(inFilename), outFilename, qSize);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time of encoding (printing and writing included): "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
                  << "[µs]" << std::endl;
    }
    else if (dflag)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        decode(std::string(inFilename), outFilename);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time of encoding (printing and writing included): "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
                  << "[µs]" << std::endl;
    }
    else if (sflag)
    {   
        std::cout<<"STATS: \n";
        stats(inFilename, outFilename);
    }
    return 0;
}