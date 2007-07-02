set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of pixels in the image"
set ylabel "Execution time per pixel (seconds/pixel)"
plot  [] [] "./images/consolidatedMorphology-perf_image_size.txt" using 1:6 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:7 with linespoints title "Moving histogram algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:8 with linespoints title "Anchor algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_image_size.txt" using 1:9 with linespoints title "van Herk / Gil Werman algorithm (dilation)"
