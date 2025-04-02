#include <Arduino.h>
#include "input_handler.hpp"
#include "gantry.hpp"

namespace cmd {
    
    InputHandler::InputHandler() {}
    InputHandler::InputHandler(gnt::Gantry* gantry) {
        m_gantry = gantry;
    }
    void InputHandler::handle_user_input() {
        if(Serial.available()) {
            unsigned char buffer[MAX_CMD];
            int ret = parse_command(buffer);
            if(ret == RET_TIMEOUT) {
                send_return_value(RET_TIMEOUT);
            }
            handle_command(buffer);
        }
    }

    int InputHandler::parse_command(unsigned char *cmd_buffer) {
        unsigned long cutoff = millis() + CMD_TIMEOUT;
        unsigned long current;
        int cmd_len = MAX_CMD;
        if (Serial.peek() < CUTTOFF_2_PARAM) {
            cmd_len = 2;
        } else if (Serial.peek() < CUTTOFF_3_PARAM) {
            cmd_len = 6;
        } else if (Serial.peek() < CUTTOFF_4_PARAM) {
            cmd_len = 10;
        } else if (Serial.peek() < CUTTOFF_5_PARAM) {
            cmd_len = 14;
        }
        for(int i = 0; i < cmd_len; ++i) {
            current = millis();
            while(!Serial.available() && current < cutoff){
                current = millis();
            }
            if(current >= cutoff) {
                return RET_TIMEOUT;
            }
            cmd_buffer[i] = Serial.read();
        }
        return 0;
    }

    void InputHandler::send_return_value(uint8_t ret) {
        Serial.write(ret);
        Serial.write(RET_ENDLINE);
    }

    void InputHandler::send_return_value(uint8_t ret, uint8_t val) {
        Serial.write(ret);
        Serial.write(val);
        Serial.write(RET_ENDLINE);
    }

    void InputHandler::send_return_values(uint8_t ret, unsigned char *buffer) {
        Serial.write(ret);
        uint8_t size = sizeof(float);
        for(int i = 0; i < size; ++i) {
            Serial.write(buffer[i]);
        }
        Serial.write(RET_ENDLINE);
    }

    void InputHandler::handle_command(unsigned char *cmd_buffer) {
        switch(cmd_buffer[0]) {
            case CMD_HOME:
                cmd_home(cmd_buffer);
                break;
            case CMD_STOP:
                cmd_stop(cmd_buffer);
                break;
            case CMD_IS_HOMING:
                cmd_is_homing(cmd_buffer);
                break;
            case CMD_IS_MOVING:
                cmd_is_moving(cmd_buffer);
                break;
            case CMD_GET_POSN:
                cmd_get_posn(cmd_buffer);
                break;
            case CMD_GET_SPEED:
                cmd_get_speed(cmd_buffer);
                break;
            case CMD_GET_DEFAULT_SPEED:
                cmd_get_default_speed(cmd_buffer);
                break;
            case CMD_GET_SLOW_SPEED:
                cmd_get_slow_speed(cmd_buffer);
                break;
            case CMD_GET_FINE_SPEED:
                cmd_get_fine_speed(cmd_buffer);
                break;
            case CMD_GET_MAX_SPEED:
                cmd_get_max_speed(cmd_buffer);
                break;
            case CMD_GET_ACCEL:
                cmd_get_accel(cmd_buffer);
                break;
            case CMD_GET_LIMIT:
                cmd_get_limit(cmd_buffer);
                break;
            case CMD_GET_DIST_TO_GO:
                cmd_get_dist_to_go(cmd_buffer);
                break;
            case CMD_GET_TARGET_POSN:
                cmd_get_target_posn(cmd_buffer);
                break;
            case CMD_MOVE_ABSOLUTE:
                cmd_move_absolute(cmd_buffer);
                break;
            case CMD_MOVE_RELATIVE:
                cmd_move_relative(cmd_buffer);
                break;
            case CMD_SET_POSN:
                cmd_set_posn(cmd_buffer);
                break;
            case CMD_SET_MAX_SPEED:
                cmd_set_max_speed(cmd_buffer);
                break;
            case CMD_SET_DEFAULT_SPEED:
                cmd_set_default_speed(cmd_buffer);
                break;
            case CMD_SET_SLOW_SPEED:
                cmd_set_slow_speed(cmd_buffer);
                break;
            case CMD_SET_FINE_SPEED:
                cmd_set_fine_speed(cmd_buffer);
                break;
            case CMD_SET_ACCEL:
                cmd_set_accel(cmd_buffer);
                break;
            case CMD_ENABLE_MAG:
                cmd_enable_mag(cmd_buffer);
                break;
            case CMD_MOVE_XZ:
                cmd_move_xz(cmd_buffer);
                break;
            case CMD_MOVE_XYZ:
                cmd_move_xyz(cmd_buffer);
                break;
            default:
                send_return_value(RET_UNRECOGNIZED);
        }
    }

    void InputHandler::bytes_to_float(float *f, unsigned char *cmd_buffer, int offset) {
        memcpy(f, cmd_buffer + offset, sizeof(float));
    }
    void InputHandler::float_to_bytes(float *f, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, f, sizeof(float));
    }
    void InputHandler::bytes_to_long(long *l, unsigned char *cmd_buffer, int offset) {
        memcpy(l, cmd_buffer + offset, sizeof(long));
    }
    void InputHandler::long_to_bytes(long *l, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, l, sizeof(long));
    }
    void InputHandler::bytes_to_int(int *l, unsigned char *cmd_buffer, int offset) {
        memcpy(l, cmd_buffer + offset, sizeof(int));
    }
    void InputHandler::int_to_bytes(int *l, unsigned char *cmd_buffer, int offset) {
        memcpy(cmd_buffer + offset, l, sizeof(int));
    }

    void InputHandler::cmd_home(unsigned char *cmd_buffer) {
        if(cmd_buffer[1] == GANTRY) {
            m_gantry->home();
        } else if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->home();
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->home();
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->home();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
    }
    
    void InputHandler::cmd_stop(unsigned char *cmd_buffer) {
        if(cmd_buffer[1] == GANTRY) {
            m_gantry->stop();
        } else if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->stop();
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->stop();
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->stop();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_is_homing(unsigned char *cmd_buffer) {
        bool val;
        if(cmd_buffer[1] == GANTRY) {
            val = m_gantry->is_homing();
        } else if(cmd_buffer[1] == X_AXIS) {
            val = m_gantry->get_x_axis()->is_homing();
        } else if(cmd_buffer[1] == Y_AXIS) {
            val = m_gantry->get_y_axis()->is_homing();
        } else if(cmd_buffer[1] == Z_AXIS) {
            val = m_gantry->get_z_axis()->is_homing();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        float ret_val;
        if (val)
            ret_val = 1;
        else
            ret_val = 0;
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&ret_val, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_is_moving(unsigned char *cmd_buffer) {
        bool val;
        if(cmd_buffer[1] == GANTRY) {
            val = m_gantry->is_moving();
        } else if(cmd_buffer[1] == X_AXIS) {
            val = m_gantry->get_x_axis()->is_moving();
        } else if(cmd_buffer[1] == Y_AXIS) {
            val = m_gantry->get_y_axis()->is_moving();
        } else if(cmd_buffer[1] == Z_AXIS) {
            val = m_gantry->get_z_axis()->is_moving();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        float ret_val;
        if (val)
            ret_val = 1;
        else
            ret_val = 0;
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&ret_val, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_posn(unsigned char *cmd_buffer) {
        float posn; 
        if(cmd_buffer[1] == X_AXIS) {
            posn = (float) m_gantry->get_x_axis()->get_current_posn();
        } else if(cmd_buffer[1] == Y_AXIS) {
            posn = (float) m_gantry->get_y_axis()->get_current_posn();
        } else if(cmd_buffer[1] == Z_AXIS) {
            posn = (float) m_gantry->get_z_axis()->get_current_posn();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&posn, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_speed(unsigned char *cmd_buffer) {
        float speed;
        if(cmd_buffer[1] == X_AXIS) {
            speed = m_gantry->get_x_axis()->get_speed();
        } else if(cmd_buffer[1] == Y_AXIS) {
            speed = m_gantry->get_y_axis()->get_speed();
        } else if(cmd_buffer[1] == Z_AXIS) {
            speed = m_gantry->get_z_axis()->get_speed();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&speed, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_default_speed(unsigned char *cmd_buffer) {
        float speed;
        if(cmd_buffer[1] == X_AXIS) {
            speed = m_gantry->get_x_axis()->get_default_speed();
        } else if(cmd_buffer[1] == Y_AXIS) {
            speed = m_gantry->get_y_axis()->get_default_speed();
        } else if(cmd_buffer[1] == Z_AXIS) {
            speed = m_gantry->get_z_axis()->get_default_speed();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&speed, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_slow_speed(unsigned char *cmd_buffer) {
        float speed;
        if(cmd_buffer[1] == X_AXIS) {
            speed = m_gantry->get_x_axis()->get_slow_speed();
        } else if(cmd_buffer[1] == Y_AXIS) {
            speed = m_gantry->get_y_axis()->get_slow_speed();
        } else if(cmd_buffer[1] == Z_AXIS) {
            speed = m_gantry->get_z_axis()->get_slow_speed();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&speed, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }

    void InputHandler::cmd_get_fine_speed(unsigned char *cmd_buffer) {
        float speed;
        if(cmd_buffer[1] == X_AXIS) {
            speed = m_gantry->get_x_axis()->get_fine_speed();
        } else if(cmd_buffer[1] == Y_AXIS) {
            speed = m_gantry->get_y_axis()->get_fine_speed();
        } else if(cmd_buffer[1] == Z_AXIS) {
            speed = m_gantry->get_z_axis()->get_fine_speed();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&speed, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_max_speed(unsigned char *cmd_buffer) {
        float speed;
        if(cmd_buffer[1] == X_AXIS) {
            speed = m_gantry->get_x_axis()->get_max_speed();
        } else if(cmd_buffer[1] == Y_AXIS) {
            speed = m_gantry->get_y_axis()->get_max_speed();
        } else if(cmd_buffer[1] == Z_AXIS) {
            speed = m_gantry->get_z_axis()->get_max_speed();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&speed, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_accel(unsigned char *cmd_buffer) {
        float accel;
        if(cmd_buffer[1] == X_AXIS) {
            accel = m_gantry->get_x_axis()->get_acceleration();
        } else if(cmd_buffer[1] == Y_AXIS) {
            accel = m_gantry->get_y_axis()->get_acceleration();
        } else if(cmd_buffer[1] == Z_AXIS) {
            accel = m_gantry->get_z_axis()->get_acceleration();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&accel, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
   
    void InputHandler::cmd_get_limit(unsigned char *cmd_buffer) {
        bool val;
        if(cmd_buffer[1] == X_AXIS) {
            val = m_gantry->get_x_axis()->get_home_lim();
        } else if(cmd_buffer[1] == Y_AXIS) {
            val = m_gantry->get_y_axis()->get_home_lim();
        } else if(cmd_buffer[1] == Z_AXIS) {
            val = m_gantry->get_z_axis()->get_home_lim();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char ret_val[1];
        if (val)
            ret_val[0] = 1;
        else
            ret_val[0] = 1;
        send_return_values(RET_OK, ret_val);
    }
    
    void InputHandler::cmd_get_dist_to_go(unsigned char *cmd_buffer) {
        float dist; 
        if(cmd_buffer[1] == X_AXIS) {
            dist = (float) m_gantry->get_x_axis()->get_dist_to_go();
        } else if(cmd_buffer[1] == Y_AXIS) {
            dist = (float) m_gantry->get_y_axis()->get_dist_to_go();
        } else if(cmd_buffer[1] == Z_AXIS) {
            dist = (float) m_gantry->get_z_axis()->get_dist_to_go();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&dist, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_get_target_posn(unsigned char *cmd_buffer) {
        float posn; 
        if(cmd_buffer[1] == X_AXIS) {
            posn = (float) m_gantry->get_x_axis()->get_target_posn();
        } else if(cmd_buffer[1] == Y_AXIS) {
            posn = (float) m_gantry->get_y_axis()->get_target_posn();
        } else if(cmd_buffer[1] == Z_AXIS) {
            posn = (float) m_gantry->get_z_axis()->get_target_posn();
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        unsigned char float_buffer[sizeof(float)];
        float_to_bytes(&posn, float_buffer, 0);
        send_return_values(RET_OK, float_buffer);
    }
    
    void InputHandler::cmd_move_absolute(unsigned char *cmd_buffer) {
        float posn;
        bytes_to_float(&posn, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->move_absolute(posn);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->move_absolute(posn);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->move_absolute(posn);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }

    void InputHandler::cmd_move_relative(unsigned char *cmd_buffer) {
        float distance;
        bytes_to_float(&distance, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->move_relative(distance);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->move_relative(distance);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->move_relative(distance);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
    }
    
    void InputHandler::cmd_set_posn(unsigned char *cmd_buffer) {
        float posn;
        bytes_to_float(&posn, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_current_posn((long)posn);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->move_absolute((long)posn);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->move_absolute((long)posn);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_set_max_speed(unsigned char *cmd_buffer) {
        float speed;
        bytes_to_float(&speed, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_max_speed(speed);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->set_max_speed(speed);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->set_max_speed(speed);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_set_default_speed(unsigned char *cmd_buffer) {
        float speed;
        bytes_to_float(&speed, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_default_speed(speed);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->set_default_speed(speed);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->set_default_speed(speed);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_set_slow_speed(unsigned char *cmd_buffer) {
        float speed;
        bytes_to_float(&speed, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_slow_speed(speed);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->set_slow_speed(speed);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->set_slow_speed(speed);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_set_fine_speed(unsigned char *cmd_buffer) {
        float speed;
        bytes_to_float(&speed, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_fine_speed(speed);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->set_fine_speed(speed);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->set_fine_speed(speed);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_set_accel(unsigned char *cmd_buffer) {
        float speed;
        bytes_to_float(&speed, cmd_buffer, 2);
        if(cmd_buffer[1] == X_AXIS) {
            m_gantry->get_x_axis()->set_acceleration(speed);
        } else if(cmd_buffer[1] == Y_AXIS) {
            m_gantry->get_y_axis()->set_acceleration(speed);
        } else if(cmd_buffer[1] == Z_AXIS) {
            m_gantry->get_z_axis()->set_acceleration(speed);
        } else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_enable_mag(unsigned char *cmd_buffer) {
        float val;
        bytes_to_float(&val, cmd_buffer, 2);
        bool enable = val == 1;
        if(cmd_buffer[1] == GANTRY) {
            m_gantry->enable_magnet(enable);
        }
        else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_move_xz(unsigned char *cmd_buffer) {
        float x_posn;
        float z_posn;
        bytes_to_float(&x_posn, cmd_buffer, 2);
        bytes_to_float(&z_posn, cmd_buffer, 6);
        if(cmd_buffer[1] == GANTRY) {
            m_gantry->move_xz_absolute(x_posn, z_posn);
        }
        else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
    
    void InputHandler::cmd_move_xyz(unsigned char *cmd_buffer) {
        float x_posn;
        float y_posn;
        float z_posn;
        bytes_to_float(&x_posn, cmd_buffer, 2);
        bytes_to_float(&y_posn, cmd_buffer, 6);
        bytes_to_float(&z_posn, cmd_buffer, 10);
        if(cmd_buffer[1] == GANTRY) {
            m_gantry->move_xyz_absolute(x_posn, y_posn, z_posn);
        }
        else {
            send_return_value(RET_INVALID_PARAM);
            return;
        }
        send_return_value(RET_OK);
        
    }
}
