#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP
namespace cmd {
    const int CMD_TIMEOUT = 3000;
    const int MAX_CMD = 50;
    const int CMD_ERROR = 1;
    /*
    Parse and execute the user's given command.
    */
    void handle_user_input();

    /*
    Parse the command given by the user over Serial.
    */
    int parse_command(char *cmd_buffer);

    /*
    Based on the received command in the buffer, perform the desired action.
    */
    void handle_command(char *cmd_buffer);
}
#endif
