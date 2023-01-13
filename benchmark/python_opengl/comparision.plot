set terminal png size 1000,750
set output 'const_R.png'
set logscale y 2

set ylabel "Time[μs]"
set xlabel "Size[pixel]"

set title "time to render 30 steps with R=13 using either python or opengl, run on a GTX1050ti, both using fast fourier transformations to calculate each step"
plot 'python.dat' using 1:2 title "python[μs]" with linespoints, 'fft.dat' using 1:2 title "opengl[μs]" with linespoints
