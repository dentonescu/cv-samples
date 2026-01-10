#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmot/ringbuf.h"

bool dmot_ringbuf_init(ringbuf *rb, size_t maxSize, size_t unitSizeBytes)
{
    if (!rb)
    {
        return false;
    }
    if (maxSize == 0 || unitSizeBytes == 0)
    {
        return false;
    }
    if (maxSize > (SIZE_MAX / unitSizeBytes))
    {
        return false;
    }
    rb->maxSize = maxSize;
    rb->unitSizeBytes = unitSizeBytes;
    rb->pData = calloc(rb->maxSize, rb->unitSizeBytes);
    rb->count = 0;
    rb->readPos = 0;
    rb->readCtr = 0;
    rb->writePos = 0;
    rb->writeCtr = 0;
    if (!rb->pData)
    {
        return false;
    }
    return true;
}

bool dmot_ringbuf_destroy(ringbuf *rb)
{
    if (!rb)
    {
        return false;
    }
    free(rb->pData);
    rb->pData = NULL;
    rb->maxSize = 0;
    rb->unitSizeBytes = 0;
    rb->count = 0;
    rb->readPos = 0;
    rb->writePos = 0;
    rb->readCtr = 0;
    rb->writeCtr = 0;
    return true;
}

RingBufferStatus dmot_ringbuf_check_status_read(ringbuf *rb)
{
    if (!rb || !rb->pData || rb->maxSize == 0 || rb->unitSizeBytes == 0)
    {
        return RINGBUF_STATUS_ERROR;
    }
    if (rb->count == 0)
    {
        return RINGBUF_STATUS_NO_DATA;
    }
    return RINGBUF_STATUS_OK;
}

RingBufferStatus dmot_ringbuf_check_status_write(ringbuf *rb)
{
    if (!rb || !rb->pData || rb->maxSize == 0 || rb->unitSizeBytes == 0)
    {
        return RINGBUF_STATUS_ERROR;
    }
    if (rb->count >= rb->maxSize)
    {
        return RINGBUF_STATUS_FULL;
    }
    return RINGBUF_STATUS_OK;
}

RingBufferStatus dmot_ringbuf_read(ringbuf *rb, void *out)
{
    RingBufferStatus status = dmot_ringbuf_check_status_read(rb);
    if (status != RINGBUF_STATUS_OK)
    {
        return status;
    }
    if (!rb || !rb->pData || !out || rb->unitSizeBytes == 0)
    {
        return RINGBUF_STATUS_ERROR;
    }
    if (rb->readPos >= rb->maxSize)
    {
        rb->readPos = 0;
    }
    memcpy(out,
           ((uint8_t *)rb->pData) + (rb->readPos * rb->unitSizeBytes),
           rb->unitSizeBytes);
    ++rb->readPos;
    ++rb->readCtr;
    --rb->count;
    if (rb->readPos >= rb->maxSize)
    {
        rb->readPos = 0;
    }
    return RINGBUF_STATUS_OK;
}

RingBufferStatus dmot_ringbuf_write(ringbuf *rb, const void *in)
{
    RingBufferStatus status = dmot_ringbuf_check_status_write(rb);
    if (status != RINGBUF_STATUS_OK)
    {
        return status;
    }
    if (!rb || !rb->pData || !in || rb->unitSizeBytes == 0)
    {
        return RINGBUF_STATUS_ERROR;
    }
    if (rb->writePos >= rb->maxSize)
    {
        rb->writePos = 0;
    }
    memcpy(((uint8_t *)rb->pData) + (rb->writePos * rb->unitSizeBytes),
           in,
           rb->unitSizeBytes);
    ++rb->writePos;
    ++rb->writeCtr;
    ++rb->count;
    if (rb->writePos >= rb->maxSize)
    {
        rb->writePos = 0;
    }
    return RINGBUF_STATUS_OK;
}

RingBufferStatus dmot_ringbuf_read_int(ringbuf *rb, int *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(int))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_int(ringbuf *rb, int i)
{
    if (!rb || rb->unitSizeBytes != sizeof(int))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_char(ringbuf *rb, char *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(char))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_char(ringbuf *rb, char c)
{
    if (!rb || rb->unitSizeBytes != sizeof(char))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &c);
}

RingBufferStatus dmot_ringbuf_read_int8(ringbuf *rb, int8_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(int8_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_int8(ringbuf *rb, int8_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(int8_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_uint8(ringbuf *rb, uint8_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint8_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_uint8(ringbuf *rb, uint8_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint8_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_int16(ringbuf *rb, int16_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(int16_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_int16(ringbuf *rb, int16_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(int16_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_uint16(ringbuf *rb, uint16_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint16_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_uint16(ringbuf *rb, uint16_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint16_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_int32(ringbuf *rb, int32_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(int32_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_int32(ringbuf *rb, int32_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(int32_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_uint32(ringbuf *rb, uint32_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint32_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_uint32(ringbuf *rb, uint32_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint32_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_int64(ringbuf *rb, int64_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(int64_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_int64(ringbuf *rb, int64_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(int64_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_uint64(ringbuf *rb, uint64_t *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint64_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_uint64(ringbuf *rb, uint64_t i)
{
    if (!rb || rb->unitSizeBytes != sizeof(uint64_t))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &i);
}

RingBufferStatus dmot_ringbuf_read_float(ringbuf *rb, float *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(float))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_float(ringbuf *rb, float value)
{
    if (!rb || rb->unitSizeBytes != sizeof(float))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &value);
}

RingBufferStatus dmot_ringbuf_read_double(ringbuf *rb, double *out)
{
    if (!rb || rb->unitSizeBytes != sizeof(double))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_read(rb, out);
}

RingBufferStatus dmot_ringbuf_write_double(ringbuf *rb, double value)
{
    if (!rb || rb->unitSizeBytes != sizeof(double))
    {
        return RINGBUF_STATUS_ERROR;
    }
    return dmot_ringbuf_write(rb, &value);
}

RingBufferStatus dmot_ringbuf_read_unsupported(ringbuf *rb, void *out)
{
    (void)rb;
    (void)out;
    return RINGBUF_STATUS_ERROR;
}

RingBufferStatus dmot_ringbuf_write_unsupported(ringbuf *rb, ...)
{
    (void)rb;
    return RINGBUF_STATUS_ERROR;
}
