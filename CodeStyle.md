## Naming and notation

### Classes
Classes names are lowercase with underscores (`_`) separating words. Member variables are prefixed with `m_` and static member variables are prefixed with `s_`.

Methods are listed in the order: public, protected, private.
Methods should start with a verb, for example `int get_count()` instead of `int count()` or `bool is_ready()` instead of `bool ready()`. Static variables inside methods and functions are not prefixed with `s_`.

```cpp
class i2c_master {
    ...
}
```

### Structs
Structs are named similarly to classes, but with trailing `_s`. Struct members should not be prefixed and visibility inside a struct must not be modified (all members should be public).

If methods inside a struct are needed, they must be `const`.

```cpp
struct i2c_device_desc_s {
    ...
}
```

### Constants
All uppercase with underscores separating words.

Use `constexpr` whenever possible, `#define` for creating constants should only be used for boolean type constants:

```cpp
#define USE_MUTEX_INSIDE_LOGGER     1

constexpr size_t OUT_PORT_BUFFER_SIZE = 12;
```

### Enums
Enum classes should have the `_e` suffix. Members should follow rules for constants.

If using C-style enums (not enum classes), all members should be prefixed by the enum class name, excluding the `_e` suffix.

```cpp
enum class colors_e {
    RED,
    GREEN,
    BLUE
};

enum colors_e {
    COLORS_RED,
    COLORS_GREEN,
    COLORS_BLUE
};
```

### Unions
Suffix `_u`. Don't use classes inside unions, only structs and primitive types.

### Template types
Template typenames should have trailing `_type`. When using template literals, they should follow rules for constants.

### Typedefs and using
Suffix `_t`.

### API naming - `read`/`write` vs `get`/`set`
Functions (methods) which are used to communicate data, for example an I2C transaction or a GPIO pin level, should use the `read`/`write` naming convention, where as methods which are used for configuration should use `get`/`set` prefixes.
