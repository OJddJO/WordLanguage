# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

On my pc: 
> Running benchmark for 'if' scripts \
> Python: 35.004455 s for 1000 iterations (avg: 0.035004 s/iter) \
> Word: 3.819796 s for 1000 iterations (avg: 0.00382 s/iter) \
> Ratio python/word: 9.16x
> 
> Running benchmark for 'while' scripts \
> Python: 35.518133 s for 1000 iterations (avg: 0.035518 s/iter) \
> Word: 3.572016 s for 1000 iterations (avg: 0.003572 s/iter) \
> Ratio python/word: 9.94x

On Github Actions:
> Running benchmark for 'if' scripts \
> Python: 1.951868 s for 100 iterations (avg: 0.019519 s/iter) \
> Word: 0.37784 s for 100 iterations (avg: 0.003778 s/iter) \
> Ratio python/word: 5.17x
> 
> Running benchmark for 'while' scripts \
> Python: 1.943316 s for 100 iterations (avg: 0.019433 s/iter) \
> Word: 0.367091 s for 100 iterations (avg: 0.003671 s/iter) \
> Ratio python/word: 5.29x
