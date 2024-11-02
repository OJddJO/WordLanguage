def int fibonacci with int n
    if n equal 0
        return 0
    elif n equal 1
        return 1
    else
        call fibonacci with n minus 1 store result1
        call fibonacci with n minus 2 store result2
        return result1 plus result2
    endif
enddef

call fibonacci with 10 store result
print "Fibonacci of 10 is" result
