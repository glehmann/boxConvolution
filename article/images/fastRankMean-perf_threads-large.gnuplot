set term postscript eps enhanced color
#set output "perf2D.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot  [] [] "./images/fastRankMean-perf_threads.txt" using 1:2 with linespoints title "fastApproxMaskRank", \
  "./images/fastRankMean-perf_threads.txt" using 1:3 with linespoints title "fastApproxRank", \
  "./images/fastRankMean-perf_threads.txt" using 1:4 with linespoints title "maskedRank", \
  "./images/fastRankMean-perf_threads.txt" using 1:5 with linespoints title "movingWindowMean", \
  "./images/fastRankMean-perf_threads.txt" using 1:6 with linespoints title "rank", \
  "./images/fastRankMean-perf_threads.txt" using 1:7 with linespoints title "separableMean", \
  "./images/fastRankMean-perf_threads.txt" using 1:(1.83/$2) with lines title "fastApproxMaskRank gain", \
  "./images/fastRankMean-perf_threads.txt" using 1:(1.78/$3) with lines title "fastApproxRank gain", \
  "./images/fastRankMean-perf_threads.txt" using 1:(3.99/$4) with lines title "maskedRank gain", \
  "./images/fastRankMean-perf_threads.txt" using 1:(2.17/$5) with lines title "movingWindowMean gain", \
  "./images/fastRankMean-perf_threads.txt" using 1:(4.36/$6) with lines title "rank gain", \
  "./images/fastRankMean-perf_threads.txt" using 1:(1.23/$7) with lines title "separableMean gain"
