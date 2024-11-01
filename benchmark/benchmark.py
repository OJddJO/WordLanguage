import timeit
import subprocess

def execute_python():
    subprocess.run(["python", f"./scripts/{file}.py"], stdout=subprocess.DEVNULL)

def execute_word():
    subprocess.run(["../bin/word.exe", f"./scripts/{file}.w"], stdout=subprocess.DEVNULL)

def execute_py_output():
    subprocess.run(["python", f"./scripts/{file}.py"])

def execute_word_output():
    subprocess.run(["../bin/word.exe", f"./scripts/{file}.w"])

if __name__ == "__main__":
    nb_iterations = 1000
    file_list = ["if", "while"]
    for file in file_list:
        print(f"Running benchmark for '{file}' scripts")
        py_time = timeit.timeit(execute_python, number=nb_iterations)
        print(f"Python: {round(py_time, 6)} s for {nb_iterations} iterations (avg: {round(py_time/nb_iterations, 6)} s/iter)")
        word_time = timeit.timeit(execute_word, number=nb_iterations)
        print(f"Word: {round(word_time, 6)} s for {nb_iterations} iterations (avg: {round(word_time/nb_iterations, 6)} s/iter)")
        print(f"Ratio python/word: {round(py_time/word_time, 2)}x")
        print("Output for python:")
        execute_py_output()
        print("Output for word:")
        execute_word_output()
        print()
