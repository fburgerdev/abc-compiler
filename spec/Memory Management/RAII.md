# RAII

The abc language follows the RAII (Resource Acquisition Is Initialisation) concept.

Every variable (primitive or class type) is always initialised when it’s introduced.

The language prevents either with syntax
```
// this won't compile
let myint :int; // invalid syntax
```
or with semantics analysis
```
class Vec2 {
    init() {
        // this won't compile
        std::print(_x); // x is not initialised!
    }

    x: int;
    y: int;
}
```
that a variable is anywhere accessible without being initialised.

## Manual Lifetime
The only exception to this rule is the \_\_manual<T> type, see [\_\_manual](RAII/\_\_manual.md)

>The usage of this type is highly discouraged since it violates the RAII concept!