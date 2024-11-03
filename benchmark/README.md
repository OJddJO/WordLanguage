# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 3.458411 s for 100 iterations (avg: 0.034584 s/iter)
Word: 0.359912 s for 100 iterations (avg: 0.003599 s/iter)
Ratio python/word: 9.61x

Running benchmark for 'while' scripts
Python: 3.489668 s for 100 iterations (avg: 0.034897 s/iter)
Word: 0.358798 s for 100 iterations (avg: 0.003588 s/iter)
Ratio python/word: 9.73x

Running benchmark for 'fibonacci_10' scripts
Python: 3.48353 s for 100 iterations (avg: 0.034835 s/iter)
Word: 0.448602 s for 100 iterations (avg: 0.004486 s/iter)
Ratio python/word: 7.77x

Running benchmark for 'fibonacci_20' scripts
Python: 3.564039 s for 100 iterations (avg: 0.03564 s/iter)
Word: 7.678685 s for 100 iterations (avg: 0.076787 s/iter)
Ratio python/word: 0.46x
```

On Github Actions:
```
Running benchmark for 'if' scripts
Python: 1.965424 s for 100 iterations (avg: 0.019654 s/iter)
Word: 0.384423 s for 100 iterations (avg: 0.003844 s/iter)
Ratio python/word: 5.11x

Running benchmark for 'while' scripts
Python: 2.005564 s for 100 iterations (avg: 0.020056 s/iter)
Word: 0.388103 s for 100 iterations (avg: 0.003881 s/iter)
Ratio python/word: 5.17x

Running benchmark for 'fibonacci_10' scripts
Python: 1.979016 s for 100 iterations (avg: 0.01979 s/iter)
Word: 0.520043 s for 100 iterations (avg: 0.0052 s/iter)
Ratio python/word: 3.81x

Running benchmark for 'fibonacci_20' scripts
Python: 2.141 s for 100 iterations (avg: 0.02141 s/iter)
Word: 13.687988 s for 100 iterations (avg: 0.13688 s/iter)
Ratio python/word: 0.16x
```