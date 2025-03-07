
#ifndef GANTRY_HPP
#define GANTRY_HPP
#include "motor.hpp"

namespace gnt {
    class Gantry {
        public:
            Gantry();
            Gantry(mtr::Motor& x_axis, mtr::Motor& y_axis, mtr::Motor& z_axis, uint8_t magnet_pin);

            void home();
            void move_xz_absolute(float x_mm, float z_mm);
            void move_y_absolute(float y_mm);
            void stop();
            void enable_magnet(bool enable);
            void update();
            void setup_drivers(void (*xf)(), void (*xb)(), void (*yf)(), void (*yb)(), void (*zf)(), void (*zb)());

            bool is_moving();
            bool is_homing();

            mtr::Motor& get_x_axis();
            mtr::Motor& get_y_axis();
            mtr::Motor& get_z_axis();

        private:
            mtr::Motor m_x_axis;
            mtr::Motor m_y_axis;
            mtr::Motor m_z_axis;
            uint8_t m_magnet_pin;
            bool m_homing;
    };
}
#endif
