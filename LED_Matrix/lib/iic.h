#ifndef IIC_PROTOCOL
#define IIC_PROTOCOL

int IIC_SCL[2] = {A2, A4};
int IIC_SDA[2] = {A1, A3};

void IIC_start(int screenNumber);
void IIC_send(unsigned char send_data, int screenNumber);
void IIC_end(int screenNumber);

#endif