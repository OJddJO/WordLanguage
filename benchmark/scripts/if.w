def bool isEven with int a
    return a .mod 2 equal 0
enddef

int number assign 4
call isEven with number store result

print result

if result
    print number "is even"
else
    print number "is odd"
endif

# Output:
# true
# 4 is even