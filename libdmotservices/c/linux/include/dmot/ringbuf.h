#ifndef DMOT_RINGBUF_H
#define DMOT_RINGBUF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Generic ring buffer storing fixed-size units.
 *
 * Initialize with `dmot_ringbuf_init`, then use `dmot_ringbuf_read`/`dmot_ringbuf_write`
 * or typed wrappers. `count` tracks occupancy while `readCtr`/`writeCtr` are for stats.
 */
typedef struct
{
    void *pData;          // dynamically allocated storage area
    size_t unitSizeBytes; // size of each storage unit in bytes
    size_t maxSize;       // the maximum number of units which can be stored
    size_t count;         // number of units currently stored
    size_t readPos;       // read marker position
    uintmax_t readCtr;    // total number of reads already performed
    size_t writePos;      // write marker position
    uintmax_t writeCtr;   // total number of writes already performed
} ringbuf;

typedef enum
{
    RINGBUF_STATUS_OK = 0,
    RINGBUF_STATUS_ERROR = 10,
    RINGBUF_STATUS_NO_DATA = 20,
    RINGBUF_STATUS_FULL = 30
} RingBufferStatus;

/*
 * Usage (C11):
 *   ringbuf rb;
 *   dmot_ringbuf_init(&rb, 16, sizeof(int));
 *   dmot_ringbuf_write_value(&rb, 42);
 *   int out = 0;
 *   dmot_ringbuf_read_value(&rb, &out);
 */
bool dmot_ringbuf_init(ringbuf *rb, size_t maxSize, size_t unitSizeBytes);
bool dmot_ringbuf_destroy(ringbuf *rb);
RingBufferStatus dmot_ringbuf_check_status_read(ringbuf *rb);
RingBufferStatus dmot_ringbuf_check_status_write(ringbuf *rb);
RingBufferStatus dmot_ringbuf_read(ringbuf *rb, void *out);
RingBufferStatus dmot_ringbuf_write(ringbuf *rb, const void *in);
RingBufferStatus dmot_ringbuf_read_int(ringbuf *rb, int *out);
RingBufferStatus dmot_ringbuf_write_int(ringbuf *rb, int i);
RingBufferStatus dmot_ringbuf_read_char(ringbuf *rb, char *out);
RingBufferStatus dmot_ringbuf_write_char(ringbuf *rb, char c);
RingBufferStatus dmot_ringbuf_read_int8(ringbuf *rb, int8_t *out);
RingBufferStatus dmot_ringbuf_write_int8(ringbuf *rb, int8_t i);
RingBufferStatus dmot_ringbuf_read_uint8(ringbuf *rb, uint8_t *out);
RingBufferStatus dmot_ringbuf_write_uint8(ringbuf *rb, uint8_t i);
RingBufferStatus dmot_ringbuf_read_int16(ringbuf *rb, int16_t *out);
RingBufferStatus dmot_ringbuf_write_int16(ringbuf *rb, int16_t i);
RingBufferStatus dmot_ringbuf_read_uint16(ringbuf *rb, uint16_t *out);
RingBufferStatus dmot_ringbuf_write_uint16(ringbuf *rb, uint16_t i);
RingBufferStatus dmot_ringbuf_read_int32(ringbuf *rb, int32_t *out);
RingBufferStatus dmot_ringbuf_write_int32(ringbuf *rb, int32_t i);
RingBufferStatus dmot_ringbuf_read_uint32(ringbuf *rb, uint32_t *out);
RingBufferStatus dmot_ringbuf_write_uint32(ringbuf *rb, uint32_t i);
RingBufferStatus dmot_ringbuf_read_int64(ringbuf *rb, int64_t *out);
RingBufferStatus dmot_ringbuf_write_int64(ringbuf *rb, int64_t i);
RingBufferStatus dmot_ringbuf_read_uint64(ringbuf *rb, uint64_t *out);
RingBufferStatus dmot_ringbuf_write_uint64(ringbuf *rb, uint64_t i);
RingBufferStatus dmot_ringbuf_read_float(ringbuf *rb, float *out);
RingBufferStatus dmot_ringbuf_write_float(ringbuf *rb, float value);
RingBufferStatus dmot_ringbuf_read_double(ringbuf *rb, double *out);
RingBufferStatus dmot_ringbuf_write_double(ringbuf *rb, double value);

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
// Convenience helpers require C11 _Generic.
RingBufferStatus dmot_ringbuf_read_unsupported(ringbuf *rb, void *out);
RingBufferStatus dmot_ringbuf_write_unsupported(ringbuf *rb, ...);
#define dmot_ringbuf_read_value(rb, out) _Generic((out), \
    char *: dmot_ringbuf_read_char, \
    int8_t *: dmot_ringbuf_read_int8, \
    uint8_t *: dmot_ringbuf_read_uint8, \
    int16_t *: dmot_ringbuf_read_int16, \
    uint16_t *: dmot_ringbuf_read_uint16, \
    int32_t *: dmot_ringbuf_read_int32, \
    uint32_t *: dmot_ringbuf_read_uint32, \
    int64_t *: dmot_ringbuf_read_int64, \
    uint64_t *: dmot_ringbuf_read_uint64, \
    int *: dmot_ringbuf_read_int, \
    float *: dmot_ringbuf_read_float, \
    double *: dmot_ringbuf_read_double, \
    default: dmot_ringbuf_read_unsupported \
)(rb, (out))

#define dmot_ringbuf_write_value(rb, value) _Generic((value), \
    char: dmot_ringbuf_write_char, \
    int8_t: dmot_ringbuf_write_int8, \
    uint8_t: dmot_ringbuf_write_uint8, \
    int16_t: dmot_ringbuf_write_int16, \
    uint16_t: dmot_ringbuf_write_uint16, \
    int32_t: dmot_ringbuf_write_int32, \
    uint32_t: dmot_ringbuf_write_uint32, \
    int64_t: dmot_ringbuf_write_int64, \
    uint64_t: dmot_ringbuf_write_uint64, \
    int: dmot_ringbuf_write_int, \
    float: dmot_ringbuf_write_float, \
    double: dmot_ringbuf_write_double, \
    default: dmot_ringbuf_write_unsupported \
)(rb, (value))
#endif


#ifdef __cplusplus
}
#endif
#endif // DMOT_RINGBUF_H
