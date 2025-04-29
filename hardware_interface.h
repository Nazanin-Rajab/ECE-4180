#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include "mbed.h"
#include "uLCD.hpp"
#include "ICM20948.h"
#include "FlashIAP.h"

// Hardware Interfaces
extern uLCD myLCD;
extern DigitalIn dip0;
extern DigitalIn dip1;
extern DigitalIn dip2;
extern DigitalIn dip3;
extern DigitalIn center;
extern DigitalIn submitButton;
extern AnalogIn pot;
extern PwmOut external_led;
extern PwmOut audioIn;
extern ICM20948 imu;

// Hardware initialization
void init_hardware();

// Helper functions
void waitForButtonPress(DigitalIn &button);
uint8_t readDipSwitchBank();
void playTone(float frequency, float duration, float volume = 0.5f);
void playVictoryFanfare();
void playFailWithSiren(uint32_t durationMs);

// Flash memory operations
uint32_t readBestTimeFromFlash();
void writeBestTimeToFlash(uint32_t best);

#endif // HARDWARE_INTERFACE_H 