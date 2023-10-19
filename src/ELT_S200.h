#ifndef ELT_S200
#define ELT_S200

#include <Arduino.h>
#include <Wire.h>

#define S200_I2C                    0x31
#define S200_READ_CMD               0x52 // 'R'
#define S200_SLEEP_CMD              0x53 // 'S'
#define S200_WAKEUP_CMD             0x57 // 'W'
#define S200_MCDL_CMD               0x4D // 'M' Manual calibration
#define S200_END_CMD                0x45 // 'E' End Calibration
#define S200_ACDL_CMD               0x41 // 'A' Automatic claibration
#define S200_CLEAR_CMD              0x43 // 'C' Clear recalibration
#define S200_UART_RATE_CHANGE_CMD   0x55 // 'U'
#define S200_TARGET_CAL_CMD         0x54 // 'T' Manual traget calibration
#define S200_GET_RECAL_STAT_CMD     0x4F // 'O' Get recalibrwation status
//#define S200_DEBUG

#define HW_MCDL_BIT 0b1000
#define HW_ACDL_BIT 0b0100
#define SW_MCDL_BIT 0b0010
#define SW_ACDL_BIT 0b0001
#define STAT_ERROR  0xFF

class S200
{
public:
  uint16_t ppm;

  bool begin(uint8_t address=S200_I2C, TwoWire &wirePort=Wire);
  uint16_t get_co2();
  void sleep();
  void wakeup();
  void MCDL();
  void ACDL();
  void calibration_end();
  void clear();
  void uart_rate(uint16_t baud);
  void target_cal(uint16_t curr, uint16_t target);
  uint8_t get_recal_stat();
  bool connected();
  uint8_t get_error(){
    return _err;
  }

private:
  uint8_t _address;
  TwoWire &_i2cPort = Wire;
  uint8_t _err;
  bool _send(uint8_t cmd);
  bool _send(const uint8_t *buf, uint8_t len);
  bool _recv(uint8_t *buf, uint8_t len);
};

#endif // ifndef ELT_S200
