#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t time; /* 16 bit float */
    uint8_t subcommand_code; /* 8 bit unsigned int */
    uint8_t opcode; /* 8 bit unsigned int */
    uint8_t sequence_num; /* 8 bit unsigned int */
    uint32_t crc; /* 32 bit unsigned int */
} SEQUENCE_NODE;