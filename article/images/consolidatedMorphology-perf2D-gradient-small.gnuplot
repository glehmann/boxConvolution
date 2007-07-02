set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of pixels in the structuring element"
set ylabel "Execution time (seconds)"
plot  [0:500] [0:0.3] "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):8 with linespoints title "Basic algorithm (gradient)", \
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):9 with linespoints title "Moving histogram algorithm (gradient)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):10 with linespoints title "Anchor algorithm (gradient)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):11 with linespoints title "van Herk / Gil Werman algroithm (gradient)"