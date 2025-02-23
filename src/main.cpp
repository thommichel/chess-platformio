#include "motor.hpp"
#include "input_handler.hpp"
#include <Arduino.h>
#include <SPI.h>

#define NUM_MTRS 3
#define BAUD_RATE 9600
#define FOLGER_TECH_MAX_SPEED 500
#define FOLGER_TECH_STEPS_PER_REV 200
#define LEAD_MM 4

mtr::Motor m1(2, 3, FOLGER_TECH_STEPS_PER_REV, LEAD_MM);
mtr::Motor m2(21, 3, FOLGER_TECH_STEPS_PER_REV, LEAD_MM);
mtr::Motor m3(22, 3, FOLGER_TECH_STEPS_PER_REV, LEAD_MM);

mtr::Motor motors[NUM_MTRS];

void m1_f();
void m1_b();
void m2_f();
void m2_b();
void m3_f();
void m3_b();

void setup() {
  SPI.begin();
  Serial.println("Starting program...");
  Serial.begin(BAUD_RATE);
  delay(1);
  m1.setup_driver(m1_f, m1_b);
  m2.setup_driver(m2_f, m2_b);
  m3.setup_driver(m3_f, m3_b);
  motors[0] = m1;
  motors[1] = m2;
  motors[2] = m3;
  motors[2].set_max_speed(500);
  motors[2].set_acceleration(750);
  motors[2].move_relative(200*10);
  motors[1].set_max_speed(500);
  motors[1].set_acceleration(750);
  motors[1].move_relative(200*10);
}

void loop() {
  int i = 0;
  for(; i < NUM_MTRS; ++i) {
    cmd::handle_user_input();
    motors[i].update();
  }
}

void m1_f() {
  m1._spi_step_forward();
}
void m1_b() {
  m1._spi_step_backwards();
}
void m2_f() {
  m2._spi_step_forward();
}
void m2_b() {
  m2._spi_step_backwards();
}void m3_f() {
  m3._spi_step_forward();
}
void m3_b() {
  m3._spi_step_backwards();
}
