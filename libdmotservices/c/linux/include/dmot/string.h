#ifndef DMOT_STRING_H
#define DMOT_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Removes whitespace from both sides of a string.
    // @param s         String to trim -- NOT a string literal but a writeable array.
    // @return          Trimmed string.
    char *dmot_string_trim(char *s);

#ifdef __cplusplus
}
#endif

#endif // DMOT_STRING_H