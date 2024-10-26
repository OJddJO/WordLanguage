> [!NOTE]
> Constructed types are used to store multiple values. They are used to group data together.

## Lists
> [!NOTE]
> ***Lists*** are used to store multiple values of different types. They are not type safe, meaning that they can contain any type of data. You need to be careful when using lists as they can lead to type errors. The size of ***lists*** is dynamic and can change.

| Expression | Description |
|---|---|
| `list var` | Create a new list variable `var` (not type safe, can contain any type of data) |
| `var get index a` | Get the value at index `a` from the list variable `var` |
| `var set index a assign b` | Set the value at index `a` in the list variable `var` to `b` |
| `var search a` | Search for the value `a` in the list variable `var` and return the index (-1 if not found) |
| `var append a` | Append `a` to the list variable `var` (not type safe) |
| `var remove a` | Remove the value at index `a` from the list variable `var` |
| `var length` | Get the length of the list variable `var` |
| `delete var` | Delete the list variable `var` |

## Dictionaries
> [!IMPORTANT]
> ***Not usable yet. Dicts are implemented but due to their complexities, they are not available for use yet.***
> [!NOTE]
> ***Dictionaries*** are used to store multiple values of different types. They are not type safe, meaning that they can contain any type of data. You need to be careful when using dictionaries as they can lead to type errors. The size of ***dictionaries*** is dynamic and can change.
| Expression | Description |
|---|---|
| *Coming soon...* | *Coming soon...* |

---
-> [Next: I/O Expressions](io_expressions.md)\
[Return to main page](README.md)