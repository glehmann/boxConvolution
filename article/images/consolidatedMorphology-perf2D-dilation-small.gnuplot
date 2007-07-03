set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of neighbors in the structuring element"
set ylabel "Execution time (seconds)"
plot  [0:2000] [0:0.2] "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):2 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):4 with linespoints title "Moving histogram algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):6 with linespoints title "Anchor algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):7 with linespoints title "van Herk / Gil Werman algroithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):3 with linespoints title "Separable basic algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):5 with linespoints title "Separable moving histogram algorithm (dilation)"
