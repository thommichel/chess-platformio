#include "motor.hpp"

namespace mtr {
    Motor::Motor(): m_homing(false), m_enabled(false), m_home_lim(NOT_A_PIN), m_home_lim_hit(false), m_dir(-1)  {}
    Motor::Motor(uint8_t select, uint8_t limit_pin, uint16_t steps_per_rev, uint8_t lead_mm, float default_acceleration, float default_speed, uint16_t motor_current, uint8_t microstep_res): m_select(select), m_home_lim(limit_pin), m_steps_per_rev(steps_per_rev), m_lead_mm(lead_mm), m_default_acc(default_acceleration), m_default_speed(default_speed), m_motor_current(motor_current), m_micro_res(microstep_res), m_dir(-1), m_homing(false), m_enabled(false), m_home_lim_hit(false), m_motor(0,0,0)
    {
        m_slow_speed = m_default_speed/3;
        m_fine_speed = m_default_speed/5;
        m_home_lim.setDebounceTime(10);
        m_home_lim_hit = m_home_lim.getState();
    }

    void Motor::setup_driver(void (*forward_func)(), void (*backwards_func)()) {
        m_driver.setChipSelectPin(m_select);
        m_driver.resetSettings();
        m_driver.clearFaults();
        m_driver.setCurrentMilliamps(m_motor_current);
        set_micro_step(m_micro_res);
        m_driver.enableSPIDirection();
        m_driver.enableSPIStep();
        m_motor = AccelStepper(forward_func, backwards_func);
        m_motor.setMaxSpeed(m_default_speed);
        m_motor.setAcceleration(m_default_acc);
    }
    
    void Motor::set_current_mA(int current) {
        m_driver.setCurrentMilliamps(current);
    }

    void Motor::set_micro_step(uint8_t resolution) {
        m_driver.setStepMode(resolution);
        m_micro_res = resolution;
        m_slow_speed = m_default_speed/3;
        m_fine_speed = m_default_speed/5;
    }

    void Motor::_spi_step_forward() {
        if (m_dir != 0) {
            m_driver.setDirection(0);
            m_dir = 0;
        }
        m_driver.step();
    }
    void Motor::_spi_step_backwards() {
        if (m_dir != 1) {
            m_driver.setDirection(1);
            m_dir = 1;
        }
        m_driver.step();
    }

    void Motor::move_absolute(float absolute_mm) {
        if(!(m_home_lim_hit && absolute_mm < get_current_posn())) {
            m_motor.moveTo(_mm_to_steps(absolute_mm));
            m_home_lim_hit = false;
        }
    }

    void Motor::move_relative(float relative_mm) {
        if(!(m_home_lim_hit && relative_mm < 0)) {
            m_motor.move(_mm_to_steps(relative_mm));
            m_home_lim_hit = false;
        }
    }

    void Motor::home() {
        m_homing = true;
        set_max_speed(m_slow_speed);

        move_relative(-1000);

        while(is_moving()) {
            update();
        }

        move_relative(15);

        while(is_moving()) {
            update();
        }

        set_max_speed(m_fine_speed);

        move_relative(-1000);

        while(is_moving()) {
            update();
        }

        set_max_speed(m_default_speed);

        set_current_posn(0);

        m_homing = false;
    }

    void Motor::update() {
        if(get_home_lim()) {
            m_motor.setCurrentPosition(m_motor.currentPosition());
        } 
        if(is_moving() && !m_enabled) {
            enable_motor(true);
        } else if(!is_moving() && m_enabled) {
            enable_motor(false);
        }
        m_motor.run();
    }

    void Motor::stop() {
        m_motor.stop();
    }

    bool Motor::is_moving() {
        return m_motor.isRunning();
    }

    bool Motor::is_homing() {
        return m_homing;
    }


    void Motor::set_current_posn(long position_mm) {
        m_motor.setCurrentPosition(_mm_to_steps(position_mm));
    }
    void Motor::set_max_speed(float speed) {
        m_motor.setMaxSpeed(speed);
    }
    void Motor::set_speed(float speed) {
        m_motor.setSpeed(speed);
    }
    void Motor::set_default_speed(float speed) {
        m_default_speed = speed;
    }
    void Motor::set_slow_speed(float speed) {
        m_slow_speed = speed;
    }
    void Motor::set_fine_speed(float speed) {
        m_fine_speed = speed;
    }
    void Motor::set_acceleration(float acceleration) {
        m_motor.setAcceleration(acceleration);
    }
    void Motor::set_limit_debounce_interval(unsigned long ms) {
        m_home_lim.setDebounceTime(ms);
    }
    void Motor::enable_motor(bool enable) {
        if(m_enabled != enable) {
            m_enabled = enable;
            if (m_enabled) {
                m_driver.enableDriver();
            } else {
                m_driver.disableDriver();
            }
        }
    }

    
    long Motor::get_current_posn() {
        return _steps_to_mm(m_motor.currentPosition());
    }
    float Motor::get_speed() {
        return m_motor.speed();
    }
    float Motor::get_default_speed() {
        return m_default_speed;
    }
    float Motor::get_slow_speed() {
        return m_slow_speed;
    }
    float Motor::get_fine_speed() {
        return m_fine_speed;
    }
    float Motor::get_max_speed() {
        return m_motor.maxSpeed();
    }
    float Motor::get_acceleration() {
        return m_motor.acceleration();
    }
    bool Motor::get_home_lim() {
        m_home_lim.loop();
        m_home_lim_hit = m_home_lim.isReleased();
        return m_home_lim_hit;
    }
    long Motor::get_dist_to_go() {
        return _steps_to_mm(m_motor.distanceToGo());
    }
    long Motor::get_target_posn() {
        return _steps_to_mm(m_motor.targetPosition());
    }

    long Motor::_mm_to_steps(float mm) {
        return long(mm * (m_steps_per_rev * m_micro_res)/m_lead_mm);
    }

    float Motor::_steps_to_mm(long steps) {
        return float(steps * 1.0 * m_lead_mm/(m_steps_per_rev * m_micro_res * 1.0));
    }
}