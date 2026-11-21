#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_MAX_DATA 64

typedef enum {
    CMD_INVALID,
    CMD_READ,
    CMD_WRITE,
    CMD_WRITE_STR
} command_type_t;

typedef struct {
    command_type_t command;
    uint16_t address;
    uint8_t length;
    uint8_t value_arr[COMMAND_MAX_DATA];
} command_t;

int8_t command_parse(char *line, command_t *cmd);

#endif