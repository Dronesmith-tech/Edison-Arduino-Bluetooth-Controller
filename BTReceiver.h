#ifdef __BT_RECEIVER_H__
#define __BT_RECEIVER_H__

/**
 * The name of input controller. This may need to be changed.
 */
#define DEVICE_NAME     "/dev/input/event2"

/**
 * Initializes the receiver. Should be called once, during the setup sequence.
 */
void Bluetooth_Init();

/**
 * Connects to the controller for usage.
 * This is called by Init automatically, but you'll need to call this
 * after closing the connection. If Init fails to connect a device (usually
 * inidicating one isn't connected to Linux yet) then you'll have to call
 * this manually, so make sure this is a hook in SerialCom
 *
 * @returns true if connected, false if not.
 */
bool Bluetooth_Open();

/**
 * Disconnects the controller.
 * NOTE This *MUST* be called before exiting your Arduino program, or you will
 * have to annoyingly reboot the Edison every time you attempt to use this
 * module. It is therefore essential to add this as a hook in SerialCom
 */
void Bluetooth_Close();

/**
 * Polls the controller's current button state, stick state, etc.
 * This should be called in the main update loop.
 */
void Bluetooth_Read();

/**
 * Proposed Configuration for FMU
 * Right Thumb stick: Attitude
 * Left Thumb stick: Yaw
 * Left Trigger: Throttle
 *
 * Cross: Arm Motors
 * Triangle: Disarm Motors
 * Square: Change Flight Mode
 * Circle: Gyro/Accel Calibration
 *
 */
unsigned getThrottle();
unsigned getYaw();
unsigned getPitch();
unsigned getRoll();
unsigned getArmBtn();
unsigned getDisarmBtn();
unsigned getModeBtn();
unsigned getCalibBtn();

#endif /* __BT_RECEIVER_H__ */
