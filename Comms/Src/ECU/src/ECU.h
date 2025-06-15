#ifndef ECU_H
#define ECU_H

#include <Arduino.h>
#include <FlexCAN_T4.h>

/* ================================
   CONSTANTS & CONFIGURATION
   ================================ */

// Sensor calibration ranges
const uint16_t APPS_GMAX = 1000; // Max APPS raw value
const uint16_t APPS_GMIN = 10;   // Min APPS raw value
const uint16_t BPS_GMAX = 925;
const uint16_t BPS_GMIN = 98;

// CAN IDs
#define APPS_ID 0x104
#define MC_LEFT_RECEIVE 0x202
#define MC_RIGHT_RECEIVE 0x201

// Control Pin assignments
#define CPLUS_PIN 18
#define FRG_PIN 6
#define RFE_PIN 7

/* ================================
   CAN VARIABLES
   ================================ */

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> d_can;
CAN_message_t msg;

/* ================================
   SENSOR & MOTOR STRUCTURES
   ================================ */

struct SensorData
{
    uint16_t raw;
    double norm;
    uint16_t max;
    uint16_t min;
};

struct MotorControllerData
{
    bool enable;
    int rpm;
    int torque;
    double voltage;
    double current;
    double power;
    int motor_temp;
    int controller_temp;
};

/* ================================
   GLOBAL VARIABLES
   ================================ */

SensorData APPS1, APPS2, BPS1, BPS2;
MotorControllerData mc_left, mc_right;

bool precharged = false;

int enable_sent_left = 0;
int enable_sent_right = 0;

/* ================================
   CAN SEND MESSAGES
   ================================ */

const char disable_send[8] = {0x51, 0x04, 0, 0, 0, 0, 0, 0};
const char enable_send[8] = {0x51, 0x40, 0, 0, 0, 0, 0, 0};

// Unused but available for future cyclic requests (30ms intervals)
/*
const char speed_request[8]      = {0x3D, 0x30, 0xC8, 0, 0, 0, 0, 0};
const char torque_request[8]     = {0x3D, 0xA0, 0xC8, 0, 0, 0, 0, 0};
const char voltage_request[8]    = {0x3D, 0xEB, 0xC8, 0, 0, 0, 0, 0};
const char power_request[8]      = {0x3D, 0xF6, 0xC8, 0, 0, 0, 0, 0};
const char current_request[8]    = {0x3D, 0x20, 0xC8, 0, 0, 0, 0, 0};
const char motor_temp_request[8] = {0x3D, 0x49, 0xC8, 0, 0, 0, 0, 0};
const char mc_temp_request[8]    = {0x3D, 0x4A, 0xC8, 0, 0, 0, 0, 0};
*/

/* ================================
   FUNCTION DECLARATIONS
   ================================ */

// CAN communication
void setup_can();
void mc_read(const CAN_message_t &msg);

// System control
void handle_precharge();
void on();
void off();

#endif // ECU_H
