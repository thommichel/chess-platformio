#include <Arduino.h>
#include <SPI.h>
#include "motor.hpp"
#include "gantry.hpp"
#include "input_handler.hpp"

// Serial constants
#define BAUD_RATE 115200

// Folger Tech Nema 17 constants 
#define FOLGER_TECH_ACCEL 8250
#define FOLGER_TECH_SPEED_X 2500
#define FOLGER_TECH_SPEED_Y 2500
#define FOLGER_TECH_SPEED_Z 2500
#define FOLGER_TECH_STEPS_PER_REV 200
#define FOLGER_TECH_MILLIAMPS 1200
#define FOLGER_TECH_MICRO_RES 4

// Lead Screw constants
#define LEAD_MM 8

// Pin constants
#define X_SPI_SEL 22
#define Y_SPI_SEL 21
#define Z_SPI_SEL 2
#define X_LIM 5
#define Y_LIM 15
#define Z_LIM 4
#define ELEC_MAG 17

mtr::Motor x_axis(X_SPI_SEL, X_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED_X, FOLGER_TECH_MILLIAMPS, FOLGER_TECH_MICRO_RES);
mtr::Motor y_axis(Y_SPI_SEL, Y_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED_Y, FOLGER_TECH_MILLIAMPS, FOLGER_TECH_MICRO_RES);
mtr::Motor z_axis(Z_SPI_SEL, Z_LIM, FOLGER_TECH_STEPS_PER_REV, LEAD_MM, FOLGER_TECH_ACCEL, FOLGER_TECH_SPEED_Z, FOLGER_TECH_MILLIAMPS, FOLGER_TECH_MICRO_RES);

gnt::Gantry three_dof(&x_axis, &y_axis, &z_axis, ELEC_MAG);
cmd::InputHandler input_handler(&three_dof);

void x_axis_f();
void x_axis_b();
void y_axis_f();
void y_axis_b();
void z_axis_f();
void z_axis_b();

void setup() {
  SPI.begin();
  Serial.begin(BAUD_RATE);
  pinMode(ELEC_MAG, OUTPUT);
  delay(1);
  three_dof.setup_drivers(x_axis_f, x_axis_b, y_axis_f, y_axis_b, z_axis_f, z_axis_b);
  Serial.write("Stating C.H.E.C.K...\n");
}

void loop() {
  input_handler.handle_user_input();
  three_dof.update();
}

void x_axis_f() {
  three_dof.get_x_axis()->_spi_step_forward();
}
void x_axis_b() {
  three_dof.get_x_axis()->_spi_step_backwards();
}
void y_axis_f() {
  three_dof.get_y_axis()->_spi_step_forward();
}
void y_axis_b() {
  three_dof.get_y_axis()->_spi_step_backwards();
}
void z_axis_f() {
  three_dof.get_z_axis()->_spi_step_forward();
}
void z_axis_b() {
  three_dof.get_z_axis()->_spi_step_backwards();
}
