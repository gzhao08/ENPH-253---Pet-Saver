#pragma once

#define BIT_12_LIMIT 4096

// Encoders
#define AS5600_ADDR 0x36
#define AS5600_MSB_REG 0x0C



// IR Array Error States
#define ERR_4 7
#define ERR_3 5
#define ERR_2 3
#define ERR_1 1

#define KP_DEFAULT 160.0

// Map Division
#define SECTION_0_SPEED 600   // Before doorway -- 0 indexing for use in arrays
#define SECTION_1_SPEED 500   // After doorway, curve before ramp
#define SECTION_2_SPEED 1100  // Ramp

#define DOORWAY_THRESH 120
#define FIRST_PET_THRESH 200
#define RAMP_THRESH 240

// IR Pins:
// Left to right: 35,34,36,39
#define OUTER_RIGHT_PIN 39 
#define INNER_RIGHT_PIN 36
#define INNER_LEFT_PIN 35
#define OUTER_LEFT_PIN 34

// Drive Motors Pins
#define LEFT_MOTOR_PIN_A 19 // 26
#define LEFT_MOTOR_PIN_B 22 // 25
#define RIGHT_MOTOR_PIN_A 20 // 32
#define RIGHT_MOTOR_PIN_B 21 // 33

// Claw Motors Pins
#define ARM_MOTOR_PIN_1 22
#define ARM_MOTOR_PIN_2 19

#define VERTICAL_STAGE_MOTOR_PIN_1 20
#define VERTICAL_STAGE_MOTOR_PIN_2 21

#define BASE_MOTOR_PIN_1 14
#define BASE_MOTOR_PIN_2 27

// Motor PWM Channels
#define LEFT_MOTOR_PWM_CHANNEL_A 0
#define LEFT_MOTOR_PWM_CHANNEL_B 1
#define RIGHT_MOTOR_PWM_CHANNEL_A 2
#define RIGHT_MOTOR_PWM_CHANNEL_B 3
#define ARM_MOTOR_PWM_CHANNEL_1 4
#define ARM_MOTOR_PWM_CHANNEL_2 5
#define VERTICAL_STAGE_MOTOR_PWM_CHANNEL_1 6
#define VERTICAL_STAGE_MOTOR_PWM_CHANNEL_2 7
#define BASE_MOTOR_PWM_CHANNEL_1 8
#define BASE_MOTOR_PWM_CHANNEL_2 9

// I2C
#define I2C_SCL_A_PIN 13
#define I2C_SDA_A_PIN 15

#define I2C_SCL_B_PIN 5
#define I2C_SDA_B_PIN 7



