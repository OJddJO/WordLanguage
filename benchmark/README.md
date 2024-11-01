# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

> Running benchmark for 'if' scripts \
> Python: 35.004455 s for 1000 iterations (avg: 0.035004 s/iter) \
> Word: 3.819796 s for 1000 iterations (avg: 0.00382 s/iter) \
> Ratio python/word: 9.16x
> 
> Running benchmark for 'while' scripts \
> Python: 35.518133 s for 1000 iterations (avg: 0.035518 s/iter) \
> Word: 3.572016 s for 1000 iterations (avg: 0.003572 s/iter) \
> Ratio python/word: 9.94x