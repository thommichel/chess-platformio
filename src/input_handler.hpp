#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "gantry.hpp"
namespace cmd {
    // Command settings
    const int CMD_TIMEOUT = 3000;
    const int MAX_CMD = 10;
    const int CMD_ERROR = 1;

    // Command Types
    const uint8_t GANTRY = 0;
    const uint8_t X_AXIS = 1;
    const uint8_t Y_AXIS = 2;
    const uint8_t Z_AXIS = 3;

    // Commands
    const uint8_t CMD_MOVE_RELATIVE = 0;
    const uint8_t CMD_HOME = 1;
    /*
    Parse and execute the user's given command.
    */
    void handle_user_input(gnt::Gantry &gantry);

    /*
    Parse the command given by the user over Serial.
    */
    int parse_command(unsigned char *cmd_buffer);

    /*
    Based on the received command in the buffer, perform the desired action.
    */
    void handle_command(unsigned char *cmd_buffer, gnt::Gantry &gantry);

    void bytes_to_float(float *f, unsigned char *cmd_buffer, int offset);
    void float_to_bytes(float *f, unsigned char *cmd_buffer, int offset);
    void bytes_to_long(long *l, unsigned char *cmd_buffer, int offset);
    void long_to_bytes(long *l, unsigned char *cmd_buffer, int offset);
    void bytes_to_int(int *l, unsigned char *cmd_buffer, int offset);
    void int_to_bytes(int *l, unsigned char *cmd_buffer, int offset);
}

#endif
