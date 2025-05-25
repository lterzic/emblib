#pragma once

// Allows for using all emblib definitions from the emblib namespace
// instead of nesting them (emblib::mutex instead of emblib::rtos::mutex, etc)
#define EMBLIB_UNNEST_NAMESPACES    1

// Select the RTOS backend
#define EMBLIB_RTOS_USE_FREERTOS    1
#define EMBLIB_RTOS_USE_THREADX     0

// Select the math backend
#define EMBLIB_MATH_USE_GLM         0
#define EMBLIB_MATH_USE_EIGEN       1
