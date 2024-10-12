# Constructed Types
> Constructed types are used to store multiple values. They are used to group data together. There are two types of constructed types: ***arrays*** and ***lists***.

## Arrays (Type safe)
> ***Arrays*** are used to store multiple values of the same type. They are type safe, meaning that all the values in the array must be of the same type. It is important to note that the size of the array is fixed and cannot be changed. Note that it is not possible to create an array of arrays.

| Expression | Description |
|---|---|
| `array int var size 10` | Create a new integer array variable `var` of size 10 |
| `var get index a` | Get the value at index `a` from the array variable `var` |
| `var set index a assign b` | Set the value at index `a` in the array variable `var` to `b` |
| `var append a` | Append `a` to the array variable `var` |
| `var remove a` | Remove the value at index `a` from the array variable `var` |
| `var length` | Get the length of the array variable `var` |
| `delete var` | Delete the array variable `var` |


## Lists (Not type safe)
> ***Lists*** are used to store multiple values of different types. They are not type safe, meaning that they can contain any type of data. You need to be careful when using lists as they can lead to type errors. ***Arrays*** are safer to use as they are type safe. The size of ***lists*** is dynamic and can change. Unlike ***arrays***, list of lists and list of arrays can be created.

| Expression | Description |
|---|---|
| `list var` | Create a new list variable `var` (not type safe, can contain any type of data) |
| `var get index a` | Get the value at index `a` from the list variable `var` |
| `var set index a assign b` | Set the value at index `a` in the list variable `var` to `b` |
| `var append a` | Append `a` to the list variable `var` (not type safe) |
| `var remove a` | Remove the value at index `a` from the list variable `var` |
| `var length` | Get the length of the list variable `var` |
| `delete var` | Delete the list variable `var` |

---

-> [Next: I/O Expressions](io_expressions.md)\
[Return to main page](README.md)