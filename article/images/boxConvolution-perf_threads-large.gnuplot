set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot  [] [] "./images/boxConvolution-perf_threads-large.txt" using 1:2 with linespoints title "Mean (direct algorithm)", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:3 with linespoints title "Mean (accumulation algorithm)", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:6 with linespoints title "Standard deviation (direct algorithm)", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:7 with linespoints title "Standard deviation (direct algorithm)", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:(2.1/$2) with lines title "Mean (direct algorithm) gain", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:(0.942/$3) with lines title "Mean (accumulation algorithm) gain", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:(2.75/$6) with lines title "Standard deviation (direct algorithm) gain", \
  "./images/boxConvolution-perf_threads-large.txt" using 1:(1.57/$7) with lines title "Standard deviation (accumulation algorithm) gain"
