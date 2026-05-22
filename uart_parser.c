#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "uart_parser.h"

/*=========================================================
    PRIVATE FUNCTIONS
=========================================================*/

static void parser_reset(UARTParser *parser)
{
    parser->state = STATE_WAIT_SOF;

    parser->cmd = 0U;
    parser->len = 0U;

    parser->payload_index = 0U;

    parser->checksum = 0U;
    parser->calculated_checksum = 0U;
}

static void print_payload(
    const uint8_t *payload,
    uint8_t len
)
{
    uint8_t i;

    printf("[");

    for (i = 0U; i < len; i++)
    {
        printf("%02X", payload[i]);

        if (i < (len - 1U))
        {
            printf(" ");
        }
    }

    printf("]");
}

/*=========================================================
    PUBLIC FUNCTIONS
=========================================================*/

void parser_init(
    UARTParser *parser,
    uint32_t timeout_ms
)
{
    memset(parser, 0, sizeof(UARTParser));

    parser->timeout_ms = timeout_ms;

    parser_reset(parser);
}

int parser_feed_byte(
    UARTParser *parser,
    uint8_t byte,
    uint32_t timestamp_ms
)
{
    /*-----------------------------------------------------
        INTER-BYTE TIMEOUT CHECK
    -----------------------------------------------------*/

    if ((parser->timeout_ms > 0U) &&
        (parser->state != STATE_WAIT_SOF))
    {
        uint32_t delta =
            timestamp_ms - parser->last_timestamp;

        if (delta > parser->timeout_ms)
        {
            printf(
                "t=%3ums byte=0x%02X -> TIMEOUT (%ums gap > %ums) -- parser reset\n",
                timestamp_ms,
                byte,
                delta,
                parser->timeout_ms
            );

            parser_reset(parser);
        }
    }

    parser->last_timestamp = timestamp_ms;

    /*-----------------------------------------------------
        STATE MACHINE
    -----------------------------------------------------*/

    switch (parser->state)
    {
        case STATE_WAIT_SOF:

            if (byte == SOF)
            {
                parser->state = STATE_CMD;
            }

            break;

        case STATE_CMD:

            parser->cmd = byte;

            parser->calculated_checksum = byte;

            parser->state = STATE_LEN;

            break;

        case STATE_LEN:

            if (byte > MAX_PAYLOAD_LENGTH)
            {
                parser_reset(parser);

                return PARSER_CHECKSUM_ERROR;
            }

            parser->len = byte;

            parser->calculated_checksum ^= byte;

            if (parser->len == 0U)
            {
                parser->state = STATE_CHECKSUM;
            }
            else
            {
                parser->state = STATE_PAYLOAD;
            }

            break;

        case STATE_PAYLOAD:

            parser->payload[parser->payload_index] = byte;

            parser->calculated_checksum ^= byte;

            parser->payload_index++;

            if (parser->payload_index == parser->len)
            {
                parser->state = STATE_CHECKSUM;
            }

            break;

        case STATE_CHECKSUM:

            parser->checksum = byte;

            if (parser->checksum ==
                parser->calculated_checksum)
            {
                return PARSER_OK;
            }
            else
            {
                parser_reset(parser);

                return PARSER_CHECKSUM_ERROR;
            }

        default:

            parser_reset(parser);

            break;
    }

    return PARSER_IN_PROGRESS;
}

void feed_stream(
    UARTParser *parser,
    const uint8_t *bytes,
    const uint32_t *timestamps,
    uint32_t count
)
{
    uint32_t i;

    for (i = 0U; i < count; i++)
    {
        int result;

        result = parser_feed_byte(
            parser,
            bytes[i],
            timestamps[i]
        );

        if (result == PARSER_IN_PROGRESS)
        {
            printf(
                "t=%3ums byte=0x%02X -> receiving...\n",
                timestamps[i],
                bytes[i]
            );
        }
        else if (result == PARSER_OK)
        {
            printf(
                "t=%3ums byte=0x%02X -> FRAME OK CMD=0x%02X LEN=%u PAYLOAD=",
                timestamps[i],
                bytes[i],
                parser->cmd,
                parser->len
            );

            print_payload(
                parser->payload,
                parser->len
            );

            printf("\n");

            parser_reset(parser);
        }
        else if (result == PARSER_CHECKSUM_ERROR)
        {
            printf(
                "t=%3ums byte=0x%02X -> CHECKSUM ERROR\n",
                timestamps[i],
                bytes[i]
            );
        }
    }
}

/*=========================================================
    MAIN
=========================================================*/

int main(void)
{
    UARTParser parser;

    /*=====================================================
        TEST 1
    =====================================================*/

    printf(
        "\n================ TEST 1 ================\n"
    );

    parser_init(&parser, 50U);

    uint8_t stream1[] =
    {
        0xAA,
        0x01,
        0x03,
        0x10,
        0x20,
        0x30,
        0x02
    };

    uint32_t time1[] =
    {
        0U,
        5U,
        10U,
        15U,
        20U,
        25U,
        30U
    };

    feed_stream(
        &parser,
        stream1,
        time1,
        sizeof(stream1)
    );

    /*=====================================================
        TEST 2
    =====================================================*/

    printf(
        "\n================ TEST 2 ================\n"
    );

    parser_init(&parser, 50U);

    uint8_t stream2[] =
    {
        0xAA,
        0x01,
        0x03,
        0x10,
        0xAA,
        0x05,
        0x01,
        0x7F,
        0x7B
    };

    uint32_t time2[] =
    {
        0U,
        5U,
        10U,
        15U,
        200U,
        205U,
        210U,
        215U,
        220U
    };

    feed_stream(
        &parser,
        stream2,
        time2,
        sizeof(stream2)
    );

    /*=====================================================
        TEST 3
    =====================================================*/

    printf(
        "\n================ TEST 3 ================\n"
    );

    parser_init(&parser, 50U);

    uint8_t stream3[] =
    {
        0xAA,
        0x03,
        0x01,
        0x55,
        0x57,
        0xAA,
        0x04,
        0x02,
        0xAA,
        0xBB,
        0x17
    };

    uint32_t time3[] =
    {
        0U,
        5U,
        10U,
        15U,
        20U,
        25U,
        30U,
        35U,
        40U,
        45U,
        50U
    };

    feed_stream(
        &parser,
        stream3,
        time3,
        sizeof(stream3)
    );

    /*=====================================================
        TEST 4
    =====================================================*/

    printf(
        "\n================ TEST 4 ================\n"
    );

    parser_init(&parser, 0U);

    feed_stream(
        &parser,
        stream2,
        time2,
        sizeof(stream2)
    );

    return 0;
}