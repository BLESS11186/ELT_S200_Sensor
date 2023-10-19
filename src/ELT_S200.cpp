#include "ELT_S200.h"
#ifdef S200_DEBUG
    #define OBJ_DEBUG(s) {Serial.print(__FUNCTION__); Serial.print(": "); Serial.println(s);}
#else
    #define OBJ_DEBUG(s) {}
#endif

bool S200::begin(uint8_t address=S200_I2C, TwoWire &wirePort=Wire){
    _address = address;
    _i2cPort = wirePort;
    _err = 0;
    _i2cPort.begin();
    OBJ_DEBUG("begin S200")
    return connected();
}

uint16_t S200::get_co2(){
    uint8_t buf[7];
    bool succ = true;
    succ &= _send(S200_READ_CMD);
    if(!succ){
        OBJ_DEBUG("Send data error occurs")
    }
    succ &= _recv(buf, 7);
    if(!succ){
        OBJ_DEBUG("Error occurs")
        return 0;
    }
    ppm =( buf[1] << 8) | buf[2];
    return ppm;
}

void S200::sleep(){
    _send(S200_SLEEP_CMD);
}
void S200::wakeup(){
    _send(S200_WAKEUP_CMD);
}
void S200::MCDL(){
    _send(S200_MCDL_CMD);
}
void S200::ACDL(){
    _send(S200_ACDL_CMD);
}
void S200::calibration_end(){
    _send(S200_SLEEP_CMD);
}
void S200::clear(){
    _send(S200_CLEAR_CMD);
}
void S200::uart_rate(uint16_t baud){
    if(baud == 9600 || baud == 19200 || baud == 38400){
        uint8_t buf[3];
        buf[0] = S200_UART_RATE_CHANGE_CMD;
        buf[1] = baud >> 8;
        buf[2] = baud & 0xff;
        _send(buf, 3);
    }
}
void S200::target_cal(uint16_t curr, uint16_t target){
    uint8_t buf[5];
    buf[0] = S200_TARGET_CAL_CMD;
    buf[1] = curr >> 8;
    buf[2] = curr & 0xff;
    buf[3] = target >> 8;
    buf[4] = target & 0xff;
    _send(buf, 5);
}

uint8_t S200::get_recal_stat(){
    uint8_t buf;
    bool succ = true;
    succ &= _send(S200_GET_RECAL_STAT_CMD);
    succ &= _recv(&buf, 1);
    if(!succ)
        return 0xff;
    return buf;
}

/* Cannot check Arduino is connected to S200.
 * Just only given address is valid and connected to i2c driver
 */
bool S200::connected(){
    _i2cPort.beginTransmission(_address);
    _err = _i2cPort.endTransmission();
    OBJ_DEBUG("connection check")
    OBJ_DEBUG(_err)
    return _err == 0;
}

bool S200::_send(uint8_t cmd){
    _i2cPort.beginTransmission(_address);
    _i2cPort.write(cmd);
    _err = _i2cPort.endTransmission();
    if(_err){
        OBJ_DEBUG("Send Error Flag")
        OBJ_DEBUG(_err)
    }
    return _err == 0;
}

bool S200::_send(const uint8_t *buf, uint8_t len){
    _i2cPort.beginTransmission(_address);
    _i2cPort.write(buf, len);
    _err = _i2cPort.endTransmission();
    if(_err){
        OBJ_DEBUG("Send Error Flag")
        OBJ_DEBUG(_err)
    }
    return _err == 0;
}

bool S200::_recv(uint8_t *buf, uint8_t len){
    _i2cPort.requestFrom(_address, len);
    int idx = 0;
    
    while(_i2cPort.available() > 0){
        buf[idx++] = _i2cPort.read();
        if(idx == len)
            break;
    }

    for(int i = idx; i<len; i++)
        buf[i] = 0xff;
    
    if(idx == 0)
        OBJ_DEBUG("No available recieve data")
    else if(idx != len)
        OBJ_DEBUG("Recieved data length is short")
    return idx == len;
}