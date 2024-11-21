def int fibonacci with int n endwith
    if n equal 0
        return 0
    elif n equal 1
        return 1
    else
        int result1 assign call fibonacci with n minus 1 endwith
        int result2 assign call fibonacci with n minus 2 endwith
        return result1 plus result2
    endif
enddef

int res assign call fibonacci with 10 endwith
print "Fibonacci of 10 is" res
