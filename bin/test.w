def list helloWorld with int a do
    print a
    print 'Hello World!'
    list myList
    myList append a
    return myList
enddef
int myNumber assign 20
1 plus 2 times 3 root 4 minus 2
call helloWorld with myNumber store myReturn
print sep '_' myReturn "something" 19