#include "hardware_interface.h"
#include "game_constants.h"
#include <chrono>
using namespace std::chrono;

// Hardware Interfaces
uLCD myLCD(p9, p10, p11, uLCD::BAUD_115200);
DigitalIn dip0(p17, PullUp);   // DIP switch Bit 3 (MSB)
DigitalIn dip1(p23, PullUp);   // DIP switch Bit 2
DigitalIn dip2(p24, PullUp);   // DIP switch Bit 1
DigitalIn dip3(p22, PullUp);   // DIP switch Bit 0 (LSB)
DigitalIn center(p13, PullUp); // Nav Switch
DigitalIn submitButton(p21, PullUp);
AnalogIn pot(p20);
PwmOut external_led(p25);
PwmOut audioIn(p26);
ICM20948 imu(p28, p27);

void init_hardware() {
    // Initialize LCD
    myLCD.setTextColor(0xDBAA16);
    myLCD.setTextBackground(BLACK);
}

void waitForButtonPress(DigitalIn &button) {
    while (button.read() != 0) {   
         ThisThread::sleep_for(10);
    }
    while (button.read() == 0) {   
         ThisThread::sleep_for(10);
    }
    ThisThread::sleep_for(100);    
}

uint8_t readDipSwitchBank() {
    uint8_t value = 0;
    value |= ((dip0.read() == 0) ? 1 : 0) << 3;
    value |= ((dip1.read() == 0) ? 1 : 0) << 2;
    value |= ((dip2.read() == 0) ? 1 : 0) << 1;
    value |= ((dip3.read() == 0) ? 1 : 0) << 0;
    return value;
}

void playTone(float frequency, float duration, float volume) {
    audioIn.period(1.0f / frequency);
    audioIn.write(volume);
    ThisThread::sleep_for(milliseconds(int(duration * 1000)));
    audioIn.write(0.0f);
    ThisThread::sleep_for(50ms); 
}

void playVictoryFanfare() {
    float notes[]   = {261.63f, 329.63f, 392.00f, 523.25f};
    float lengths[] = {0.20f,   0.20f,   0.20f,   0.40f  };
    for (int i=0; i<4; i++)
        playTone(notes[i], lengths[i], 0.6f);
    playTone(659.25f, 0.18f, 0.6f);
    playTone(587.33f, 0.18f, 0.6f);
    playTone(523.25f, 0.30f, 0.6f);
}

void playFailWithSiren(uint32_t durationMs) {
    myLCD.cls();
    myLCD.drawCircleFilled(FACE_CENTER_X, FACE_CENTER_Y, FACE_RADIUS, FACE_COLOR);
    myLCD.drawCircleFilled(LEFT_EYE_X,  LEFT_EYE_Y,  EYE_RADIUS,   EYE_COLOR);
    myLCD.drawCircleFilled(RIGHT_EYE_X, RIGHT_EYE_Y, EYE_RADIUS,   EYE_COLOR);
    myLCD.drawLine(MOUTH_LEFT_X, MOUTH_LEFT_Y, MOUTH_BOTTOM_X, MOUTH_BOTTOM_Y, MOUTH_COLOR);
    myLCD.drawLine(MOUTH_BOTTOM_X, MOUTH_BOTTOM_Y, MOUTH_RIGHT_X, MOUTH_RIGHT_Y, MOUTH_COLOR);

    audioIn.period(1.0f/800.0f);  // start at the low tone
    audioIn.write(0.5f);          // 50% duty

    uint32_t start      = Kernel::get_ms_count();
    uint32_t lastTone   = start;
    bool     toneHigh   = false;
    int      tearY      = TEAR_START_Y;

    while (Kernel::get_ms_count() - start < durationMs) {
        uint32_t now = Kernel::get_ms_count();

        int secRem = int((durationMs - (now - start) + 999) / 1000);
        myLCD.locate(0,14);
        myLCD.printf("Restart in: %2d s", secRem>0 ? secRem : 0);

        myLCD.drawCircleFilled(LEFT_EYE_X, tearY, TEAR_RADIUS, TEAR_COLOR);
        ThisThread::sleep_for(ANIM_DELAY_MS);
        myLCD.drawCircleFilled(LEFT_EYE_X, tearY, TEAR_RADIUS, FACE_COLOR);
        tearY++;
        if (tearY > TEAR_END_Y) tearY = TEAR_START_Y; 

        if (now - lastTone >= 250) {
            lastTone = now;
            toneHigh = !toneHigh;
            float freq = toneHigh ? 969.0f : 800.0f;
            audioIn.period(1.0f/freq); 
        }
    } 
    audioIn.write(0.0f);
}

uint32_t readBestTimeFromFlash() {
    FlashIAP flash;
    flash.init();
    uint32_t best = NO_BEST;
    flash.read(&best, BEST_TIME_ADDR, sizeof(best));
    flash.deinit();
    return best;
}

void writeBestTimeToFlash(uint32_t best) {
    FlashIAP flash;
    flash.init(); 
    uint32_t sz = flash.get_sector_size(BEST_TIME_ADDR);
    flash.erase(BEST_TIME_ADDR, sz);
    flash.program(&best, BEST_TIME_ADDR, sizeof(best));
    flash.deinit();
} 