#include "command.h"

int8_t command_parse(char *line, command_t *cmd) {
    char *command = strtok(line, " ");
    char *endptr, *addr, *val;
    if (command == NULL) return -1;

    if (strcmp(command, "read") == 0) {
        cmd->command = CMD_READ;
    }
    else if (strcmp(command, "write") == 0) {
        cmd->command = CMD_WRITE;
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
            if (strtok(NULL, " ") != NULL) return -1;
            return 0;
        }
        val = strtok(NULL, " ");

        if (val == NULL) return -1;

        cmd->value = strtoul(val, &endptr, 16);
        if (endptr != val && *endptr == '\0' && (strtok(NULL, " ") == NULL)) return 0;
        return -1;
    }
    return -1;
}