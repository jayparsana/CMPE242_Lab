#define _GNU_SOURCE
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
 
#include <wiringPi.h>
#include <wiringPiSPI.h>
 
#define	TRUE	            (1==1)
#define	FALSE	            (!TRUE)
#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0
 
static int myFd ;
 
void spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 1000000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}
 
int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}



void delayy(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

float compensate_value(void){
    float uncompensated_digital_value = 0, 
 	  compensated_digital_value = 0,
	  g_x = 0,
	  new_raw_value = 0;

    int raw_value = 0;
    raw_value = myAnalogRead(0,8,0);
    uncompensated_digital_value = ((3.3/1023)*raw_value);
    g_x = (-3.495*uncompensated_digital_value) + 1.032;
    new_raw_value = raw_value + g_x;
    compensated_digital_value = (3.3/1023)*new_raw_value;
    return compensated_digital_value;
}

int main (int agrc, char *argv[])
{
    // command line argument will accept sampling rate
    char char_sampling_rate = argv[1];
    int int_sampling_rate = char_sampling_rate - '0'; 
    wiringPiSetup();
    spiSetup(0);
    float digital_value = 0;
    int raw_value_adc = 0;
//    for(int i = 0; i < 10000; i++){
        printf("voltage = %f\n", compensate_value());
	
//	delayy(1);   
// }
    close(myFd);
    return 0;
}
