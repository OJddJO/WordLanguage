def int fibonacci with int n
    if n less 2
        return n
    else
        call fibonacci with n minus 1 store result1
        call fibonacci with n minus 2 store result2
        return result1 plus result2
    endif
enddef

call fibonacci with 10 store result
print result