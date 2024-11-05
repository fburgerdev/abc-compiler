# \_\_unique_ptr

1. \_\_unique_ptr\<T\>

- The `__unique_ptr` type store an instance of `T` in the [heap](../Heap.md).
- The lifetime of the value is tied to the lifetime of the variable.

## Member Functions
- init(args...), initializes instance of `T` using T(args...)
- init(&&\_\_unique_ptr\<T\>), transfers ownership of heap memory to _self_
- deinit(), if holding a value: destructs instance of `T` and frees it's memory
- func get() mut -> mut T&
- func get() const -> const T&