set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of pixels in the structuring element"
set ylabel "Execution time (seconds)"
plot  [0:500] [0:0.3] "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):2 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):3 with linespoints title "Separable basic algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):4 with linespoints title "Moving histogram algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):5 with linespoints title "Separable moving histogram algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):6 with linespoints title "Anchor algorithm (dilation)",\
  "./images/consolidatedMorphology-perf2D.txt" using ($1*$1):7 with linespoints title "van Herk / Gil Werman algroithm (dilation)"
