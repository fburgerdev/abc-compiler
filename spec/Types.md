# Types

1.  _type-value_
2. _qualifier_  _type-value_
3. _value-category_ _type-value_
4. _qualifier_ _value-category_ _type-value_

## Type Values
### Primitives
A _primitive_ is a very basic type value and is the starting block of the abc type-system. Primitives include truth values, integers and decimals as well as characters characters.

#### Primitive: bool
Stores either _true_ or _false_ and has a (minimum) size of 1 byte.

#### Primitive: char
Stores an _UTF-8 character_ and has a (minimum) size of 1 byte. 

#### Primitive: int
Stores a _signed integer_ ranging from −2,147,483,648 to 2,147,483,647 and has a (minimum) size of 4 bytes.

#### Primitive: uint
Stores an _unsigned integer_ ranging from 0 to 4,294,967,295 and has a (minimum) size of 4 bytes.

#### Primitive: float
Stores a _floating-point_ value in [IEE754 binary32](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) format
and has a (minimum) size of 4 bytes.

### Classes
A _class-name_ can also be a _type-value_ (e.g. `Dog`, `Vec2`, `FileStream` etc.).

## Qualifiers
A _qualifier_ is one of the following:

### Qualifier: **mut**
The `mut` qualifier ensures that the given type is _mutable_,
i.e. that values of the type are allowed to change:
```
let myint :mut int= 0;
myint = 42; // value changes
```

We can also change member values:
```
let position :mut Vec2= Vec2(0, 0);
position.x = 0; // member value changes
```

The same rule applies for member functions:
```
class Vec2 {
    // this member function is not marked as const!
    func reset() {
        _x = 0;
        _y = 0;
    }

    x: float;
    y: float;
}

let position :const Vec2= Vec2(0, 0);

// so we can't call it from a const variable!
/* position.reset(); */
```

>A type is _mutable_ by default!

In most cases, the `mut` keyword is not necessary.
However in some cases, it is useful:
- to put emphasis on the fact that a variable is mutable
- to set the qualifier of a _generic_ to _mutable_

To understand why the `mut` keyword is necessary for generics, read [here](Generics.md).

### Qualifier: **const**
The `const` qualifier ensures that the given type is _constant_,
i.e. that values of the type are <u>not</u> allowed to change:
```
let myint :const int= 0;
// this won't compile since the variable is marked as const!
// myint = 42;
```

It is also forbidden to change member values:
```
let position :const Vec2= Vec2(0, 0);
// this won't compile since the variable is marked as const!
// position.x = 0;
```

TODO: member function overload with qualifier

## Value Categories
Similar to C++, _value-categories_ in abc are used for _move-semantics_.

If no _value-category_ is specified, values are passed in-place and if a _value-category_ is specified, values are passed as references:
```
func pass_in_place(x: int) {
    x = 0; // no effect outside of the function
}
func pass_as_ref(x: &int) {
    x = 0; // sets value of myint
}
func pass_as_ref(x: &&int) {
    x = 0; // sets value of myint
}

func main() {
    let myint := 42;
    pass_in_place(myint); // the value 42 is copied to x
    pass_as_ref(myint); // the address of myint is copied to x
    pass_as_ref(move(myint)); // the address of myint is copied to x
}
```

### lvalue: **&**
### rvalue: **&&**