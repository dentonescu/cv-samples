#ifndef DMOT_DATATYPES_H
#define DMOT_DATATYPES_H
#include <stdbool.h>

#ifndef bool
#define bool _Bool
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Safely narrows a `size_t` value to `int`.
     *
     * @param[in] value Value to convert.
     * @return Converted value when representable; otherwise `INT_MAX`.
     */
    int size_t_to_int(size_t value);

#ifdef __cplusplus
}
#endif
#endif // DMOT_DATATYPES_H
