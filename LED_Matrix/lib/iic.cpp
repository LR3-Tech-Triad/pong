#include "iic.h"

void IIC_start(int screenNumber) {
  digitalWrite(IIC_SCL[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], LOW);
  delayMicroseconds(3);
}
void IIC_send(unsigned char send_data, int screenNumber) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(IIC_SCL[screenNumber], LOW);
    delayMicroseconds(3);
    if (send_data & 0x01) {
      digitalWrite(IIC_SDA[screenNumber], HIGH);
    } else {
      digitalWrite(IIC_SDA[screenNumber], LOW);
    }
    delayMicroseconds(3);
    digitalWrite(IIC_SCL[screenNumber], HIGH);
    delayMicroseconds(3);
    send_data = send_data >> 1;
  }
}
void IIC_end(int screenNumber) {
  digitalWrite(IIC_SCL[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], HIGH);
  delayMicroseconds(3);
}