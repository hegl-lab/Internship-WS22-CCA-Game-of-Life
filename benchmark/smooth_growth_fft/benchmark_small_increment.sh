#!/bin/bash
rm result_no_fft.dat
rm result_fft.dat

cd ..
cd ..

mkdir cmake
cd cmake
cmake ..
make

for i in {6..12..1}
do
	./smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 200 >> ../benchmark/smooth_growth_fft/result_no_fft.dat
	sleep 1s
	./fft_smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 200 >> ../benchmark/smooth_growth_fft/result_fft.dat
	sleep 1s
done

cd ../benchmark/smooth_growth_fft

gnuplot fft_no_fft.plot

