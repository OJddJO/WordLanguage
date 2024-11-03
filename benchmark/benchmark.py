import timeit, subprocess, sys, os

def execute_python():
    out = subprocess.run(["python", f"./scripts/{file}.py"], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    if out.returncode != 0:
        raise Exception(f"Error while executing python script [Code: {out.returncode}]\n{out.stderr.decode('utf-8')}")

def execute_word():
    out = subprocess.run(["../bin/word.exe", f"./scripts/{file}.w"], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    if out.returncode != 0:
        raise Exception(f"Error while executing word script [Code: {out.returncode}]\n{out.stderr.decode('utf-8')}")

def execute_py_output():
    out = subprocess.run(["python", f"./scripts/{file}.py"], stdout=subprocess.PIPE)
    if out.returncode != 0:
        raise Exception(f"Error while executing python script [Code: {out.returncode}]\n{out.stderr.decode('utf-8')}")
    print(out.stdout.decode("utf-8"))

def execute_word_output():
    out = subprocess.run(["../bin/word.exe", f"./scripts/{file}.w"], stdout=subprocess.PIPE)
    if out.returncode != 0:
        raise Exception(f"Error while executing word script [Code: {out.returncode}]\n{out.stderr.decode('utf-8')}")
    print(out.stdout.decode("utf-8"))

if __name__ == "__main__":
    if len(sys.argv) > 1:
        nb_iterations = int(sys.argv[1])
    else:
        nb_iterations = 1000

    #Get list of files
    file_list = [file.split(".")[0] for file in os.listdir("./scripts") if file.endswith(".py")]

    for file in file_list: # Benchmark for each file
        print(f"Running benchmark for '{file}' scripts")

        #Python
        py_time = timeit.timeit(execute_python, number=nb_iterations)
        print(f"Python: {round(py_time, 6)} s for {nb_iterations} iterations (avg: {round(py_time/nb_iterations, 6)} s/iter)")

        #Word
        word_time = timeit.timeit(execute_word, number=nb_iterations)
        print(f"Word: {round(word_time, 6)} s for {nb_iterations} iterations (avg: {round(word_time/nb_iterations, 6)} s/iter)")

        #Ratio python/word
        print(f"Ratio python/word: {round(py_time/word_time, 2)}x")

        #Outputs
        print("Output for Python:")
        execute_py_output()
        print("Output for Word:")
        execute_word_output()
        print()
