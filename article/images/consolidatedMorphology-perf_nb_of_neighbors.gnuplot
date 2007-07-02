set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of neighbors in the structuring element"
set ylabel "Execution time (seconds)"
plot  [] [] "./images/consolidatedMorphology-perf_nb_of_neighbors.txt" using 4:6 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_nb_of_neighbors.txt" using 4:7 with linespoints title "Moving histogram algorithm (dilation)"
