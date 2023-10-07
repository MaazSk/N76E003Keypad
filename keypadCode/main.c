#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#define KEYPAD_PORT P1 // Assuming keypad is connected to Port 1
#define BUZZER_PORT P2 // Buzzer connected to Port 2
#define BUZZER_PIN P20 // Buzzer connected to Port 2, Pin 0 (P2.0)
#define ROWS 4
#define COLS 3

unsigned char keypad[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++) {
            _nop_();
        }
    }
}

void initKeypad() {
    P1M1 = 0x00; // Set Port 1 as Quasi mode
    P1M2 = 0x00;
}

void initBuzzer() {
    BUZZER_PORT &= ~(1 << 0); // Set P2.0 as push-pull output
    BUZZER_PIN = 0; // Turn off the buzzer initially
}

void buzzerOn() {
    BUZZER_PIN = 1; // Turn on the buzzer
}

void buzzerOff() {
    BUZZER_PIN = 0; // Turn off the buzzer
}

char scanKeypad() {
    unsigned char i, j;
    for (i = 0; i < COLS; i++) {
        KEYPAD_PORT = 0xFF ^ (1 << (i + 4)); // Ground one column at a time
        for (j = 0; j < ROWS; j++) {
            if (!(KEYPAD_PORT & (1 << j))) { // Check if the key is pressed
                while (!(KEYPAD_PORT & (1 << j))); // Wait for key release
                return keypad[j][i]; // Return the pressed key
            }
        }
    }
    return '\0'; // No key pressed
}

void main() {
    char key;
    initKeypad();
    initBuzzer();
    while (1) {
        key = scanKeypad();
        if (key != '\0') {
            buzzerOn(); // Turn on the buzzer when a key is pressed
            printf("Key Pressed: %c\n", key); // Print the pressed key
            delay_ms(100); // Add a delay to avoid multiple keypresses
            buzzerOff(); // Turn off the buzzer
        }
    }
}
