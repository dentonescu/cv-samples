#ifndef DMOT_STRING_H
#define DMOT_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Writes formatted text into a string buffer at the cursor location.
     *
     * Typical setup prior to calling this function:
     * @code{.c}
     * char *cursor = buf;
     * char *end = buf + buf_size;
     * @endcode
     *
     * @param[in,out] cursor Pointer to the buffer cursor; advanced as characters are emitted.
     * @param[in] end Pointer one past the writable end of the buffer.
     * @param[in] fmt printf-style format string.
     * @param[in] ... Variadic arguments consumed by the format string.
     * @return Number of characters appended on success; negative on failure.
     */
    int dmot_string_write_into(char **cursor, char *end, const char *fmt, ...);

    /**
     * @brief Trims leading and trailing whitespace from a mutable string.
     *
     * @param[in,out] s Writable string buffer to trim in place.
     * @return Pointer to the trimmed portion of @p s.
     */
    char *dmot_string_trim(char *s);

#ifdef __cplusplus
}
#endif

#endif // DMOT_STRING_H
