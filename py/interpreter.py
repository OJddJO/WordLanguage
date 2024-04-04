import os

class Interpreter:
    def __init__(self):
        self.outputPath = "./"
        self.inputPath = "./main.w"

    def setOutputPath(self, path):
        self.outputPath = path

    def setInputPath(self, path):
        self.inputPath = path

    def interprete(self):
        code = open(self.inputPath, "r").readlines()
        for line in code:
            line = line.split(" ")