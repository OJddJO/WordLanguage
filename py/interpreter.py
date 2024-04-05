from math import sqrt, factorial, inf
from w import W

class Interpreter:
    def __init__(self, inputPath:str=".\\main.w", outputPath:str=".\\"):
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.w = W()
        self.words = [
            "plus", "minus", "times", "div", "floorDiv", "power", "mod", "root", "factorial",
            "int", "float", "str", "bool", "assign",
            "list", "get", "set", "append", "remove", "length",
            "print", "input", "ask",
            "and", "or", "not", "equal", "greater", "less", "gEqual", "lEqual",
            "if", "elif", "else", "endif", "then",
            "for", "in", "range", "to", "step", "endfor", "while", "endwhile",
            "def", "with", "return", "enddef"
        ]


    def setOutputPath(self, path:str):
        self.outputPath = path

    def setInputPath(self, path:str):
        self.inputPath = path


    def interprete(self):
        code = open(self.inputPath, "r").readlines()
        for i, line in enumerate(code):
            if line == "":
                continue
            self.interpreteLine(line, i+1)


    def interpreteLine(self, line:str, lineNum:int):
        words = line.split(" ")
        priority = {} #stores the index of the word as the key and its priority as the value

        def checkPriority(word:str): #returns the priority of the word
            if word not in self.words:
                return inf
            priority = 0
            for i in range(len(word)):
                if word[i] == ".":
                    priority += 1
                else:
                    break
            return priority

        for i, word in enumerate(words):
            priority[i] = checkPriority(word)
        priority = dict(sorted(priority.items(), key=lambda item: item[1], reverse=True))

        def interpreteWord(index:int, word:str):
            #interprete and execute the word
            match word[priority[i]:]:
                case "power":
                    a, b = words[index-1], words[index+1]
                    value = self.w.power(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "root":
                    a, b = words[index-1], words[index+1]
                    value = self.w.root(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "times":
                    a, b = words[index-1], words[index+1]
                    value = self.w.times(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "div":
                    a, b = words[index-1], words[index+1]
                    value = self.w.div(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "floorDiv":
                    a, b = words[index-1], words[index+1]
                    value = self.w.floorDiv(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "mod":
                    a, b = words[index-1], words[index+1]
                    value = self.w.mod(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "plus":
                    a, b = words[index-1], words[index+1]
                    value = self.w.add(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case "minus":
                    a, b = words[index-1], words[index+1]
                    value = self.w.minus(a, b, lineNum)
                    for n in [-1, 0, 1]: words[index+n] = value
                    return value
                case _ :
                    allVar = list(self.w.INT_VARS.keys()) + list(self.w.FLOAT_VARS.keys()) + list(self.w.STR_VARS.keys()) + list(self.w.BOOL_VARS.keys()) + list(self.w.LISTS.keys()) + list(self.w.FUNCTIONS.keys())
                    if word in allVar:
                        self.w.getVar(word, lineNum)
                    else:
                        raise NameError(f"line {lineNum}: {word} is not defined")




if __name__ == "__main__":
    interpreter = Interpreter()
    interpreter.interprete()
