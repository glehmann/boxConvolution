set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of pixels in the image"
set ylabel "Execution time (seconds)"
plot  [] [] "./images/consolidatedMorphology-perf_image_size.txt" using 1:2 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:3 with linespoints title "Moving histogram algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:4 with linespoints title "Anchor algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:5 with linespoints title "van Herk / Gil Werman algorithm (dilation)"
