set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Neighborhood size"
set ylabel "Execution time (seconds)"
plot  [0:1500] [0:10] "./images/boxConvolution-perf2D-large.txt" using (($1*2+1)*($1*2+1)):2 with linespoints title "Mean (direct algorithm)", \
  "./images/boxConvolution-perf2D-large.txt" using (($1*2+1)*($1*2+1)):3 with linespoints title "Mean (accumulation algorithm)", \
  "./images/boxConvolution-perf2D-large.txt" using (($1*2+1)*($1*2+1)):4 with linespoints title "Standard deviation (direct algorithm)", \
  "./images/boxConvolution-perf2D-large.txt" using (($1*2+1)*($1*2+1)):5 with linespoints title "Standard deviation (direct algorithm)"
