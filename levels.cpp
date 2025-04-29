#include "levels.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

// Level 3 game state
int l3_playerX = 64, l3_playerY = 110;
int l3_enemyX[LEVEL3_ENEMIES], l3_enemyY[LEVEL3_ENEMIES];
bool l3_active[LEVEL3_ENEMIES];
Timer l3_gameTimer;

void displayDecimalNumber(uint8_t decNum) {
    myLCD.cls();
    myLCD.locate(0, 0);
    myLCD.printf("Convert");
    myLCD.locate(0, 1);
    myLCD.printf("Decimal: %d", decNum);
}

void displayBinaryInput(uint8_t value, int timeRemaining) {
    char binaryStr[5];
    for (int i = 0; i < 4; i++) {
         int bit = (value >> (3 - i)) & 0x01;
         binaryStr[i] = bit ? '1' : '0';
    }
    binaryStr[4] = '\0';
    myLCD.locate(0, 3);
    myLCD.printf("Binary: %s", binaryStr);
    myLCD.locate(0, 5);
    myLCD.printf("Press submit   ");
    myLCD.locate(0, 7);
    myLCD.printf("Time: %2d s", timeRemaining);
}

bool runLevel1() {
    for (int challenge = 0; challenge < NUM_CHALLENGES; challenge++) {
         uint8_t decimalNum = rand() % 16;  // Random number between 0 and 15
         uint8_t correctAnswer = decimalNum;
         displayDecimalNumber(decimalNum);
    
         Timer challengeTimer;
         challengeTimer.start();
         bool submitted = false;
         uint8_t playerValue = 0;
    
         while (challengeTimer.read() < GAME_DURATION) { 
              playerValue = readDipSwitchBank();
              int secondsRemaining = GAME_DURATION - challengeTimer.read();
              if (secondsRemaining < 0) { secondsRemaining = 0; }
              displayBinaryInput(playerValue, secondsRemaining);
    
              if (submitButton.read() == 0) {
                   submitted = true;
                   break;
              }
              ThisThread::sleep_for(50);
         }
    
         if (!submitted || playerValue != correctAnswer) {
              return false;
         }
    
         ThisThread::sleep_for(1000);
    }
    return true;
}

bool runLevel2() {
    DigitalOut stage_leds[3] = {DigitalOut(LED1), DigitalOut(LED2), DigitalOut(LED3)};
    float target_values[3];
    srand(pot.read_u16());
    for (int i = 0; i < 3; i++) {
        target_values[i] = (float)rand() / (float)RAND_MAX;
    }

    int current_stage = 0;
    Timer hold_timer;
    bool timer_active = false;
    Timer level2Timer;
    level2Timer.start();
    const int LEVEL2_DURATION = 60; 
    myLCD.cls();

    while (true) {
        int timeElapsed = level2Timer.read();
        int timeRemaining = LEVEL2_DURATION - timeElapsed;
        if(timeRemaining < 0) { timeRemaining = 0; }

        myLCD.locate(0, 0);
        myLCD.printf("Time: %2d s", timeRemaining);

        if (timeRemaining <= 0) {
            return false;
        }

        float current = pot.read();
        float error = fabs(current - target_values[current_stage]);

        float brightness = 1.0f - pow(error, 0.15f);
        if (brightness < 0.0f) brightness = 0.0f;
        if (brightness > 1.0f) brightness = 1.0f;
        external_led = brightness;

        if (error <= TOLERANCE) {
            if (!timer_active) {
                hold_timer.reset();
                hold_timer.start();
                timer_active = true;
            }
            if (hold_timer.read() >= 2.0f) {
                stage_leds[current_stage] = 1;
                if (current_stage < 2) {
                    current_stage++;
                    hold_timer.reset();
                    timer_active = false;
                } else {
                    external_led = 1.0f;
                    ThisThread::sleep_for(2000ms);
                    return true;
                }
            }
        } else {
            hold_timer.reset();
            hold_timer.stop();
            timer_active = false;
        }
        ThisThread::sleep_for(80ms);
    }
}

void spawnEnemy3() {
    for (int i = 0; i < LEVEL3_ENEMIES; i++) {
        if (!l3_active[i]) {
            l3_active[i] = true;
            l3_enemyX[i] = rand() % (118 - 10) + 10;
            l3_enemyY[i] = -10;
            break;
        }
    }
}

bool checkCollision3() {
    for (int i = 0; i < LEVEL3_ENEMIES; i++) {
        if (l3_active[i] &&
            abs(l3_playerX - l3_enemyX[i]) < LEVEL3_PLAYER_W &&
            abs(l3_playerY - l3_enemyY[i]) < LEVEL3_PLAYER_H) {
            return true;
        }
    }
    return false;
}

void drawLevel3() {
    myLCD.drawRectangleFilled(0, 8, 127, 127, BLACK);

    for (int i = 0; i < LEVEL3_ENEMIES; i++) {
        if (l3_active[i]) {
            myLCD.drawRectangleFilled(
                l3_enemyX[i] - LEVEL3_PLAYER_W/2, l3_enemyY[i] - LEVEL3_PLAYER_H/2,
                l3_enemyX[i] + LEVEL3_PLAYER_W/2, l3_enemyY[i] + LEVEL3_PLAYER_H/2,
                CAR_RED
            );
        }
    }

    myLCD.drawRectangleFilled(
        l3_playerX - LEVEL3_PLAYER_W/2, l3_playerY - LEVEL3_PLAYER_H/2,
        l3_playerX + LEVEL3_PLAYER_W/2, l3_playerY + LEVEL3_PLAYER_H/2,
        CAR_GREEN
    );

    myLCD.drawRectangleFilled(0, 0, 127, 8, BLACK);
    myLCD.locate(0,0);
    int rem = LEVEL3_DURATION - int(l3_gameTimer.read());
    myLCD.printf("Time: %2d  ", rem > 0 ? rem : 0);
}

bool runLevel3() {
    l3_playerX = 64;
    l3_playerY = 110;
    for (int i = 0; i < LEVEL3_ENEMIES; i++) {
        l3_active[i] = false;
        l3_enemyX[i] = 0;
        l3_enemyY[i] = -30;
    }

    myLCD.cls();
    myLCD.locate(0, 0);
    myLCD.printf("Calibrating IMU...");
    imu.powerOn();
    imu.init(ACC_RATE_4g, ACC_SMPLRT_100Hz, GYRO_RATE_250, GYRO_SMPLRT_100Hz);
    imu.gyroCalib();
    ThisThread::sleep_for(500ms);

    l3_gameTimer.reset();
    l3_gameTimer.start();

    while (true) {
        float ax, ay, az;
        imu.getAccGyro();
        ax = imu.getAX();
        l3_playerX += static_cast<int>(ax * 15.0f);
        if (l3_playerX < 10) l3_playerX = 10;
        if (l3_playerX > 118) l3_playerX = 118;

        for (int i = 0; i < LEVEL3_ENEMIES; i++) {
            if (l3_active[i]) {
                l3_enemyY[i] += LEVEL3_SPEED;
                if (l3_enemyY[i] > 127) l3_active[i] = false;
            } else if (rand() % 100 < LEVEL3_SPAWN_RATE) {
                spawnEnemy3();
            }
        }

        drawLevel3();

        if (checkCollision3()) return false;
        if (l3_gameTimer.read() >= LEVEL3_DURATION) return true;

        ThisThread::sleep_for(14ms);
    }
} 