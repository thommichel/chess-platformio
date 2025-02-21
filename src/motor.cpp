#include <AccelStepper.h>
#include "motor.hpp"
#include <ezButton.h>
#include <SPI.h>
namespace mtr {
    Motor::Motor(): m_homing(false), m_enabled(false), m_lim_n(NOT_A_PIN), m_lim_p(NOT_A_PIN), m_lim_neg(false), m_lim_pos(false) {}
    Motor::Motor(uint8_t select, uint8_t lim_neg, uint8_t lim_pos): m_select(select), m_homing(false), m_enabled(false), m_lim_n(lim_neg), m_lim_p(lim_pos), m_lim_neg(false), m_lim_pos(false)
    {
        m_lim_n.setDebounceTime(10);
        m_lim_p.setDebounceTime(10);
        m_lim_neg = m_lim_n.getState();
        m_lim_pos = m_lim_p.getState();
    }

    void Motor::setup_driver(void (*forward_func)(), void (*backwards_func)(), int starting_mA) {
        Serial.println("Driver Setup!");
        m_driver.setChipSelectPin(m_select);
        m_driver.resetSettings();
        m_driver.clearFaults();
        m_driver.setCurrentMilliamps(starting_mA);
        m_driver.setStepMode(DRV8434SStepMode::MicroStep1);
        m_driver.enableSPIDirection();
        m_driver.enableSPIStep();
        m_driver.enableDriver();
        m_motor = AccelStepper(forward_func, backwards_func);
    }

    void Motor::setup_driver(void (*forward_func)(), void (*backwards_func)()) {
        setup_driver(forward_func, backwards_func, 500);
    }

    void Motor::_spi_step_forward() {
        // Serial.println("Forward called");
        m_driver.setDirection(0);
        m_driver.step();
        // Serial.println("Forward over");
    }
    void Motor::_spi_step_backwards() {
        // Serial.println("Backwards called");
        m_driver.setDirection(1);
        m_driver.step();
    }

    void Motor::move_absolute(long absolute) {
        if(!((m_lim_pos && absolute > get_current_posn()) || (m_lim_neg && absolute < get_current_posn()))) {
            m_motor.moveTo(absolute);
            m_lim_neg = false;
            m_lim_pos = false;
        }
    }

    void Motor::move_relative(long relative) {
        if(!((m_lim_neg && relative < 0) || (m_lim_pos && relative > 0))) {
            m_motor.move(relative);
            m_lim_neg = false;
            m_lim_pos = false;
        }
        m_motor.move(relative);
    }

    void Motor::move_at_velocity() {
        if(!((m_lim_pos && get_speed() > 0) || (m_lim_neg && get_speed() < 0))) {
            m_motor.runSpeed();
        }
    }

    void Motor::update() {
        // if((!m_lim_pos && get_lim_pos()) || (!m_lim_neg && get_lim_neg())) {
        //     Serial.println("Hit Limit");
        //     m_motor.setCurrentPosition(m_motor.currentPosition());
        // } 
        if(is_moving() && !m_enabled) {
            // Serial.println("Starting to move");
            enable_motor(true);
            m_motor.run();
        } else if(is_moving()) {
            // Serial.println("Moving");
            m_motor.run();
        } else if(!is_moving() && m_enabled) {
            // Serial.println("Reached destination");
            enable_motor(false);
        }
        m_motor.run();
        // Serial.println("Update Called");
    }

    void Motor::stop() {
        m_motor.stop();
    }

    bool Motor::is_moving() {
        return m_motor.isRunning();
    }


    void Motor::set_current_posn(long position) {
        m_motor.setCurrentPosition(position);
    }
    void Motor::set_max_speed(float speed) {
        m_motor.setMaxSpeed(speed);
    }
    void Motor::set_speed(float speed) {
        m_motor.setSpeed(speed);
    }
    void Motor::set_acceleration(float acceleration) {
        m_motor.setAcceleration(acceleration);
    }
    void Motor::set_limit_debounce_interval(unsigned long ms) {
        m_lim_n.setDebounceTime(ms);
        m_lim_p.setDebounceTime(ms);
    }
    void Motor::enable_motor(bool enable) {
        if(m_enabled != enable) {
            // m_motor.enableOutputs();
            m_enabled = enable;
        }
        else {
            // m_motor.disableOutputs();
            m_enabled = enable;
        }
    }



    long Motor::get_current_posn() {
        return m_motor.currentPosition();
    }
    float Motor::get_speed() {
        return m_motor.speed();
    }
    float Motor::get_max_speed() {
        return m_motor.maxSpeed();
    }
    float Motor::get_acceleration() {
        return m_motor.acceleration();
    }
    bool Motor::get_lim_neg() {
        m_lim_n.loop();
        m_lim_neg = m_lim_n.isPressed();
        return m_lim_neg;
    }
    bool Motor::get_lim_pos() {
        m_lim_p.loop();
        m_lim_pos = m_lim_p.isPressed();
        return m_lim_pos;
    }
    long Motor::get_dist_to_go() {
        return m_motor.distanceToGo();
    }
    long Motor::get_target_posn() {
        return m_motor.targetPosition();
    }
}