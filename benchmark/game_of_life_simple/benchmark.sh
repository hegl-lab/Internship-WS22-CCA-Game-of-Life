#!/bin/bash
cd ..
cd ..
mkdir cmake
cd cmake
cmake ..
make
cd ..
./cmake/simple_fragment_benchmark 1400 1400 1000
sleep 2s
./cmake/simple_compute_benchmark 1400 1400 1000
sleep 2s
./cmake/simple_fragment_benchmark 2000 2000 1000
sleep 2s
./cmake/simple_compute_benchmark 2000 2000 1000
sleep 2s
./cmake/simple_fragment_benchmark 2800 2800 1000
sleep 2s
./cmake/simple_compute_benchmark 2800 2800 1000
sleep 2s
./cmake/simple_fragment_benchmark 5600 5600 1000
sleep 2s
./cmake/simple_compute_benchmark 5600 5600 1000
sleep 2s
./cmake/simple_fragment_benchmark 8000 8000 1000
sleep 2s
./cmake/simple_compute_benchmark 8000 8000 1000
sleep 2s
./cmake/simple_fragment_benchmark 11000 11000 1000
sleep 2s
./cmake/simple_compute_benchmark 11000 11000 1000
sleep 2s
./cmake/simple_fragment_benchmark 16000 16000 1000
sleep 2s
./cmake/simple_compute_benchmark 16000 16000 1000
sleep 2s
./cmake/simple_fragment_benchmark 22000 22000 1000
sleep 2s
./cmake/simple_compute_benchmark 32000 32000 1000
sleep 2s

