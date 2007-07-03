set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of neighbors in the structuring element"
set ylabel "Execution time (seconds)"
plot  [0:2000] [0:0.2] "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):8 with linespoints title "Basic algorithm (gradient)", \
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):9 with linespoints title "Moving histogram algorithm (gradient)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):10 with linespoints title "Anchor algorithm (gradient)",\
  "./images/consolidatedMorphology-perf2D.txt" using (($1*2+1)*($1*2+1)):11 with linespoints title "van Herk / Gil Werman algroithm (gradient)"