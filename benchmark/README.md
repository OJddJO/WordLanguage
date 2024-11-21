# Benchmark

This directory contains the benchmarking code for the project. The benchmarking code is written in Python and uses the `timeit` module to measure the time taken to execute the same code written in Python and WordLanguage. (see [benchmark.py](benchmark.py))

Benckmark with version 0.0.8.

## Output

On my pc:
```
Running benchmark for 'if' scripts
Python: 17.467939 s for 500 iterations (avg: 0.034936 s/iter)
Word: 1.827952 s for 500 iterations (avg: 0.003656 s/iter)
Ratio python/word: 9.56x

Running benchmark for 'while' scripts
Python: 17.47005 s for 500 iterations (avg: 0.03494 s/iter)
Word: 1.739445 s for 500 iterations (avg: 0.003479 s/iter)
Ratio python/word: 10.04x

Running benchmark for 'fibonacci_10' scripts
Python: 16.787859 s for 500 iterations (avg: 0.033576 s/iter)
Word: 2.277316 s for 500 iterations (avg: 0.004555 s/iter)
Ratio python/word: 7.37x

Running benchmark for 'fibonacci_20' scripts
Python: 17.282531 s for 500 iterations (avg: 0.034565 s/iter)
Word: 48.087615 s for 500 iterations (avg: 0.096175 s/iter)
Ratio python/word: 0.36x
```

On Github Actions:
```
Running benchmark for 'if' scripts
Python: 1.941289 s for 100 iterations (avg: 0.019413 s/iter)
Word: 0.379983 s for 100 iterations (avg: 0.0038 s/iter)
Ratio python/word: 5.11x

Running benchmark for 'while' scripts
Python: 1.936811 s for 100 iterations (avg: 0.019368 s/iter)
Word: 0.370779 s for 100 iterations (avg: 0.003708 s/iter)
Ratio python/word: 5.22x

Running benchmark for 'fibonacci_10' scripts
Python: 1.975508 s for 100 iterations (avg: 0.019755 s/iter)
Word: 0.526515 s for 100 iterations (avg: 0.005265 s/iter)
Ratio python/word: 3.75x

Running benchmark for 'fibonacci_20' scripts
Python: 2.126514 s for 100 iterations (avg: 0.021265 s/iter)
Word: 14.356268 s for 100 iterations (avg: 0.143563 s/iter)
Ratio python/word: 0.15x
```
