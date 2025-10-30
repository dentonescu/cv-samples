#include <stdio.h>
#include <stdlib.h>
#include "dmot/string.h"
#include "wfq/wifiequ.h"

/*
 * internals
 */

 // nothing yet


/*
 * externals
 */
void wfq_sample2json(wfq_sample *sample, char *buf, size_t buf_size)
{
    if (!buf || buf_size == 0)
        return;

    buf[0] = '\0';
    if (!sample)
        return;

    char *cursor = buf;
    char *end = buf + buf_size;

    if (!dmot_string_write_into(&cursor, end, "{\n"))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"timestamp_ms\": %lld,\n", sample->timestamp_ms))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"readings\": [\n"))
        return;

    bool first = true;
    for (size_t i = 0; i < WFQ_EQU_MAX_READINGS; ++i)
    {
        const wfq_channel *channel = &sample->readings[i];
        if (channel->chan_id <= 0)
            continue;

        if (!first && !dmot_string_write_into(&cursor, end, ",\n"))
            return;

        if (!dmot_string_write_into(&cursor, end,
                                    "        {\"chan\": %d, \"dbm\": %.1f}",
                                    channel->chan_id, channel->chan_dbm))
            return;

        first = false;
    }

    if (!dmot_string_write_into(&cursor, end, "\n    ]\n}\n"))
        return;
}