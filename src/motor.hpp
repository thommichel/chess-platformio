#include <AccelStepper.h>
#include <ezButton.h>
#include <DRV8434S.h>
#ifndef MOTOR_HPP // Check if the macro is not defined
#define MOTOR_HPP
namespace mtr {
    class Motor {
        private:
            bool m_homing;
            bool m_enabled;
            AccelStepper m_motor;
            DRV8434S m_driver;
            ezButton m_lim_n;
            ezButton m_lim_p;
            bool m_lim_neg;
            bool m_lim_pos;
            u_int8_t m_select;

        public:
            Motor();
            Motor(uint8_t select, uint8_t lim_neg, uint8_t lim_pos);

            void move_absolute(long absolute);
            void move_relative(long relative);
            void move_at_velocity();
            void update();
            void stop();
            bool is_moving();

            void set_current_posn(long position);
            void set_max_speed(float speed);
            void set_speed(float speed);
            void set_acceleration(float acceleration);
            void set_limit_debounce_interval(unsigned long ms);
            void enable_motor(bool enable);

            long get_current_posn();
            float get_speed();
            float get_max_speed();
            float get_acceleration();
            bool get_lim_neg();
            bool get_lim_pos();
            long get_dist_to_go();
            long get_target_posn();

            void setup_driver(void (*forward_func)(), void (*backwards_func)());
            void setup_driver(void (*forward_func)(), void (*backwards_func)(), int starting_mA);
            void _spi_step_forward();
            void _spi_step_backwards();
    };
}
#endif
