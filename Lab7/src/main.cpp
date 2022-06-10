#include <stdio.h>
#include <getopt.h>
#include <cctype>
#include <cstdlib>
#include <chrono>

#include "header.hpp"

int main(int argc, char **argv)
{
    bool eflag = 0, dflag = 0, nflag = 0, cflag = 0, pflag = 0;

    uint8_t probability = 50;
    char *filename1 = NULL, *filename2 = NULL;
    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "e:d:n:c:p:h")) != -1)
    {
        switch (c)
        {
        case 'e':
            eflag = 1;
            filename1 = optarg;
            break;
        case 'd':
            dflag = 1;
            filename1 = optarg;
            break;
        case 'n':
            nflag = 1;
            filename1 = optarg;
            break;
        case 'c':
            cflag = 1;
            filename1 = optarg;
            break;
        case 'p':
            pflag = 1;
            probability = std::stod(optarg) * 100;
            break;
        case 'h':
            fprintf(stderr, "-e --encode\n-d --decode\n-n --noise\n-c --compare\np --probability\n");
            break;
        case '?':
            if (optopt == 'e')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'd')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'n')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'p')
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
        filename2 = argv[optind];
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    if (eflag)
    {
        std::cout << "ENCODE\n";
        encode(filename1, filename2);
    }
    else if (dflag)
    {
        std::cout << "DECODE\n";
        decode(filename1, filename2);
    }
    else if (nflag & pflag)
    {
        std::cout << "NOISE\n"
                  << "File in: " << filename1
                  << "\nFile out: " << filename2
                  << "\nProbability (0-100): " << (int)probability << '\n';
        noise(filename1, filename2, probability);
    }
    else if (cflag)
    {
        std::cout << "COMPARE\n";
        compare(filename1, filename2);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time (printing and writing included): "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl;
    return 0;
}