set terminal png size 1000,750
set output 'const_update_R_frames.png'
set logscale y 2

set ylabel "Steps/s"
set xlabel "Size[pixel]"

set title "time to render 30 steps with different R values in normal smooth growth and fft"

plot 'result_r16.dat' using 1:(30*(1000000/$2)) title "R=16 [μs]" with linespoints, \
'result_r32.dat' using 1:(30*(1000000/$2)) title "R=32 [μs]" with linespoints, \
'result_r64.dat' using 1:(30*(1000000/$2)) title "R=64 [μs]" with linespoints, \
'result_r128.dat' using 1:(30*(1000000/$2)) title "R=128 [μs]" with linespoints, \
'result_dynamic.dat' using 1:(30*(1000000/$2)) title "R=n/2-1 [μs]" with linespoints, \
'result_fft.dat' using 1:(30*(1000000/$2)) title "fft [μs]" with linespoints
