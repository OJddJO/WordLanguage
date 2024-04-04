class W:
    #math
    @staticmethod
    def print(text):
        print(text, end='')

    @staticmethod
    def printInt(var:int, line:int) -> None:
        if type(var) != int:
            raise TypeError(f"line {line}: Using 'print int var' with a non-int/non-float variable")
        print(var, end='')

    @staticmethod
    def printStr(var:str, line:int) -> None:
        if type(var) != str:
            raise TypeError(f"line {line}: Using 'print str var' with a non-str variable")
        print(var, end='')

    @staticmethod
    def printBool(var:bool, line:int) -> None:
        if type(var) != bool:
            raise TypeError(f"line {line}: Using 'print bool var' with a non-bool variable")
        print(var, end='')

    @staticmethod
    def plus(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'plus' with non-int/non-float variables")
        return var1 + var2
    
    @staticmethod
    def minus(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'minus' with non-int/non-float variables")
        return var1 - var2
    
    @staticmethod
    def times(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'times' with non-int/non-float variables")
        return var1 * var2
    
    @staticmethod
    def div(var1:int|float, var2:int|float, line:int) -> float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'div' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 / var2
    
    @staticmethod
    def floorDiv(var1:int, var2:int, line:int) -> int:
        if type(var1) != int or type(var2) != int:
            raise TypeError(f"line {line}: Using 'floorDiv' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 // var2
    
    @staticmethod
    def power(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'power' with non-int/non-float variables")
        return var1 ** var2
    
    @staticmethod
    def mod(var1:int, var2:int, line:int) -> int:
        if type(var1) != int or type(var2) != int:
            raise TypeError(f"line {line}: Using 'mod' with non-int/non-float variables")
        if var2 == 0:
            raise ZeroDivisionError(f"line {line}: Division by zero")
        return var1 % var2
    
    @staticmethod
    def root(var1:int|float, var2:int|float, line:int) -> int|float:
        if type(var1) not in [int, float] or type(var2) not in [int, float]:
            raise TypeError(f"line {line}: Using 'root' with non-int/non-float variables")
        return var1 ** (1/var2)
    
    @staticmethod
    def factorial(var:int, line:int) -> int:
        if type(var) != int:
            raise TypeError(f"line {line}: Using 'factorial' with non-int variable")
        if var < 0:
            raise ValueError(f"line {line}: Using 'factorial' with negative number")
        if var == 0:
            return 1
        return var * W.factorial(var-1, line)
    
    @staticmethod
    def abs(var:int|float, line:int) -> int|float:
        if type(var) not in [int, float]:
            raise TypeError(f"line {line}: Using 'abs' with non-int/non-float variable")
        return abs(var)
    
    #variables
    