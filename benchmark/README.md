# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 17.039089 s for 500 iterations (avg: 0.034078 s/iter)
Word: 1.779429 s for 500 iterations (avg: 0.003559 s/iter)
Ratio python/word: 9.58x

Running benchmark for 'while' scripts
Python: 18.221854 s for 500 iterations (avg: 0.036444 s/iter)
Word: 1.875409 s for 500 iterations (avg: 0.003751 s/iter)
Ratio python/word: 9.72x

Running benchmark for 'fibonacci' scripts
Python: 17.432245 s for 500 iterations (avg: 0.034864 s/iter)
Word: 2.396842 s for 500 iterations (avg: 0.004794 s/iter)
Ratio python/word: 7.27x
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