import timeit
import subprocess

def execute_python():
    subprocess.run(["python", "test.py"], stdout=subprocess.DEVNULL)

def execute_word():
    subprocess.run(["../bin/word.exe", "test.w"], stdout=subprocess.DEVNULL)

if __name__ == "__main__":
    nb_iterations = 10000
    py_time = timeit.timeit(execute_python, number=nb_iterations)
    print(f"Python: {round(py_time, 6)} s for {nb_iterations} iterations (avg: {round(py_time/nb_iterations, 6)} s/iter)")
    word_time = timeit.timeit(execute_word, number=nb_iterations)
    print(f"Word: {round(word_time, 6)} s for {nb_iterations} iterations (avg: {round(word_time/nb_iterations, 6)} s/iter)")
    print(f"Ratio python/word: {round(py_time/word_time, 2)}x")

# Output:
# Python: 349.520957 s for 10000 iterations (avg: 0.034952 s/iter)
# Word: 37.664156 s for 10000 iterations (avg: 0.003766 s/iter)
# Ratio python/word: 9.28x
