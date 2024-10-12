# Operations
> [!NOTE]
> ***Operations*** are the basic building blocks of the language. They are the smallest unit of code that can be executed. They can be combined to form ***expressions*** which can be used to perform more complex operations.

> [!IMPORTANT]
> ***Operations*** does not follow the mathematical order of operations. They are executed from left to right. To enforce a specific order of operations, use `.`. For example, `2 plus 3 times 4` will be evaluated as `2 + 3 = 5` then `5 * 4 = 20`. To evaluate `3 * 5` first, use `2 plus 3 .times 4`.

| Expression | Description |
|---|---|
| `a plus b` | Add `a` and `b` |
| `a minus b` | Subtract `b` from `a` |
| `a times b` | Multiply `a` and `b` |
| `a div b` | Divide `a` by `b` |
| `a mod b` | Remainder of `a` divided by `b` |
| `a ediv b` | Euclidean division of `a` by `b` |
| `a power b` | `a` raised to the power of `b` |
| `a root b` | `a`th root of `b` |
| `a plus b .times c` | `a` times `b * c` (priority) |

---

-> [Next: Variables](variables.md)\
[Return to main page](README.md)