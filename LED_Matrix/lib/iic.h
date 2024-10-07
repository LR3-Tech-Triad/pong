#ifndef IIC_PROTOCOL
#define IIC_PROTOCOL

void IIC_start(int screenNumber);
void IIC_send(unsigned char send_data, int screenNumber);
void IIC_end(int screenNumber);

#endif