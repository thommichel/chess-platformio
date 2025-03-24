#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP
#define HELLO 5
#include "gantry.hpp"

// Command settings
#define CMD_TIMEOUT 3000
#define MAX_CMD 15
namespace cmd {

    class InputHandler {
        public:
            InputHandler();
            InputHandler(gnt::Gantry& gantry);
    
            /*
            Parse and execute the user's given command.
            */
            void handle_user_input();
        
        private:
            gnt::Gantry m_gantry;
            enum : uint8_t {
                // Command error codes
                RET_OK = 0,
                RET_TIMEOUT = 1,
                RET_UNRECOGNIZED = 2,
                RET_INVALID_PARAM = 3,
                RET_ERROR = 4,
                RET_ENDLINE = 0x0A,

                // Command Types
                GANTRY = 0,
                X_AXIS = 1,
                Y_AXIS = 2,
                Z_AXIS = 3,

                // 2 parameter commands
                CMD_HOME = 0,
                CMD_STOP = 1,
                CMD_IS_HOMING = 2,
                CMD_IS_MOVING = 3,
                CMD_GET_POSN = 4,
                CMD_GET_SPEED = 5,
                CMD_GET_DEFAULT_SPEED = 6,
                CMD_GET_SLOW_SPEED = 7,
                CMD_GET_FINE_SPEED = 8,
                CMD_GET_MAX_SPEED = 9,
                CMD_GET_ACCEL = 10,
                CMD_GET_LIMIT = 11,
                CMD_GET_DIST_TO_GO = 12,
                CMD_GET_TARGET_POSN = 13,
                CUTTOFF_2_PARAM = 32,

                // 3 parameter commands
                CMD_MOVE_ABSOLUTE = 32,
                CMD_MOVE_RELATIVE = 33,
                CMD_SET_POSN = 34,
                CMD_SET_MAX_SPEED = 35,
                CMD_SET_DEFAULT_SPEED = 36,
                CMD_SET_SLOW_SPEED = 37,
                CMD_SET_FINE_SPEED = 38,
                CMD_SET_ACCEL = 39,
                CMD_ENABLE_MAG = 40,
                CUTTOFF_3_PARAM = 64,

                // 4 parameter commands
                CMD_MOVE_XZ = 64,
                CUTTOFF_4_PARAM = 96,

                // 5 parameter commands
                CMD_MOVE_XYZ = 96,
                CUTTOFF_5_PARAM = 128
            };
            /*
            Parse the command given by the user over Serial.
            */
            int parse_command(unsigned char *cmd_buffer);

            /*
            Based on the received command in the buffer, perform the desired action.
            */
            void handle_command(unsigned char *cmd_buffer);

            void send_return_value(uint8_t ret);
            void send_return_value(uint8_t ret, uint8_t val);
            void send_return_values(uint8_t ret, unsigned char *cmd_buffer);

            void bytes_to_float(float *f, unsigned char *cmd_buffer, int offset);
            void float_to_bytes(float *f, unsigned char *cmd_buffer, int offset);
            void bytes_to_long(long *l, unsigned char *cmd_buffer, int offset);
            void long_to_bytes(long *l, unsigned char *cmd_buffer, int offset);
            void bytes_to_int(int *l, unsigned char *cmd_buffer, int offset);
            void int_to_bytes(int *l, unsigned char *cmd_buffer, int offset);

            void cmd_home(unsigned char *cmd_buffer);
            void cmd_stop(unsigned char *cmd_buffer);
            void cmd_is_homing(unsigned char *cmd_buffer);
            void cmd_is_moving(unsigned char *cmd_buffer);
            void cmd_get_posn(unsigned char *cmd_buffer);
            void cmd_get_speed(unsigned char *cmd_buffer);
            void cmd_get_default_speed(unsigned char *cmd_buffer);
            void cmd_get_slow_speed(unsigned char *cmd_buffer);
            void cmd_get_fine_speed(unsigned char *cmd_buffer);
            void cmd_get_max_speed(unsigned char *cmd_buffer);
            void cmd_get_accel(unsigned char *cmd_buffer);
            void cmd_get_limit(unsigned char *cmd_buffer);
            void cmd_get_dist_to_go(unsigned char *cmd_buffer);
            void cmd_get_target_posn(unsigned char *cmd_buffer);
            void cmd_move_absolute(unsigned char *cmd_buffer);
            void cmd_move_relative(unsigned char *cmd_buffer);
            void cmd_set_posn(unsigned char *cmd_buffer);
            void cmd_set_max_speed(unsigned char *cmd_buffer);
            void cmd_set_default_speed(unsigned char *cmd_buffer);
            void cmd_set_slow_speed(unsigned char *cmd_buffer);
            void cmd_set_fine_speed(unsigned char *cmd_buffer);
            void cmd_set_accel(unsigned char *cmd_buffer);
            void cmd_enable_mag(unsigned char *cmd_buffer);
            void cmd_move_xz(unsigned char *cmd_buffer);
            void cmd_move_xyz(unsigned char *cmd_buffer);
    };
}

#endif
