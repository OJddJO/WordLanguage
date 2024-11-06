# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

Benckmark with version 0.0.8.

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 34.030275 s for 1000 iterations (avg: 0.03403 s/iter)
Word: 3.659287 s for 1000 iterations (avg: 0.003659 s/iter)
Ratio python/word: 9.3x

Running benchmark for 'while' scripts
Python: 35.609537 s for 1000 iterations (avg: 0.03561 s/iter)
Word: 3.772758 s for 1000 iterations (avg: 0.003773 s/iter)
Ratio python/word: 9.44x

Running benchmark for 'fibonacci_10' scripts
Python: 35.488974 s for 1000 iterations (avg: 0.035489 s/iter)
Word: 4.759803 s for 1000 iterations (avg: 0.00476 s/iter)
Ratio python/word: 7.46x

Running benchmark for 'fibonacci_20' scripts
Python: 36.500882 s for 1000 iterations (avg: 0.036501 s/iter)
Word: 87.358467 s for 1000 iterations (avg: 0.087358 s/iter)
Ratio python/word: 0.42x
```

On Github Actions:
```
Running benchmark for 'if' scripts
Python: 1.955134 s for 100 iterations (avg: 0.019551 s/iter)
Word: 0.384222 s for 100 iterations (avg: 0.003842 s/iter)
Ratio python/word: 5.09x

Running benchmark for 'while' scripts
Python: 1.947458 s for 100 iterations (avg: 0.019475 s/iter)
Word: 0.380653 s for 100 iterations (avg: 0.003807 s/iter)
Ratio python/word: 5.12x

Running benchmark for 'fibonacci_10' scripts
Python: 1.973353 s for 100 iterations (avg: 0.019734 s/iter)
Word: 0.518058 s for 100 iterations (avg: 0.005181 s/iter)
Ratio python/word: 3.81x

Running benchmark for 'fibonacci_20' scripts
Python: 2.144943 s for 100 iterations (avg: 0.021449 s/iter)
Word: 13.806171 s for 100 iterations (avg: 0.138062 s/iter)
Ratio python/word: 0.16x
```
