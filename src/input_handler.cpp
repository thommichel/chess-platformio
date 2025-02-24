#include <Arduino.h>
#include "input_handler.hpp"
#include "gantry.hpp"

namespace cmd {
    void handle_user_input(gnt::Gantry &gantry) {
        if(Serial.available()) {
            unsigned char buffer[MAX_CMD];
            int ret = parse_command(buffer);
            if(ret == CMD_ERROR) {
                return;
            }
            handle_command(buffer, gantry);
        }
    }

    int parse_command(unsigned char *cmd_buffer) {
        unsigned long cutoff = millis() + CMD_TIMEOUT;
        unsigned long current;
        for(int i = 0; i < MAX_CMD; ++i) {
            current = millis();
            while(!Serial.available() && current < cutoff){
                current = millis();
            }
            if(current >= cutoff) {
                Serial.write("Command timed out\n");
                return CMD_ERROR;
            }
            cmd_buffer[i] = Serial.read();
        }
        if(Serial.read() != 0x0A) {
            Serial.write("Incomplete command\n");
            return CMD_ERROR;
        }
        return 0;
    }

    void handle_command(unsigned char *cmd_buffer, gnt::Gantry &gantry) {
        switch(cmd_buffer[0]) {
            case CMD_MOVE_RELATIVE:
                float distance;
                bytes_to_float(&distance, cmd_buffer, 1);
                Serial.print("FLOAT RECEIVED: ");
                Serial.println(distance);
                if(cmd_buffer[1] == X_AXIS) {
                    gantry.get_x_axis().move_relative(distance);
                } else if(cmd_buffer[1] == Y_AXIS) {
                    gantry.get_y_axis().move_relative(distance);
                } else if(cmd_buffer[1] == Z_AXIS) {
                    gantry.get_z_axis().move_relative(distance);
                } else {
                    Serial.write("Invalid type\n");
                }
                Serial.write("OK");
                break;
            default:
                Serial.write("Invalid command\n");
        }
    }

    void bytes_to_float(float *f, unsigned char *cmd_buffer, int offset) {
        memcpy(f, cmd_buffer + offset, sizeof(float));
    }
    void float_to_bytes(float *f, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, f, sizeof(float));
    }
    void bytes_to_long(long *l, unsigned char *cmd_buffer, int offset) {
        memcpy(l, cmd_buffer + offset, sizeof(long));
    }
    void long_to_bytes(long *l, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, l, sizeof(long));
    }
    void bytes_to_int(int *l, unsigned char *cmd_buffer, int offset) {
        memcpy(l, cmd_buffer + offset, sizeof(int));
    }
    void int_to_bytes(int *l, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, l, sizeof(int));
    }
}
