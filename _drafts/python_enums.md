---
layout: post
title: Implementing enums in python
---

## Background

In Python, everything is an object. An instance of a class is an object, but the class itself is also an object.

A metaclass creates classes. A class creates instances. You can inspect with `type`:

```
class Meta(type): pass
class Foo(metaclass=Meta): pass
>>> foo = Foo()
>>> type(foo)
<class 'Foo'>
>>> type(Foo)
<class 'Meta'>
>>> type(Meta)
<class 'type'>
```

Python attributes can exist in multiple places. There's a dictionary for the class object and there's a dictionary for the instance object.

Look up of attributes on the instance follows these rules

1. Class descriptors has highest precedence. 
2. instance dicts comes next
3. Then instance descriptors
4. and last class dicts

Lookup on class attributes

Python provides built-in support for representing numbers, sequences and strings.

Aggregation is supported through tuples and (lately) dataclasses.

We can create our own abstractions using functions and classes.

A lot of things in Python is built up around dictionaries.

A type always represent a set of values. It's common that we want to refer to a set of integers.

What is a type system?

Why do we need enums?

## Better printing of enum class

For printing classes you need a metaclass.

```
class EnumMeta(type):
    def __repr__(cls):
        return "<enum %r>" % cls.__name__

class Enum(metaclass=EnumMeta):
    A = 1
    B = 2
    C = 3
>>>Enum
<enum 'Enum'>
```

## Filter out members

## Print members

If we try to create a class constant it will print the value, but if we print an enum enumerator it will print the name and the value.

```
class Enum:
	A = 1
	B = 2
>>> Enum.A
1
class Enum(enum.Enum):
	A = 1
	B = 2
>>> Enum.A
<Enum.A: 1>
```

## Create Constructor

The constructor for an enum class should create an instance assigned to one of the enumerators.

```
class Enum(enum.Enum):
	A = 1
	B = 2
>>> e = Enum()
__call__() missing 1 required positional argument: 'value'
>>> e = Enum(Enum.A)
>>> e
<Enum.A: 1>
>>> e = Enum(1)
>> e
<Enum.A: 1>
>>
```

