# \_\_shared_ptr

1. \_\_shared_ptr<T>

- The `__shared_ptr` type store an instance of `T` in the [heap](../Heap.md).
- The lifetime of the instance is tied to the lifetime of <u>all</u> `__shared_ptr` referencing this memory.

## Member Functions
- init(args...), initializes instance of `T` using T(args...)
- init(const &\_\_shared_ptr\<T\>), references instance of existing `__shared_ptr`
- init(&&\_\_shared_ptr\<T\>), transfers reference of instance from existing `__shared_ptr` to the newly created one.
- deinit(), if holding a value: destructs instance of `T` and frees it's memory
- func get() mut -> mut T&
- func get() const -> const T&