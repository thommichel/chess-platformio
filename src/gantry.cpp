#include "gantry.hpp"
#include "motor.hpp"

#include <Arduino.h>

namespace gnt {
    Gantry::Gantry(): m_x_axis(mtr::Motor()), m_y_axis(mtr::Motor()), m_z_axis(mtr::Motor()) {}
    Gantry::Gantry(mtr::Motor x_axis, mtr::Motor y_axis, mtr::Motor z_axis, uint8_t magnet_pin): m_x_axis(x_axis), m_y_axis(y_axis), m_z_axis(z_axis), m_magnet_pin(magnet_pin) {}

    void Gantry::toggle_magnet(bool enable) {
        digitalWrite(m_magnet_pin, enable);
    }

    void Gantry::move_to(long x_mm, long y_mm) {
        m_x_axis.move_absolute(x_mm);
        m_y_axis.move_absolute(y_mm);
    }

    void Gantry::pick_place_piece(ChessPiece piece, bool pickup) {
        long height;
        
        switch (piece) {
            case PAWN:
                height = 123; // Find height needed for each piece
                break;
            case ROOK:
                height = 123; // Find height needed for each piece
                break;
            case KNIGHT:
                height = 123; // Find height needed for each piece
                break;
            case BISHOP:
                height = 123; // Find height needed for each piece
                break;
            case QUEEN:
                height = 123; // Find height needed for each piece
                break;
            case KING:
                height = 123; // Find height needed for each piece
                break;
            default:
                return;
        }

        m_z_axis.move_absolute(height);
        // make it blocking
        // enable the magnet
        // move up by the same height / go back to 0

    }

    void Gantry::home() {

    }

    mtr::Motor Gantry::get_x_axis() {
        return m_x_axis; 
    }
            
    mtr::Motor Gantry::get_y_axis() {
        return m_y_axis;
    }
    
    mtr::Motor Gantry::get_z_axis() {
        return m_z_axis;
    }

}