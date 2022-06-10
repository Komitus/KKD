#include "header.hpp"
#include <stdio.h>
#include <getopt.h>
#include <chrono>

int main(int argc, char **argv)
{
    bool cflag = 0;
    bool dflag = 0;
    char *inFilename = NULL;
    char *encoding =  (char*)"gamma";
    char *outFilename = NULL;
    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "c:d:e:")) != -1)
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
        case 'e': // encoding
            encoding = optarg;
            break;
        case '?':
            if (optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'd')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'e')
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

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if (cflag)
    {
        compress(inFilename, outFilename, encoding);
    }
    else if (dflag)
    {   
        decompress(inFilename, outFilename, encoding);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time of program running (printing and writing included): "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() 
              << "[µs]" << std::endl;

    // ./bin/main -c ./bin/tests/in/test3.bin ./bin/tests/encoded/test3.bin -e omega
    // ./bin/main -d ./bin/tests/encoded/test3.bin ./bin/tests/decoded/test3.bin -e omega
    // diff ./bin/tests/in/test3.bin ./bin/tests/decoded/test3.bin

    // ./bin/main -c ./bin/tests/in/tadeusz.txt ./bin/tests/encoded/tadeusz.bin -e omega
    // ./bin/main -d ./bin/tests/encoded/tadeusz.bin ./bin/tests/decoded/tadeusz.txt -e omega
    // diff ./bin/tests/in/tadeusz.txt ./bin/tests/decoded/tadeusz.txt
    return 0;
}
