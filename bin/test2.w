list create matrix
list create row
list append row value 1
print row
delete row #delete row and free memory
list create row
list append matrix value row
print matrix

#test: def func in a func

def null func3
enddef

def int func1

    def int func2
        return 1
    enddef

    call func3
    call func2 store a
    return a

enddef

call func1 store a
print a
