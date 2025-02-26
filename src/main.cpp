#include "motor.hpp"
#include "gantry.hpp"
#include "input_handler.hpp"
#include <Arduino.h>
#include <SPI.h>

// Serial constants
#define BAUD_RATE 115200

// Folger Tech Nema 17 constants 
#define FOLGER_TECH_ACCEL 750
#define FOLGER_TECH_SPEED 500
#define FOLGER_TECH_STEPS_PER_REV 200
#define FOLGER_TECH_MILLIAMPS 1000

// Lead Screw constants
#define LEAD_MM 4

// Pin constants
#define X_SPI_SEL 2
#define Y_SPI_SEL 21
#define Z_SPI_SEL 22
#define X_LIM 3
#define Y_LIM 3
#define Z_LIM 3
#define ELEC_MAG 3

mtr::Motor x_axis(X_SPI_SEL, X_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED, FOLGER_TECH_MILLIAMPS);
mtr::Motor y_axis(Y_SPI_SEL, Y_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED, FOLGER_TECH_MILLIAMPS);
mtr::Motor z_axis(Z_SPI_SEL, Z_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED, FOLGER_TECH_MILLIAMPS);

gnt::Gantry three_dof(x_axis, y_axis, z_axis, ELEC_MAG);

void x_axis_f();
void x_axis_b();
void y_axis_f();
void y_axis_b();
void z_axis_f();
void z_axis_b();

void setup() {
  SPI.begin();
  Serial.begin(BAUD_RATE);
  delay(1);
  three_dof.setup_drivers(x_axis_f, x_axis_b, y_axis_f, y_axis_b, z_axis_f, z_axis_b);
}

void loop() {
  cmd::handle_user_input(three_dof);
  three_dof.update();
}

void x_axis_f() {
  three_dof.get_x_axis()._spi_step_forward();
}
void x_axis_b() {
  three_dof.get_x_axis()._spi_step_backwards();
}
void y_axis_f() {
  three_dof.get_y_axis()._spi_step_forward();
}
void y_axis_b() {
  three_dof.get_y_axis()._spi_step_backwards();
}void z_axis_f() {
  three_dof.get_z_axis()._spi_step_forward();
}
void z_axis_b() {
  three_dof.get_z_axis()._spi_step_backwards();
}
