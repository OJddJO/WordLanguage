def isEven(a):
    return a % 2 == 0

number = 4
result = isEven(number)

print(result)

if result:
    print(number, "is even")
else:
    print(number, "is odd")

# Output:
# True
# 4 is even