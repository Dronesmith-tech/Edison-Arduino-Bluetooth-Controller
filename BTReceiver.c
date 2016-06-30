
/**
 * Bluetooth Flight Receiver for Edison
 *
 * Make sure the device is paired and connected before attempting to use this.
 *
 */

#include "BTReceiver.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>


#define TYPE_RELATIVE   3
#define TYPE_BUTTON     1

#define REL_LEFT_THUMB_X      0x00
#define REL_LEFT_THUMB_Y      0x01
#define REL_RIGHT_THUMB_X     0x02
#define REL_LEFT_TRIG2        0x03
#define REL_RIGHT_TRIG2       0x04
#define REL_RIGHT_THUMB_Y     0x05

#define BTN_SQUARE            0x130
#define BTN_CROSS             0x131
#define BTN_CIRCLE            0x132
#define BTN_TRIANGLE          0x133
#define BTN_LEFT_TRIG1        0x134
#define BTN_RIGHT_TRIG1       0x135
#define BTN_LEFT_TRIG2        0x136
#define BTN_RIGHT_TRIG2       0x137
#define BTN_LEFT_THUMB        0x13A
#define BTN_RIGHT_THUMB       0x13B

int fd = NULL;
bool readbuff = false;

struct input_event ev[64];

struct BTReceiver_s {
  unsigned throttle;
  unsigned yaw;
  unsigned pitch;
  unsigned roll;
  bool arm;
  bool disarm;
  bool calib;
  bool mode;
} BTReceiver;

bool Bluetooth_Open() {
  if (fd) {
    close (fd);
  }

  if ((fd = open(DEVICE_NAME, O_RDONLY)) < 0) {
    Serial.println("[BT Receiver] Could not open controller.");
      fd = NULL;
      return false;
  }

  return true;
}

void Bluetooth_Close() {
  if (!fd || fd < 0) {
    Serial.println("[BT Receiver] No Device open!");
  } else {
    close(fd);
    Serial.println("[BT Receiver] Controller closed.");
  }
}

void Bluetooth_Read() {
  if (fd && readbuff) {
    int rd = 0;
    int i = 0;
    rd = read(fd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event)) {
      // try again?
    } else {
      for (i = 0; i < rd / sizeof(struct input_event); i++) {

        if (ev[i].type == TYPE_RELATIVE) {
          switch (ev[i].code) {
            case REL_LEFT_THUMB_X:
              BTReceiver.yaw = ev[i].value;
              break;

            case REL_LEFT_TRIG2:
              BTReceiver.throttle = ev[i].value;
              break;

            case REL_RIGHT_THUMB_X:
              BTReceiver.roll = ev[i].value;
              break;

            case REL_RIGHT_THUMB_Y:
              BTReceiver.pitch = ev[i].value;
              break;
          }
        } else if (ev[i].type = TYPE_BUTTON) {
          switch (ev[i].code) {
            case BTN_CROSS:
              BTReceiver.arm = (bool)ev[i].value;
              break;

            case BTN_TRIANGLE:
              BTReceiver.disarm = (bool)ev[i].value;
              break;

            case BTN_CIRCLE:
              BTReceiver.calib = (bool)ev[i].value;
              break;

            case BTN_SQUARE:
              BTReceiver.mode = (bool)ev[i].value;
              break;
          }
        }
      }
    }
  }
}

void Bluetooth_Init() {

  BTReceiver.throttle = 0;
  BTReceiver.yaw = 0;
  BTReceiver.pitch = 0;
  BTReceiver.roll = 0;

  BTReceiver.arm = false;
  BTReceiver.disarm = false;
  BTReceiver.calib = false;
  BTReceiver.mode = false;

  if (Bluetooth_Open()) {
    Serial.println("[BT Receiver] Ready.");
  } else {
    Serial.println("[BT Receiver] Error during init.");
  }
}

unsigned getThrottle()  { return BTReceiver.throttle; };
unsigned getYaw()       { return BTReceiver.yaw; };
unsigned getPitch()     { return BTReceiver.pitch; };
unsigned getRoll()      { return BTReceiver.roll; };
unsigned getArmBtn()    { return BTReceiver.arm; };
unsigned getDisarmBtn() { return BTReceiver.disarm; };
unsigned getModeBtn()   { return BTReceiver.mode; };
unsigned getCalibBtn()  { return BTReceiver.calib; };
