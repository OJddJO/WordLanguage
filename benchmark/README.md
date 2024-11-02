# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 3.388713 s for 100 iterations (avg: 0.033887 s/iter)
Word: 0.361091 s for 100 iterations (avg: 0.003611 s/iter)
Ratio python/word: 9.38x

Running benchmark for 'while' scripts
Python: 3.418189 s for 100 iterations (avg: 0.034182 s/iter)
Word: 0.353599 s for 100 iterations (avg: 0.003536 s/iter)
Ratio python/word: 9.67x

Running benchmark for 'fibonacci' scripts
Python: 3.365003 s for 100 iterations (avg: 0.03365 s/iter)
Word: 0.465831 s for 100 iterations (avg: 0.004658 s/iter)
Ratio python/word: 7.22x
```

On Github Actions:
```
Running benchmark for 'if' scripts
Python: 2.042915 s for 100 iterations (avg: 0.020429 s/iter)
Word: 0.395542 s for 100 iterations (avg: 0.003955 s/iter)
Ratio python/word: 5.16x

Running benchmark for 'while' scripts
Python: 2.008363 s for 100 iterations (avg: 0.020084 s/iter)
Word: 0.376756 s for 100 iterations (avg: 0.003768 s/iter)
Ratio python/word: 5.33x

Running benchmark for 'fibonacci' scripts
Python: 2.001812 s for 100 iterations (avg: 0.020018 s/iter)
Word: 0.558315 s for 100 iterations (avg: 0.005583 s/iter)
Ratio python/word: 3.59x
```