Requires a CMake library to be defined with the name `emblib::math-impl`.

Header file called `matrix_native.hpp` should provide a definition for
```cpp
template <typename scalar_type, size_t ROWS, size_t COLS>
emblib::math::details::matrix_native_t
```
Header file `matrix_impl.hpp` should provide inline method definitions for the matrix class.