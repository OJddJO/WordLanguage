# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 33.488615 s for 1000 iterations (avg: 0.033489 s/iter)
Word: 3.441121 s for 1000 iterations (avg: 0.003441 s/iter)
Ratio python/word: 9.73x

Running benchmark for 'while' scripts
Python: 33.493601 s for 1000 iterations (avg: 0.033494 s/iter)
Word: 3.35622 s for 1000 iterations (avg: 0.003356 s/iter)
Ratio python/word: 9.98x

Running benchmark for 'fibonacci_10' scripts
Python: 33.592082 s for 1000 iterations (avg: 0.033592 s/iter)
Word: 4.506011 s for 1000 iterations (avg: 0.004506 s/iter)
Ratio python/word: 7.45x

Running benchmark for 'fibonacci_20' scripts
Python: 34.694974 s for 1000 iterations (avg: 0.034695 s/iter)
Word: 76.360572 s for 1000 iterations (avg: 0.076361 s/iter)
Ratio python/word: 0.45x
```

On Github Actions:
```
Running benchmark for 'if' scripts
Python: 2.118754 s for 100 iterations (avg: 0.021188 s/iter)
Word: 0.391955 s for 100 iterations (avg: 0.00392 s/iter)
Ratio python/word: 5.41x

Running benchmark for 'while' scripts
Python: 2.076555 s for 100 iterations (avg: 0.020766 s/iter)
Word: 0.387519 s for 100 iterations (avg: 0.003875 s/iter)
Ratio python/word: 5.36x

Running benchmark for 'fibonacci_10' scripts
Python: 2.110027 s for 100 iterations (avg: 0.0211 s/iter)
Word: 0.549725 s for 100 iterations (avg: 0.005497 s/iter)
Ratio python/word: 3.84x

Running benchmark for 'fibonacci_20' scripts
Python: 2.279345 s for 100 iterations (avg: 0.022793 s/iter)
Word: 13.906515 s for 100 iterations (avg: 0.139065 s/iter)
Ratio python/word: 0.16x
```
