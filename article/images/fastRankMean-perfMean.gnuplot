set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Neighborhood size"
set ylabel "Execution time (seconds)"
plot  [0:2000] [0:0.1] "./images/fastRankMean-perfMeanB.txt" using (($1*2+1)*($1*2+1)):2 with linespoints title "Mean (direct algorithm)", \
  "./images/fastRankMean-perfMeanB.txt" using (($1*2+1)*($1*2+1)):3 with linespoints title "Mean (moving window algorithm)", \
  "./images/fastRankMean-perfMeanB.txt" using (($1*2+1)*($1*2+1)):4 with linespoints title "Mean (separable direct algorithm)", \
  "./images/fastRankMean-perfMeanB.txt" using (($1*2+1)*($1*2+1)):5 with linespoints title "Mean (separable moving window algorithm)"
