set terminal png size 1000,750
set output 'const_R.png'
set logscale y 2

set ylabel "Time[μs]"
set xlabel "Size[pixel]"

set title "time to render 60 steps with R=13 using no fft and simple fft"
plot 'results_const_R_updated.dat' using 1:2 title "no fft[μs]" with linespoints, 'results_const_R_updated.dat' using 1:3 title "simple fft[μs]" with linespoints
