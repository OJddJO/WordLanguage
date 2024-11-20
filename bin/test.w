def int incr with int a endwith
    return a plus 1
enddef

int a assign 0
a assign call incr with a endwith
a assign call incr with a endwith
print a