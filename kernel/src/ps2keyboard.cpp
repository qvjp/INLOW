#include <inlow/kernel/print.h>
#include <inlow/kernel/port.h>
#include <inlow/kernel/ps2keyboard.h>

#define KEYBOARD_ENABLE_SCANNING 0xF4
#define KEYBOARD_SET_LED 0xED

static void sendKeyboardCommand(uint8_t command);
static void sendKeyboardCommand(uint8_t command, uint8_t data);

PS2Keyboard::PS2Keyboard() {
    listener = nullptr;
    sendKeyboardCommand(KEYBOARD_ENABLE_SCANNING);
    Print::printf("Keyboard initialized\n");
}

enum {
    STATE_NORMAL,
    STATE_ESCAPED,
};

static int state = STATE_NORMAL;
static uint8_t ledState = 0;

void PS2Keyboard::irqHandler() {
    uint8_t data = Hardwarecommunication::inportb(0x60);
    int keycode;

    if (data == 0xFA || data == 0xFE) {
        return;
    } else if (data == 0xE0) {
        state = STATE_ESCAPED;
    } else {
        if (state == STATE_NORMAL) {
            keycode = data & 0x7F;
        } else {
            keycode = data | 0x80;
            state = STATE_NORMAL;
        }
        bool released = data & 0x80;

        handleKey(released ? -keycode : keycode);

    }
}

void PS2Keyboard::handleKey(int keycode) {
    uint8_t newLed = ledState;
    if (keycode == 0x45) { // NumLock
        newLed ^= 2;
    } else if (keycode == 0x3A) { // CapsLock
        newLed ^= 4;
    } else if (keycode == 0x46) { // ScrollLock
        newLed ^= 1;
    }

    if (newLed != ledState) {
        ledState = newLed;

        sendKeyboardCommand(KEYBOARD_SET_LED, ledState);
    }
    if (listener) {
        listener->onKeyboardEvent(keycode);
    }
}

static void sendKeyboardCommand(uint8_t command) {
    while (Hardwarecommunication::inportb(0x64) & 2);
    Hardwarecommunication::outportb(0x60, command);
}
static void sendKeyboardCommand(uint8_t command, uint8_t data) {
    while (Hardwarecommunication::inportb(0x64) & 2);
    Hardwarecommunication::outportb(0x60, command);
    while (Hardwarecommunication::inportb(0x64) & 2);
    Hardwarecommunication::outportb(0x60, data);
}