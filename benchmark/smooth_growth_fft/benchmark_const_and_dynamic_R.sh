#!/bin/bash
rm result_r16.dat
rm result_r32.dat
rm result_r64.dat
rm result_r128.dat
rm result_dynamic.dat
rm result_fft.dat

cd ..
cd ..

for i in {6..11..1}
do
	./cmake/smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 16 >> benchmark/smooth_growth_fft/result_r16.dat
	sleep 1s
	./cmake/smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 32 >> benchmark/smooth_growth_fft/result_r32.dat
	sleep 1s
	./cmake/smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 64 >> benchmark/smooth_growth_fft/result_r64.dat
	sleep 1s
	./cmake/smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 128 >> benchmark/smooth_growth_fft/result_r128.dat
	sleep 1s
	./cmake/smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 dynamic >> benchmark/smooth_growth_fft/result_dynamic.dat
	sleep 1s
	./cmake/fft_smooth_growth_benchmark $((2 ** $i)) $((2 ** $i)) 30 dynamic >> benchmark/smooth_growth_fft/result_fft.dat
	sleep 1s
done

cd benchmark/smooth_growth_fft

gnuplot plot_smallinc.plot

