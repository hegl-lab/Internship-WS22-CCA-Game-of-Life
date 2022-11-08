set terminal png size 1000,750
set output 'smallinc.png'
set logscale y 2
set title "time for 10 steps using only fragment or compute shaders"
plot 'result_fragment_smallinc.dat' using 1:4 title "fragment[ms]" with linespoints, 'result_compute_smallinc.dat' using 1:4 title "compute[ms]" with linespoints
