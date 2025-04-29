#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// Colors (16-bit RGB565)
#define BLACK         0x0000   // Background
#define DOOR_COLOR    0xC618   // Door color
#define FRAME_COLOR   0xFFFF   // White door frame
#define KNOB_COLOR    0xF800   // Red door knob
#define FACE_COLOR    0xFFE0   // Yellow face
#define EYE_COLOR     0x0000   // Black eyes
#define MOUTH_COLOR   0x0000   // Black mouth
#define TEAR_COLOR    0x001F   // Blue tear
#define CHEST_COLOR   0xA145   // Brownish color for the chest
#define GOLD_COLOR    0x7E0    // Yellow coins
#define SPARK_COLOR   0xFFFF   // White sparkle
#define CAR_GREEN     0x07E0   // Green car
#define CAR_RED       0xF800   // Red car

// Success tick message
#define TICK_MSG      "OK"

// Emoji and tear animation dimensions
#define FACE_CENTER_X 64
#define FACE_CENTER_Y 50
#define FACE_RADIUS   40
#define LEFT_EYE_X    48
#define LEFT_EYE_Y    40
#define RIGHT_EYE_X   80
#define RIGHT_EYE_Y   40
#define EYE_RADIUS    5
#define MOUTH_LEFT_X    48
#define MOUTH_LEFT_Y    70
#define MOUTH_BOTTOM_X  64
#define MOUTH_BOTTOM_Y  85
#define MOUTH_RIGHT_X   80
#define MOUTH_RIGHT_Y   70
#define TEAR_START_Y  (LEFT_EYE_Y + EYE_RADIUS + 2)
#define TEAR_END_Y    65
#define TEAR_RADIUS   2
#define ANIM_DELAY_MS 100   // Delay (ms) between frames in animation

// Game parameters
#define GAME_DURATION  30          // seconds allowed for each challenge in Level 1
#define NUM_CHALLENGES 2           // Total number of challenges in Level 1
#define LEVEL3_PLAYER_W   12
#define LEVEL3_PLAYER_H    8
#define LEVEL3_ENEMIES     3
#define LEVEL3_SPEED       5       // px per frame
#define LEVEL3_SPAWN_RATE  5       // 5% chance per frame
#define LEVEL3_DURATION  30       // seconds
#define TOLERANCE      0.10f       // 10% tolerance window for Level 2

// Flash memory addresses
constexpr uint32_t BEST_TIME_ADDR = 0x0007E000;  
constexpr uint32_t NO_BEST       = 0xFFFFFFFF;

#endif // GAME_CONSTANTS_H 