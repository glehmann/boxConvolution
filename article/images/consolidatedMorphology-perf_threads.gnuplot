set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot  [] [] "./images/consolidatedMorphology-perf_threads.txt" using 1:2 with linespoints title "Basic algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:3 with linespoints title "Moving histogram algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:4 with linespoints title "Anchor algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:5 with linespoints title "van Herk / Gil Werman algorithm (dilation)", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:(3.37/$2) with lines title "Basic algorithm gain", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:(2.34/$3) with lines title "Moving histogram algorithm gain", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:(1.23/$4) with lines title "Anchor algorithm gain", \
  "./images/consolidatedMorphology-perf_threads.txt" using 1:(1.02/$5) with lines title "van Herk / Gil Werman algorithm gain"
