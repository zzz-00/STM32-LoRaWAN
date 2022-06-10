#ifndef __FUNC_H__
#define __FUNC_H__

typedef struct
{
    int rssi;
    int snr;
    char success_flag;
} recv_data;

void Device_Config(void);
int my_atoi(unsigned char *);

#endif
