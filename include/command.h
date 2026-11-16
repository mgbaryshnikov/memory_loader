#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CMD_INVALID,
    CMD_READ,
    CMD_WRITE
} command_type_t;

typedef struct {
    command_type_t command;
    uint16_t address;
    uint8_t value;
} command_t;

int8_t command_parse(char *line, command_t *cmd);

#endif