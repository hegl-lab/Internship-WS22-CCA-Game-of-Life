set terminal png size 1000,750
set output 'const_R.png'
set logscale y 2

set ylabel "Time[μs]"
set xlabel "Size[pixel]"

set title "time to render 30 steps with R=13 using no fft, simple fft, glfft"
plot 'results_const_R.dat' using 1:3 title "simple fft[μs]" with linespoints, 'results_const_R.dat' using 1:4 title "glfft[μs]" with linespoints
