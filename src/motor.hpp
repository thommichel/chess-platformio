#include <AccelStepper.h>
#include <ezButton.h>
#include <DRV8434S.h>
#ifndef MOTOR_HPP // Check if the macro is not defined
#define MOTOR_HPP
namespace mtr {
    class Motor {
        private:
            AccelStepper m_motor;
            DRV8434S m_driver;
            u_int8_t m_select;
            int m_dir;

            ezButton m_home_lim;
            bool m_home_lim_hit;
            bool m_homing;
            bool m_enabled;
            uint16_t m_micro_res;

            uint16_t m_steps_per_rev;
            uint8_t m_lead_mm;
            long _mm_to_steps(float mm);
            float _steps_to_mm(long steps);
        public:
            Motor();
            Motor(uint8_t select, uint8_t limit_pin, uint16_t steps_per_rev, uint8_t lead_mm);

            void setup_driver(void (*forward_func)(), void (*backwards_func)());
            void setup_driver(void (*forward_func)(), void (*backwards_func)(), int starting_mA);
            void set_current_mA(uint16_t current);
            void set_micro_step(uint8_t resolution);
            void _spi_step_forward();
            void _spi_step_backwards();

            void move_absolute(float absolute_mm);
            void move_relative(float relative_mm);
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
            bool get_home_lim();
            long get_dist_to_go();
            long get_target_posn();
    };
}
#endif
