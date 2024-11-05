# Definitions

[Variables](Definitions/Variables.md)

[Functions](Definitions/Functions.md)

[Classes](Definitions/Classes.md)

[Groups](Definitions/Groups.md)

# Access

1. _identifier (single)_
2. _identifier_**::**_…_**::**_identifier (nested)_

_Definitions_ can be accessed via _single-_ or _nested-identifiers_.

The preceding names in a _nested-identifier_ specify the context of the _definition_ and the last name specifies the name of the accessed _definition._


group A {
  group C {
    func test() {
      std::print("called by foo!");
    }
  }
  
  group B {
    group C {
      func test() {
        std::print("called by bar!");
      }
    }
    
    func foo() {
      A::C::test();
    }
    func bar() {
      C::test();
    }
  }
}
