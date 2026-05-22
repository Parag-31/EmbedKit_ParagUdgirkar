#ifndef UART_PARSER_H
#define UART_PARSER_H

#include <stdint.h>

#define SOF                     0xAA
#define MAX_PAYLOAD_LENGTH      16

#define PARSER_OK               1
#define PARSER_IN_PROGRESS      0
#define PARSER_CHECKSUM_ERROR  -1
#define PARSER_TIMEOUT         -2

typedef enum
{
    STATE_WAIT_SOF = 0,
    STATE_CMD,
    STATE_LEN,
    STATE_PAYLOAD,
    STATE_CHECKSUM

} ParserState;

typedef struct
{
    ParserState state;

    uint8_t cmd;
    uint8_t len;
    uint8_t payload[MAX_PAYLOAD_LENGTH];

    uint8_t checksum;
    uint8_t calculated_checksum;

    uint8_t payload_index;

    uint32_t last_timestamp;
    uint32_t timeout_ms;

} UARTParser;

void parser_init(UARTParser *parser, uint32_t timeout_ms);

int parser_feed_byte(
    UARTParser *parser,
    uint8_t byte,
    uint32_t timestamp_ms
);

void feed_stream(
    UARTParser *parser,
    const uint8_t *bytes,
    const uint32_t *timestamps,
    uint32_t count
);

#endif