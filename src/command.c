#include "command.h"

int8_t command_parse(char *line, command_t *cmd) {
    char *command = strtok(line, " ");
    char *endptr, *addr, *val, *length;
    if (command == NULL) return -1;

    if (strcmp(command, "read") == 0) {
        cmd->command = CMD_READ;
    }
    else if (strcmp(command, "write") == 0) {
        cmd->command = CMD_WRITE;
    }
    else if (strcmp(command, "write_str") == 0) {
        cmd->command = CMD_WRITE_STR;
    }
    else {
        cmd->command = CMD_INVALID;
        return -1;
    }
    addr = strtok(NULL, " ");
    if (addr == NULL) return -1;

    cmd->address = strtoul(addr, &endptr, 16);

    if (endptr != addr && *endptr == '\0') {
        if (cmd->command == CMD_READ) {
            length = strtok(NULL, " ");
            if (length == NULL) return -1;
            cmd->length = strtoul(length, &endptr, 10);

            if (endptr == length || *endptr != '\0') return -1;

            if (strtok(NULL, " ") != NULL) return -1;

            return 0;
        }
        else if (cmd->command == CMD_WRITE) {
            cmd->length = 0;
            val = strtok(NULL, " ");
            if (val == NULL) return -1;
            cmd->value_arr[0] = strtoul(val, &endptr, 10);
            if (endptr == val || *endptr != '\0') return -1;
            cmd->length++;

            for (int i = 1; i < COMMAND_MAX_DATA; i++) {
                val = strtok(NULL, " ");
                if (val == NULL) return 0;
                cmd->value_arr[i] = strtoul(val, &endptr, 10);
                if (endptr == val || *endptr != '\0') return -1;
                cmd->length++;
            }
            return -1;
        }
        else {
            cmd->length = 0;
            char *str = strtok(NULL, "");

            if (str == NULL) return -1;

            while (*str == ' ') str++;
            if (*str == '\0') return -1;
            
            while (*str != '\0') {
                if (cmd->length >= COMMAND_MAX_DATA) return -1;
                cmd->value_arr[cmd->length++] = (uint8_t)*str++;
            }
            return 0;
            }

    }
    return -1;
}