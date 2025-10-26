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

    // Converts a size_t type to an int.
    // @param value         size_t value to be converted.
    // @return              Corresponding int value if within range, INT_MAX otherwise.
    int size_t_to_int(size_t value);

#ifdef __cplusplus
}
#endif
#endif // DMOT_DATATYPES_H