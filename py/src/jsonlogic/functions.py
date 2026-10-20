""" Free functions for JSONLogic operations that can't be expressed as operators or methods. """

from __future__ import annotations

from . import Operand, Literal, Expression
from .operations import jl_if
from .jsontypes import PyJsonType


def _as_operand(o: Operand | PyJsonType) -> Operand:
    return o if isinstance(o, Operand) else Literal(o)


def ifelse(
    cond: Operand | PyJsonType,
    if_true: Operand | PyJsonType,
    if_false: Operand | PyJsonType,
) -> Expression:
    """JSONLogic `if`: evaluates to `if_true` when `cond` is truthy, otherwise `if_false`.

    `if` is a reserved word in Python, hence the name. `cond` need not be a boolean: it is
    tested for truthiness as defined by JSONLogic (https://jsonlogic.com/truthy.html).
    `false`, `null`, `0`, `""` and `[]` are falsy; every other value is truthy, including
    the strings `"0"` and `"false"`. Note that unlike Python, an empty array is falsy.

    Any argument may be an `Operand` (`Variable`, `Literal`, `Expression`) or a plain
    JSON-compatible Python value, which is converted to a `Literal`.

    >>> v = Variable("name")
    >>> print(ifelse(v, v, "anonymous"))
    {"if": [{"var": "name"}, {"var": "name"}, "anonymous"]}
    """
    return Expression(jl_if, _as_operand(cond), if_true, if_false)


__all__ = ["ifelse"]
