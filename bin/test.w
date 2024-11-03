int a assign 0
def null test
    infloop
        if a gequal 5
            break
        endif
        change a to a plus 1
    endinf
enddef

call test
print a