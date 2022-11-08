set terminal png size 1000,750
set output '1000.png'
set logscale y 2
set title "time to render 1000 steps using only fragment or compute shaders"
plot 'result_1000.dat' using 1:2 title "fragment[ms]" with linespoints, 'result_1000.dat' using 1:3 title "compute[ms]" with linespoints
