int i assign 0
infloop
    # while i < 6 is equivalent to break if not(i < 6)
    if not i .less 6
        break
    endif
    change i to i plus 1
    if i equal 3
        continue
    endif
    print i
endinf

# Output:
# 1
# 2
# 4
# 5
# 6