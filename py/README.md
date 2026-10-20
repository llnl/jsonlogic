## jsonlogic.py - JSON Logic expression generator

This package provides functionality to express JSON Logic using standard Python datastructures.

An example:

```python
>>> from jsonlogic import Variable
>>> v1 = Variable('var1')
>>> v2 = Variable('var2')
>>> e = (v1 < v2)
>>> print(e)
{"<": [{"var": "var1"}, {"var": "var2"}]}
>>> print (v1 < 3)
{"<": [{"var": "var1"}, 3]}
>>> print ( (v1 < 3) & (v1 > v2))
{"and": [{"<": [{"var": "var1"}, 3]}, {">": [{"var": "var1"}, {"var": "var2"}]}]}
>>> print ( (v1 < 3) & ~(v1 > v2)))  # ~ is "not"
{"and": [{"<": [{"var": "v1"}, 3]}, {"not": [{">": [{"var": "v1"}, {"var": "v2"}]}]}]}
```

### Conditionals

Because `if` is a reserved word in Python, JSON Logic's `if` is written as the function
`ifelse(condition, if_true, if_false)`:

```python
>>> from jsonlogic import Variable, ifelse
>>> v1 = Variable('var1')
>>> v2 = Variable('var2')
>>> print(ifelse(v1 < 3, "low", v2))
{"if": [{"<": [{"var": "var1"}, 3]}, "low", {"var": "var2"}]}
>>> print(ifelse(v1 < 3, "low", ifelse(v1 < 10, "medium", "high")))  # nesting
{"if": [{"<": [{"var": "var1"}, 3]}, "low", {"if": [{"<": [{"var": "var1"}, 10]}, "medium", "high"]}]}
```

The condition does not have to be a boolean. It is tested for
[truthiness as defined by JSON Logic](https://jsonlogic.com/truthy.html):

| Condition value | Treated as |
|---|---|
| `false`, `null` | false |
| `0` | false; any other number is true |
| `""` | false; any other string is true, including `"0"` and `"false"` |
| `[]` | false; any non-empty array is true |

This makes a fallback value straightforward:

```python
>>> name = Variable('name')
>>> print(ifelse(name, name, "anonymous"))  # like Python's `name or "anonymous"`
{"if": [{"var": "name"}, {"var": "name"}, "anonymous"]}
```
