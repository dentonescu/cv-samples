#ifndef DMOT_STRING_H
#define DMOT_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Writes a formatted into a string at the position indicated by cursor.
    // @param cursor                Pointer to the a pointer marking the position to start overwriting
    // @param end                   Marks the end of the string.
    // @param fmt                   String to insert which may containing format specifiers.
    // @param ...                   Variables which accompany the formatted string.
    // @return                      Integer representing the characters written or unable to be written. Positive means success; negative, failure.
    int dmot_string_write_into(char **cursor, char *end, const char *fmt, ...);

    // Removes whitespace from both sides of a string.
    // @param s         String to trim -- NOT a string literal but a writeable array.
    // @return          Trimmed string.
    char *dmot_string_trim(char *s);

#ifdef __cplusplus
}
#endif

#endif // DMOT_STRING_H