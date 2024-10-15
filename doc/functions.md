# Functions
> [!NOTE]
> Functions are used to group code into reusable blocks. They can take arguments and return values. You can use `def`, `return`, `enddef` to define functions.

| Expression | Description |
|---|---|
| `def null funcName do` | Define a function `funcName` that returns nothing |
| `def int funcName with bool a int b do` | Define a function `funcName` with arguments `a` (boolean) and `b` (integer) that returns an integer |
| `return a` | Return `a` from the function |
| `return a plus b` | Return `a + b` from the function |
| `enddef` | End function |
| `call funcName` | Call the function `funcName` |
| `call funcName with true 5` | Call the function `funcName` with arguments `true` and `5` |
| `call funcName store varName` | Call the function `funcName` and store the return value in `varName` |
| `call funcName with true 5 store varName` | Call the function `funcName` with arguments `true` and `5` and store the return value in `varName` |

---

-> [Return to main page](README.md)