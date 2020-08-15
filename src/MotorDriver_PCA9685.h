/*
 *  @file MotorDriver_PCA9685.h
 *
 *  This is a library for Motor driver.
 *
 *  Designed specifically to work with the Yfrobot Motor driver.
 *   L298P MD01 MD02 MD03 MD04 PM-R3 DRV8833 TB6612
 *
 *  These driver use I2C to communicate, 2 pins are required to interface.
 *  For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4.
 *
 *  BSD license, all text above must be included in any redistribution
 */

#ifndef _MotorDriver_PCA9685_H
#define _MotorDriver_PCA9685_H

#include <Arduino.h>
#include <Wire.h>

// REGISTER ADDRESSES
#define PCA9685_MODE1 0x00      /**< Mode Register 1 */
#define PCA9685_MODE2 0x01      /**< Mode Register 2 */
#define PCA9685_SUBADR1 0x02    /**< I2C-bus subaddress 1 */
#define PCA9685_SUBADR2 0x03    /**< I2C-bus subaddress 2 */
#define PCA9685_SUBADR3 0x04    /**< I2C-bus subaddress 3 */
#define PCA9685_ALLCALLADR 0x05 /**< LED All Call I2C-bus address */
#define PCA9685_LED0_ON_L 0x06  /**< LED0 on tick, low byte*/
#define PCA9685_LED0_ON_H 0x07  /**< LED0 on tick, high byte*/
#define PCA9685_LED0_OFF_L 0x08 /**< LED0 off tick, low byte */
#define PCA9685_LED0_OFF_H 0x09 /**< LED0 off tick, high byte */
// etc all 16:  LED15_OFF_H 0x45
#define PCA9685_ALLLED_ON_L 0xFA  /**< load all the LEDn_ON registers, low */
#define PCA9685_ALLLED_ON_H 0xFB  /**< load all the LEDn_ON registers, high */
#define PCA9685_ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, low */
#define PCA9685_ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers,high */
#define PCA9685_PRESCALE 0xFE     /**< Prescaler for PWM output frequency */
#define PCA9685_TESTMODE 0xFF     /**< defines the test mode to be entered */

// MODE1 bits
#define MODE1_ALLCAL 0x01  /**< respond to LED All Call I2C-bus address */
#define MODE1_SUB3 0x02    /**< respond to I2C-bus subaddress 3 */
#define MODE1_SUB2 0x04    /**< respond to I2C-bus subaddress 2 */
#define MODE1_SUB1 0x08    /**< respond to I2C-bus subaddress 1 */
#define MODE1_SLEEP 0x10   /**< Low power mode. Oscillator off */
#define MODE1_AI 0x20      /**< Auto-Increment enabled */
#define MODE1_EXTCLK 0x40  /**< Use EXTCLK pin clock */
#define MODE1_RESTART 0x80 /**< Restart enabled */
// MODE2 bits
#define MODE2_OUTNE_0 0x01 /**< Active LOW output enable input */
#define MODE2_OUTNE_1 \
  0x02 /**< Active LOW output enable input - high impedience */
#define MODE2_OUTDRV 0x04 /**< totem pole structure vs open-drain */
#define MODE2_OCH 0x08    /**< Outputs change on ACK vs STOP */
#define MODE2_INVRT 0x10  /**< Output logic state inverted */

#define PCA9685_I2C_ADDRESS 0x40 /**< Default PCA9685 I2C Slave Address */
/*
    In theory the internal oscillator (clock) is 25MHz but it really isn't that precise.
    You can 'calibrate' this by tweaking this number until you get the PWM update frequency you're expecting!
    The int.osc. for the PCA9685 chip is a range between about 23-27MHz and is used for calculating things like writeMicroseconds()
    Analog servos run at ~50 Hz updates, It is importaint to use an oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
      1) Attach the oscilloscope to one of the PWM signal pins and ground on the I2C PCA9685 chip you are setting the value for.
      2) Adjust setOscillatorFrequency() until the PWM update frequency is the expected value (50Hz for most ESCs)
    Setting the value here is specific to each individual I2C PCA9685 chip and affects the calculations for the PWM update frequency.
    Failure to correctly set the int.osc value will cause unexpected PWM results

    理论上，内部振荡器（时钟）是25MHz，但实际上不是那么精确。
    你可以通过调整这个数字来“校准”这个数字，直到你得到你期望的PWM更新频率！
    这个内景osc. 对于PCA9685芯片来说，它的范围在23-27MHz之间，用于计算writeMiroseconds（）
    模拟伺服运行在~50赫兹更新，这是很重要的使用示波器设置内景oscI2C PCA9685芯片的频率。
      1） 将示波器连接到其中一个PWM信号引脚上，并在I2C PCA9685芯片上接地。
      2） 调整setOscillatorFrequency()，直到PWM更新频率为预期值（大多数ESC为50赫兹）
    此处的值设置是针对每个I2C PCA9685芯片的，它会影响PWM更新频率的计算。
    未能正确设置内景osc值将导致意外的PWM结果
*/
#define FREQUENCY_OSCILLATOR 25000000 /**< Int. osc. frequency in datasheet */

#define PCA9685_PRESCALE_MIN 3   /**< minimum prescale value */
#define PCA9685_PRESCALE_MAX 255 /**< maximum prescale value */

#define M1 1
#define M2 2
#define M3 3
#define M4 4
#define MAll 5

/*!
 *  @brief  Class that stores state and functions for interacting with PCA9685
 * PWM chip
 */
class MotorDriver_PCA9685 {
 public:
  MotorDriver_PCA9685();
  MotorDriver_PCA9685(const uint8_t addr);
  MotorDriver_PCA9685(const uint8_t addr, TwoWire &i2c);
  void begin(uint8_t prescale = 0);
  void reset();
  void sleep();
  void wakeup();
  void setExtClk(uint8_t prescale);
  void setPWMFreq(float freq);
  void setOutputMode(bool totempole);
  uint8_t getPWM(uint8_t num);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  void setPin(uint8_t num, uint16_t val, bool invert = false);
  uint8_t readPrescale(void);
  void writeMicroseconds(uint8_t num, uint16_t Microseconds);

  void setOscillatorFrequency(uint32_t freq);
  uint32_t getOscillatorFrequency(void);

  void setSingleMotor(int8_t motorNum, int16_t speed);  // 驱动单个电机
  void setMotor(int16_t speedM1, int16_t speedM2, int16_t speedM3, int16_t speedM4);   // 驱动电机
  void setMotor(int16_t speedall);  // 相同速度驱动所有电机
  void stopMotor(int8_t motorNum);  // 刹车
  void setMotorDirReverse(bool m1Dir, bool m2Dir, bool m3Dir, bool m4Dir);
  void setMotorDirReverse(bool MAllDir);

 private:
  uint8_t _i2caddr;
  TwoWire *_i2c;

  bool _inited = false;  // 是否已经初始化引脚
  uint8_t _M1IN1;        // 电机M1 输入1
  uint8_t _M1IN2;        // 电机M1 输入2
  uint8_t _M1PWM;        // 电机M1 PWM
  uint8_t _M2IN1;        // 电机M2 输入1
  uint8_t _M2IN2;        // 电机M2 输入2
  uint8_t _M2PWM;        // 电机M2 PWM
  uint8_t _M3IN1;        // 电机M3 输入1
  uint8_t _M3IN2;        // 电机M3 输入2
  uint8_t _M3PWM;        // 电机M3 PWM
  uint8_t _M4IN1;        // 电机M4 输入1
  uint8_t _M4IN2;        // 电机M4 输入2
  uint8_t _M4PWM;        // 电机M4 PWM

  bool _MOTORM1REVERSE = 0;   /** motor M1 reverse 电机M1反向 **/
  bool _MOTORM2REVERSE = 0;   /** motor M2 reverse 电机M2反向 **/
  bool _MOTORM3REVERSE = 0;   /** motor M3 reverse 电机M3反向 **/
  bool _MOTORM4REVERSE = 0;   /** motor M4 reverse 电机M4反向 **/
  bool _MOTORMALLREVERSE = 0; /** all motor reverse 所有电机反向 **/

  uint32_t _oscillator_freq;
  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
  void initPin();
};

#endif