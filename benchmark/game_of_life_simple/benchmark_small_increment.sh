#!/bin/bash
rm result_fragment_smallinc.dat
rm result_compute_smallinc.dat

cd ..
cd ..

for i in {0..10000..100}
do
	./cmake/simple_fragment_benchmark $i $i 10 n >> benchmark/game_of_life_simple/result_fragment_smallinc.dat
	sleep 1s
	./cmake/simple_compute_benchmark $i $i 10 n >> benchmark/game_of_life_simple/result_compute_smallinc.dat
	echo $i / 10000
	sleep 1s
done

cd benchmark/game_of_life_simple

gnuplot plot_smallinc.plot

