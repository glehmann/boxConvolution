set term postscript eps enhanced color
#set output "perf2Dpoly.eps"
set xlabel "Structuring element radius"
set ylabel "Execution time (seconds)"
plot  [0:] [0:4] "./images/consolidatedMorphology-perf2Dpoly.txt" using 1:2 with linespoints title "Moving histogram algorithm with ball structuring element (dilation)",\
  "./images/consolidatedMorphology-perf2Dpoly.txt" using 1:3 with linespoints title "Anchor algorithm with polygon structuring element  (dilation)",\
  "./images/consolidatedMorphology-perf2Dpoly.txt" using 1:4 with linespoints title "van Herk / Gil Werman algroithm with polygon structuring element (dilation)"
