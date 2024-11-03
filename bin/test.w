def int fibo with int n
    if n less 2
        return n
    else
        call fibo with n minus 1 store a
        call fibo with n minus 2 store b
        return a plus b
    endif
enddef

call fibo with 2 store result
print result