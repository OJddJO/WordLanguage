#int i assign 0
#infloop
#    if i greater 10
#        break
#    endif
#    print i
#    change i to i plus 1
#endinf

def bool isEven with int a
    return a .mod 2 equal 0
enddef

int number assign 4
call isEven with number store result

if result
    print number "is even"
else
    print number "is odd"
endif