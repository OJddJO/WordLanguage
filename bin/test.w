int a assign 0
int b
" def int test "

infloop
    if a gequal 100
        break
    endif

    b assign a plus 1
    a assign a plus b

    print "a is" a "and b is" b

endinf

print "a is" a