#include "motor.hpp"
#include <Arduino.h>
#include <SPI.h>

#define NUM_MTRS 3
#define CMD_TIMEOUT 3000
#define MAX_CMD 50
#define CMD_ERROR 1
#define BAUD_RATE 9600

mtr::Motor m1(2, 3, 3);
mtr::Motor m2(21, 3, 3);
mtr::Motor m3(22, 3, 3);

mtr::Motor motors[NUM_MTRS];

void handle_user_input();

void handle_command(char *cmd_buffer);

int parse_command(char *cmd_buffer);

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
    handle_user_input();
    motors[i].update();
  }
}

/*
Parse and execute the user's given command.
*/
void handle_user_input() {
  if(Serial.available()) {
    char buffer[MAX_CMD];
    int ret = parse_command(buffer);
    if(ret == CMD_ERROR) {
      return;
    }
    handle_command(buffer);
  }
}

/*
Parse the command given by the user over Serial.
*/
int parse_command(char *cmd_buffer) {
  unsigned long cutoff = millis() + CMD_TIMEOUT;
    char b = Serial.read();
    if(b != '<') {
      return CMD_ERROR;
    }
    unsigned long current;
    bool complete_command = false;
    for(int i = 0; i < MAX_CMD; ++i) {
      current = millis();
      while(!Serial.available() && current < cutoff){
        current = millis();
      }
      if(current >= cutoff) {
        Serial.write("Command timed out\n");
        return CMD_ERROR;
      }
      b = Serial.read();
      if(b == '>') {
        cmd_buffer[i] = '\0';
        complete_command = true;
        break;
      }
      cmd_buffer[i] = b;
    }
    if(!complete_command) {
      Serial.write("Incomplete command\n");
      return CMD_ERROR;
    }
    return 0;
}

/*
Based on the received command in the buffer, perform the desired action.
*/
void handle_command(char *cmd_buffer) {
  char *res;
    res = strtok(cmd_buffer, " ");
    if(strcmp(res, "cmd_name_goes_here") == 0){
      res = strtok(NULL, " ");
      if(strcmp(res, "2nd arg of command") == 0) {
        digitalWrite(1, HIGH);
        Serial.write("OK\n");
      }
      else if(strcmp(res, "2nd arg of command") == 0) {
        digitalWrite(1, LOW);
        Serial.write("OK\n");
      }
      else{
        Serial.write("Invalid use of command\n");
        Serial.write(cmd_buffer, 50);
      }
    } else {
      Serial.write("Invalid command\n");
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
