def bool isEven with int a endwith
    return a mod 2 equal 0
enddef

int number assign 4
bool result assign call isEven with number endwith

print result

if result
    print number "is even"
else
    print number "is odd"
endif

# Output:
# true
# 4 is even