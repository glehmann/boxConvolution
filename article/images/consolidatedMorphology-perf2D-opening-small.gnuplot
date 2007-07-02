set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of pixels in the structuring element"
set ylabel "Execution time (seconds)"
plot  [0:500] [0:0.3] "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):12 with linespoints title "Basic algorithm (opening)", \
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):13 with linespoints title "Moving histogram algorithm (opening)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):14 with linespoints title "Anchor algorithm (opening)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):15 with linespoints title "van Herk / Gil Werman algroithm (opening)"\
