# RAII

The abc language follows the RAII (Resource Acquisition Is Initialisation) concept.

Every variable (primitive or class type) is always initialised when it’s introduced.

# Manual Lifetime

The only exception to this rule is the \_\_manual<T> type, see

[\_\_manual](RAII/\_\_manual.md)

<aside>
<img src="https://www.notion.so/icons/exclamation-mark_red.svg" alt="https://www.notion.so/icons/exclamation-mark_red.svg" width="40px" />

the usage of this type is highly discouraged since it violates the RAII concept

</aside>