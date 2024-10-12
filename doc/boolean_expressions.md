# Boolean expressions
> [!NOTE]
> ***Boolean expressions*** are used to evaluate conditions. They return a ***boolean value*** (`true` or `false`).

## Logic
> [!NOTE]
> Logic operations are used to evaluate ***boolean expressions***. They are used to make decisions in the code. You can use `and`, `or`, `not` to combine ***boolean expressions***. You can also use `.` to specify the priority of the operations. The more `.` you use, the higher the priority.

| Expression | Description |
|---|---|
| `a and b` | `a` and `b` |
| `a or b` | `a` or `b` |
| `not a` | not `a` |
| `a .or b and c` | (`a` or `b`) and `c` (priority)|

## Comparison
> [!NOTE]
> Comparison operations are used to compare values. They return a boolean value (`true` or `false`). You can compare integers, floats, strings, and boolean values. You can use `not` to negate the comparison.

| Expression | Description |
|---|---|
| `a equal b` | Returns true if `a` is equal to `b`, else false |
| `a greater b` | Returns true if `a` is greater than `b`, else false |
| `a less b` | Returns true if `a` is less than `b`, else false |
| `a gEqual b` | Returns true if `a` is greater than or equal to `b`, else false |
| `a lEqual b` | Returns true if `a` is less than or equal to `b`, else false |
| `.not a ..equal b and b equal c` | Returns true if (`a` is not equal to `b`) and `b` is equal to `c`, else false (priority) |

---

-> [Next: Control Flow](control_flow.md)\
[Return to main page](README.md)