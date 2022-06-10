#!/bin/bash
make
./bin/main -e ./tests/test.txt ./tests/out.bin
./bin/main -d ./tests/out.bin ./tests/out_dec.txt
./bin/main -c ./tests/test.txt ./tests/out_dec.txt
printf "\nMake noise\n\n"
./bin/main -n ./tests/out.bin ./tests/out_noise.bin -p 0.1
./bin/main -d ./tests/out_noise.bin ./tests/out_noise_dec.txt
./bin/main -c ./tests/test.txt ./tests/out_noise_dec.txt

printf "\nTGA\n"
./bin/main -e ./tests/example0.tga ./tests/example0.bin
./bin/main -d ./tests/example0.bin ./tests/example0_dec.tga
./bin/main -c ./tests/example0.tga ./tests/example0_dec.tga
printf "\nMake noise\n\n"
./bin/main -n ./tests/example0.bin ./tests/example0_noise.bin -p 0.03
./bin/main -d ./tests/example0_noise.bin ./tests/example0_noise_dec.tga
./bin/main -c ./tests/example0.tga ./tests/example0_noise_dec.tga
