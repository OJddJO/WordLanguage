def list helloWorld with int a
    print a "Hello World!"
    list create myList
    return myList
enddef

int myNumber assign 20
call helloWorld with myNumber store myReturn
print sep " | " myReturn "something" 19 myNumber

print 1 ...plus -2 ..times 2 ..minus 1 ..plus ..sqrt 2 ..power 2 ..plus 1.5 div 0.5
int a assign 1 ediv 2
int b assign 1 mod 2
print "1 // 2 =" a ", 1 % 2 =" b

# Benchmark: (On my pc)
# with Measure-Command { .\word.exe .\test.w }

# avg: 6ms

# Milliseconds      : 5
# Ticks             : 59925
# TotalMilliseconds : 5.9925