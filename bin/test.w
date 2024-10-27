def list helloWorld with int a
    print a "Hello World!"
    list myList
    return myList
enddef

int myNumber assign 20
call helloWorld with myNumber store myReturn
print sep " | " myReturn "something" 19 myNumber

# Benchmark:
# with Measure-Command { .\word.exe .\test.w }

# Milliseconds      : 6
# Ticks             : 62894
# TotalMilliseconds : 6.2894

1 plus 2