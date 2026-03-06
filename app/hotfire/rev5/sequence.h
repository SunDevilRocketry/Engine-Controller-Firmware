#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t time;
    uint8_t subcommand_code;
    uint8_t opcode;
    uint8_t sequence_num;
    uint32_t crc;
} SEQUENCE_NODE;