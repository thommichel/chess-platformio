#include "gantry.hpp"
#include "motor.hpp"

#include <Arduino.h>

namespace gnt {
    Gantry::Gantry(): m_x_axis(mtr::Motor()), m_y_axis(mtr::Motor()), m_z_axis(mtr::Motor()), m_homing(false) {}
    Gantry::Gantry(mtr::Motor& x_axis, mtr::Motor& y_axis, mtr::Motor& z_axis, uint8_t magnet_pin): m_x_axis(x_axis), m_y_axis(y_axis), m_z_axis(z_axis), m_magnet_pin(magnet_pin), m_homing(false) {}

    void Gantry::home() {
        m_homing = true;
        m_x_axis.set_max_speed(m_x_axis.get_slow_speed());
        m_y_axis.set_max_speed(m_y_axis.get_slow_speed());
        m_z_axis.set_max_speed(m_z_axis.get_slow_speed());

        m_x_axis.move_relative(-1000);
        m_y_axis.move_relative(-1000);
        m_z_axis.move_relative(-1000);

        while(is_moving()) {}

        m_x_axis.move_relative(5);
        m_y_axis.move_relative(5);
        m_z_axis.move_relative(5);

        while(is_moving()) {}

        m_x_axis.set_max_speed(m_x_axis.get_fine_speed());
        m_y_axis.set_max_speed(m_y_axis.get_fine_speed());
        m_z_axis.set_max_speed(m_z_axis.get_fine_speed());

        m_x_axis.move_relative(-1000);
        m_y_axis.move_relative(-1000);
        m_z_axis.move_relative(-1000);

        while(is_moving()) {}

        m_x_axis.set_max_speed(m_x_axis.get_default_speed());
        m_y_axis.set_max_speed(m_y_axis.get_default_speed());
        m_z_axis.set_max_speed(m_z_axis.get_default_speed());

        m_x_axis.set_current_posn(0);
        m_y_axis.set_current_posn(0);
        m_z_axis.set_current_posn(0);

        m_homing = false;
    }
    void Gantry::move_xz_absolute(float x_mm, float z_mm) {
        m_x_axis.move_absolute(x_mm);
        m_z_axis.move_absolute(z_mm);
    }
    void Gantry::move_y_absolute(float y_mm) {
        m_y_axis.move_absolute(y_mm);
    }
    void Gantry::stop() {
        m_x_axis.stop();
        m_y_axis.stop();
        m_z_axis.stop();
    }
    void Gantry::enable_magnet(bool enable) {
        digitalWrite(m_magnet_pin, enable);
    }
    void Gantry::update() {
        m_x_axis.update();
        m_y_axis.update();
        m_z_axis.update();
    }
    void Gantry::setup_drivers(void (*xf)(), void (*xb)(), void (*yf)(), void (*yb)(), void (*zf)(), void (*zb)()) {
        m_x_axis.setup_driver(xf, xb);
        m_y_axis.setup_driver(yf, yb);
        m_z_axis.setup_driver(zf, zb);
    }

    bool Gantry::is_moving() {
        return m_x_axis.is_moving() || m_y_axis.is_moving() || m_z_axis.is_moving();
    }
    bool Gantry::is_homing() {
        return m_homing;
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
