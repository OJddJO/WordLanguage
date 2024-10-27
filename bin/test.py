def helloWorld(a: int) -> list:
    print(a, "Hello World")
    myList = []
    return myList

myNumber = 20
myReturn = helloWorld(myNumber)
print(myReturn, "something", 19, myNumber, sep=" | ")

# Benchmark:
# with Measure-Command { python test.py }     

# Milliseconds      : 37
# Ticks             : 375826
# TotalMilliseconds : 37.5826