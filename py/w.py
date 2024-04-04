class W:
    def __init__(self):
        self._VARS = {}
        self._INT_VARS = {}
        self._FLOAT_VARS = {}
        self._STR_VARS = {}
        self._BOOL_VARS = {}

    #math
    def print(text):
        print(text, end='')

    def printInt(var:int, line:int) -> None:
        if type(var) != int:
            raise TypeError(f"line {line}: Using 'print int var' with a non-int/non-float variable")
        print(var, end='')

    def printStr(var:str, line:int) -> None:
        if type(var) != str:
            raise TypeError(f"line {line}: Using 'print str var' with a non-str variable")
        print(var, end='')

    def printBool(var:bool, line:int) -> None:
        if type(var) != bool:
            raise TypeError(f"line {line}: Using 'print bool var' with a non-bool variable")
        print(var, end='')

    def plus(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'plus' with non-int/non-float variables")
        return var1 + var2

    def minus(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'minus' with non-int/non-float variables")
        return var1 - var2
    
    def times(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'times' with non-int/non-float variables")
        return var1 * var2

    def div(var1:int|float, var2:int|float, line:int) -> float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'div' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 / var2

    def floorDiv(var1:int, var2:int, line:int) -> int:
        if type(var1) != int or type(var2) != int:
            raise TypeError(f"line {line}: Using 'floorDiv' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 // var2

    def power(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'power' with non-int/non-float variables")
        return var1 ** var2    
    
    def mod(var1:int, var2:int, line:int) -> int:
        if type(var1) != int or type(var2) != int:
            raise TypeError(f"line {line}: Using 'mod' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 % var2

    def root(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'root' with non-int/non-float variables")
        return var1 ** (1/var2)

    def factorial(var:int, line:int) -> int:
        if type(var) != int:
            raise TypeError(f"line {line}: Using 'factorial' with non-int variable")
        if var < 0:
            raise ValueError(f"line {line}: Using 'factorial' with negative number")
        if var == 0:
            return 1
        return var * W.factorial(var-1, line)

    def abs(var:int|float, line:int) -> int|float:
        if type(var) not in [int, float]:
            raise TypeError(f"line {line}: Using 'abs' with non-int/non-float variable")
        return abs(var)
    
    #variables
    def setVar(var:str, val:int|float|str|bool, line:int) -> None:
        if type(val) not in [int, float, str, bool]:
            raise TypeError(f"line {line}: Using 'set var' with non-int/non-float/non-str/non-bool variable")
        W._VARS[var] = val
