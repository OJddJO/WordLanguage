from w import W

class Interpreter:
    def __init__(self, inputPath:str=".\\main.w", outputPath:str=".\\"):
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.w = W()


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
                case "plus":
                    #get the two values to add
                    #test if there is an expression to evaluate
                    #add the two values
                    pass
                case _ :
                    pass 

        res = {} #stores the result of the index of the word as the key and its value as the value
        #execute the words in the order of their priority



if __name__ == "__main__":
    interpreter = Interpreter()
    interpreter.interprete()
