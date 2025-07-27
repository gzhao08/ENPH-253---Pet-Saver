#pragma once

#define BIT_12_LIMIT 4096

// Encoders
#define AS5600_ADDR 0x36
#define AS5600_MSB_REG 0x0C

#define I2C_SCL_A_PIN 13
#define I2C_SDA_A_PIN 15

// IR Array Error States
#define ERR_4 7
#define ERR_3 5
#define ERR_2 3
#define ERR_1 1

// Map Division
#define SECTION_0_SPEED 600   // Before doorway -- 0 indexing for use in arrays
#define SECTION_1_SPEED 500   // After doorway, curve before ramp
#define SECTION_2_SPEED 1100  // Ramp

#define DOORWAY_THRESH 60
#define RAMP_THRESH 100

