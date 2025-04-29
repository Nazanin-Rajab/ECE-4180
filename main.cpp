#include "mbed.h"
#include "game_constants.h"
#include "hardware_interface.h"
#include "levels.h"
#include <cstdlib>
#include <chrono>
using namespace std::chrono;

// Global Variables
volatile uint32_t g_best_time_ms = std::numeric_limits<uint32_t>::max(); 
Timer total_timer;

int main() {
    while (true) {
        g_best_time_ms = readBestTimeFromFlash();
        srand(time(NULL));
        init_hardware();

        // --- Best Time Display ---
        myLCD.cls();
        uint32_t shown = (g_best_time_ms == NO_BEST) ? 0 : g_best_time_ms;
        myLCD.locate(0, 0);
        myLCD.printf("Best: %2u.%02u s", shown/1000, (shown%1000)/10);

        myLCD.locate(2, 4);
        myLCD.setTextBold(true);
        myLCD.printf("Welcome to\nTreasure Hunt");
        myLCD.setTextBold(false);
        myLCD.locate(2, 12);
        myLCD.printf("Press Center");
        waitForButtonPress(center);

        // --- Main Menu ---
        myLCD.cls();
        myLCD.locate(0, 1);
        myLCD.setTextBold(true);
        myLCD.printf("Treasure Hunt\nLevels");
        myLCD.setTextBold(false);
        myLCD.locate(0, 4);
        myLCD.printf("1: Binary Puzzle");
        myLCD.locate(0, 5);
        myLCD.printf("2: Lock Picking");
        myLCD.locate(0, 6);
        myLCD.printf("3: Car Chase");
        myLCD.locate(0, 9);
        myLCD.printf("Press Center to\nStart");
        waitForButtonPress(center);

        // --- Start timer for run ---
        total_timer.reset();
        total_timer.start();
        Timer t1, t2, t3;
        uint32_t ms1=0, ms2=0, ms3=0;

        // --- LEVEL 1 ---
        while (true) {
            t1.reset(); t1.start();
            bool result = runLevel1();
            t1.stop();
            if (!result) {
                playFailWithSiren(5000);
                continue;
            } else {
                myLCD.cls();
                myLCD.locate(1, 3);
                myLCD.printf("Level 1 Complete!");
                ThisThread::sleep_for(1500ms);
                break;
            }
        }

        // --- LEVEL 2 ---
        while (true) {
            myLCD.cls();
            myLCD.locate(0, 1);
            myLCD.setTextBold(true);
            myLCD.printf("Level 2: Lock Picking");
            myLCD.setTextBold(false);
            myLCD.locate(0, 3);
            myLCD.printf("Find the sweet spot");
            myLCD.locate(0, 4);
            myLCD.printf("when the LED is");
            myLCD.locate(0, 5);
            myLCD.printf("brightest!");
            myLCD.locate(0, 7);
            myLCD.printf("Press Center to");
            myLCD.locate(0, 8);
            myLCD.printf("Start");
            waitForButtonPress(center);
            myLCD.cls();

            t2.reset(); t2.start();
            bool result = runLevel2();
            t2.stop();
            if (!result) {
                playFailWithSiren(5000);
                main();
                return 0;
            } else {
                myLCD.cls();
                myLCD.locate(1, 3);
                myLCD.printf("Level 2 Complete!");
                ThisThread::sleep_for(1500ms);
                break;
            }
        }

        // --- LEVEL 3 ---
        while (true) {
            myLCD.cls();
            myLCD.locate(0, 0);
            myLCD.setTextBold(true);
            myLCD.printf("Level 3: Car Chase");
            myLCD.setTextBold(false);
            myLCD.locate(0, 2);
            myLCD.printf("Avoid blue cars!");
            myLCD.locate(0, 3);
            myLCD.printf("Survive for 30 s");
            myLCD.locate(0, 5);
            myLCD.printf("Press Center to");
            myLCD.locate(0, 6);
            myLCD.printf("Start");
            waitForButtonPress(center);
            myLCD.cls();

            t3.reset(); t3.start();
            bool result = runLevel3();
            t3.stop();
            if (!result) {
                playFailWithSiren(5000);
                main();
                return 0;
            } else {
                myLCD.cls();
                myLCD.locate(1, 3);
                myLCD.printf("Level 3 Complete!");
                ThisThread::sleep_for(1500ms);
                break;
            }
        }

        uint32_t runMs = total_timer.read_ms();
        if (runMs < g_best_time_ms) {
            g_best_time_ms = runMs;
            writeBestTimeToFlash(g_best_time_ms);
        }

        myLCD.cls();
        myLCD.setTextBold(true);
        myLCD.locate(2, 6);
        myLCD.printf("YOU WON!");
        myLCD.setTextBold(false);

        playVictoryFanfare();

        myLCD.locate(1, 12);
        myLCD.printf("Press Center");
        waitForButtonPress(center);
    }
} 