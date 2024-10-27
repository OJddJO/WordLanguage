from math import sqrt
def helloWorld(a: int) -> list:
    print(a, "Hello World")
    myList = []
    return myList

myNumber = 20
myReturn = helloWorld(myNumber)
print(myReturn, "something", 19, myNumber, sep=" | ")

print(((1+2)*2-2+sqrt(2)**2+1.5)/0.5)
a = 1//2
b = 1%2
print("1 // 2 =", a, "1 % 2 =", b)

# Benchmark:
# with Measure-Command { python test.py }

# Milliseconds      : 66
# Ticks             : 660324
# TotalMilliseconds : 66.0324