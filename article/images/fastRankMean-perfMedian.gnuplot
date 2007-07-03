set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Neighborhood size"
set ylabel "Execution time (seconds)"
plot  [0:2000] [0:0.1] "./images/fastRankMean-perfMedianB.txt" using (($1*2+1)*($1*2+1)):2 with linespoints title "Median (direct algorithm)", \
  "./images/fastRankMean-perfMedianB.txt" using (($1*2+1)*($1*2+1)):3 with linespoints title "Median (separable Huang algorithm)", \
  "./images/fastRankMean-perfMedianB.txt" using (($1*2+1)*($1*2+1)):4 with linespoints title "Median (separable direct algorithm)", \
  "./images/fastRankMean-perfMedian.txt" using (($1*2+1)*($1*2+1)):3 with linespoints title "Median (Huang algorithm)"
