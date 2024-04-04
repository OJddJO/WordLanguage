class W:
    def __init__(self):
        self._INT_VARS = {}
        self._FLOAT_VARS = {}
        self._STR_VARS = {}
        self._BOOL_VARS = {}
        self._LISTS = {}
        self._FUNCTIONS = {}

    #math
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
    def setIntVar(var:str, val:int, line:int) -> None:
        if type(val) != int:
            raise TypeError(f"line {line}: Using 'int {var} assign' with non-int value")
        W._INT_VARS[var] = val

    def setFloatVar(var:str, val:float, line:int) -> None:
        if type(val) != float:
            raise TypeError(f"line {line}: Using 'float {var} assign' with non-float value")
        W._FLOAT_VARS[var] = val

    def setStrVar(var:str, val:str, line:int) -> None:
        if type(val) != str:
            raise TypeError(f"line {line}: Using 'str {var} assign' with non-str value")
        W._STR_VARS[var] = val

    def setBoolVar(var:str, val:bool, line:int) -> None:
        if type(val) != bool:
            raise TypeError(f"line {line}: Using 'bool {var} assign' with non-bool value")
        W._BOOL_VARS[var] = val
    
    def getVar(var:str, line:int) -> int|float|str|bool:
        if var in W._INT_VARS:
            return W._INT_VARS[var]
        elif var in W._FLOAT_VARS:
            return W._FLOAT_VARS[var]
        elif var in W._STR_VARS:
            return W._STR_VARS[var]
        elif var in W._BOOL_VARS:
            return W._BOOL_VARS[var]
        else:
            raise NameError(f"line {line}: Using undeclared variable '{var}'")
    
    #lists
    def initList(var:str, elements:list[int], line:int) -> None:
        if type(any(elements)) != int:
            raise TypeError(f"line {line}: Using 'list {var} assign' with non-int elements")
        W._LISTS[var] = list(elements)

    def appendList(var:str, element:int, line:int) -> None:
        if type(element) != int:
            raise TypeError(f"line {line}: Using 'append {var}' with non-int element")
        if var not in W._LISTS:
            raise NameError(f"line {line}: Using undeclared list '{var}'")
        W._LISTS[var].append(element)

    def removeList(var:str, element:int, line:int) -> None:
        if type(element) != int:
            raise TypeError(f"line {line}: Using 'remove {var}' with non-int element")
        if var not in W._LISTS:
            raise NameError(f"line {line}: Using undeclared list '{var}'")
        W._LISTS[var].pop(element)

    def getList(var:str, index:int, line:int) -> int:
        if type(index) != int:
            raise TypeError(f"line {line}: Using 'get {var}' with non-int index")
        if var not in W._LISTS:
            raise NameError(f"line {line}: Using undeclared list '{var}'")
        if index >= len(W._LISTS[var]) or index < 0:
            raise IndexError(f"line {line}: Index out of range")
        return W._LISTS[var][index]

    def setList(var:str, index:int, element:int, line:int) -> None:
        if type(index) != int:
            raise TypeError(f"line {line}: Using 'set {var}' with non-int index")
        if type(element) != int:
            raise TypeError(f"line {line}: Using 'set {var}' with non-int element")
        if var not in W._LISTS:
            raise NameError(f"line {line}: Using undeclared list '{var}'")
        if index >= len(W._LISTS[var]) or index < 0:
            raise IndexError(f"line {line}: Index out of range")
        W._LISTS[var][index] = element

    def lenList(var:str, line:int) -> int:
        if var not in W._LISTS:
            raise NameError(f"line {line}: Using undeclared list '{var}'")
        return len(W._LISTS[var])

    #output
    def print(text:str, line:int, end:str='') -> None:
        if type(text) != str:
            print(f"line {line}: Using 'print' with a non-str variable is deprecated")
        print(text, end=end)

    def printInt(var:int, line:int, end:str='') -> None:
        if type(var) != int:
            raise TypeError(f"line {line}: Using 'print int' with a non-int/non-float variable")
        print(var, end=end)

    def printStr(var:str, line:int, end:str='') -> None:
        if type(var) != str:
            raise TypeError(f"line {line}: Using 'print str' with a non-str variable")
        print(var, end=end)

    def printBool(var:bool, line:int, end:str='') -> None:
        if type(var) != bool:
            raise TypeError(f"line {line}: Using 'print bool' with a non-bool variable")
        print(var, end=end)

    #logic
    
    def _or(var1:bool, var2:bool, line:int) -> bool:
        return var1 or var2
    
    def _and(var1:bool, var2:bool, line:int) -> bool:
        return var1 and var2
    
    def _not(var:bool, line:int) -> bool:
        return not var
    
    #comparisons
    def equal(var1:int|float|str|bool, var2:int|float|str|bool, line:int) -> bool:
        return var1 == var2

    def greater(var1:int|float, var2:int|float, line:int) -> bool:
        if var1 not in [int, float] or var2 not in [int, float]:
            raise TypeError(f"line {line}: Using 'greater' with non-int/non-float variables")
        return var1 > var2
    
    def less(var1:int|float, var2:int|float, line:int) -> bool:
        if var1 not in [int, float] or var2 not in [int, float]:
            raise TypeError(f"line {line}: Using 'less' with non-int/non-float variables")
        return var1 < var2
    
    def gEqual(var1:int|float, var2:int|float, line:int) -> bool:
        if var1 not in [int, float] or var2 not in [int, float]:
            raise TypeError(f"line {line}: Using 'gEqual' with non-int/non-float variables")
        return var1 >= var2

    def lEqual(var1:int|float, var2:int|float, line:int) -> bool:
        if var1 not in [int, float] or var2 not in [int, float]:
            raise TypeError(f"line {line}: Using 'lEqual' with non-int/non-float variables")
        return var1 <= var2

    #functions
    def defineFunction(name:str, startLine:int, endLine:int, line:int) -> None:
        W._FUNCTIONS[name] = (startLine, endLine)
