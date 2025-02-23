#include <Arduino.h>
#include "input_handler.hpp"

namespace cmd {
    void handle_user_input() {
        if(Serial.available()) {
        char buffer[MAX_CMD];
        int ret = parse_command(buffer);
        if(ret == CMD_ERROR) {
            return;
        }
        handle_command(buffer);
        }
    }

    int parse_command(char *cmd_buffer) {
        unsigned long cutoff = millis() + CMD_TIMEOUT;
        char b = Serial.read();
        if(b != '<') {
            return CMD_ERROR;
        }
        unsigned long current;
        bool complete_command = false;
        for(int i = 0; i < MAX_CMD; ++i) {
            current = millis();
            while(!Serial.available() && current < cutoff){
            current = millis();
            }
            if(current >= cutoff) {
            Serial.write("Command timed out\n");
            return CMD_ERROR;
            }
            b = Serial.read();
            if(b == '>') {
            cmd_buffer[i] = '\0';
            complete_command = true;
            break;
            }
            cmd_buffer[i] = b;
        }
        if(!complete_command) {
            Serial.write("Incomplete command\n");
            return CMD_ERROR;
        }
        return 0;
    }

    void handle_command(char *cmd_buffer) {
        char *res;
        res = strtok(cmd_buffer, " ");
        if(strcmp(res, "cmd_name_goes_here") == 0){
            res = strtok(NULL, " ");
            if(strcmp(res, "2nd arg of command") == 0) {
            digitalWrite(1, HIGH);
            Serial.write("OK\n");
            }
            else if(strcmp(res, "2nd arg of command") == 0) {
            digitalWrite(1, LOW);
            Serial.write("OK\n");
            }
            else{
            Serial.write("Invalid use of command\n");
            Serial.write(cmd_buffer, 50);
            }
        } else {
            Serial.write("Invalid command\n");
        }
    }
}
