def list helloWorld with int a
    print a "Hello World!"
    list myList
    return myList
enddef

int myNumber assign 20
call helloWorld with myNumber store myReturn
print sep " | " myReturn "something" 19 myNumber

print 1 ...plus 2 ..times 2 ..minus 1 ..plus ..sqrt 2 ..power 2 ..plus 1.5 div 0.5
print 1.5 div 0.5
print true and false or true .and 0
print 20 .lequal 20 and 20 .gequal 20
print "lesser:" 20 less 20
print "greater:" 20 greater 20
int a assign 1 ediv 2
int b assign 1 mod 2
print "1 // 2 =" a ", 1 % 2 =" b

# Benchmark:
# with Measure-Command { .\word.exe .\test.w }

# Milliseconds      : 14
# Ticks             : 142348
# TotalMilliseconds : 14.2348