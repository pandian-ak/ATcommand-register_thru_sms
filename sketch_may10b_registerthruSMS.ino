#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM868
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_M95
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_MC60
// #define TINY_GSM_MODEM_MC60E
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

#define TINY_GSM_DEBUG SerialMon

#include <TinyGsmClient.h>
char SMS_TARGET[]=  "+919841214977";
// Module baud rate
uint32_t rate = 0; // Set to 0 for Auto-Detect

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(6000);
  if (!rate) {
    rate = TinyGsmAutoBaud(SerialAT);
  }

  SerialAT.begin(rate);
  delay(1000);
  SerialAT.write("AT+CMGF=1\r\n");
  updateSerial();
  SerialAT.write("AT+CNMI=2,2,0,0,0\r\n");
  updateSerial();
  delay(9000); //delay for Call and SMS ready
  updateSerial();
  //delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
    char dataa[100],dataa2[100],dataa3[100];
    for (int k=0; k<100;k++)
    {
      dataa[k]=0;
      dataa2[k]=0;
      dataa3[k]=0;
    }
    int pos = -1;int i= -1, j=-1;
    while (SerialAT.available() && (pos<100)) {
      pos++;
      dataa[pos] = SerialAT.read();
      if((pos>8 )&& (pos<22))
        dataa2[++i] = dataa[pos];        
      if(pos>50)
        dataa3[++j] = dataa[pos];
      delay(100);      
    }
    if ((pos != -1))
    {
          if(strcmp(dataa3,"Add me") ==0);
          {
            strcpy(SMS_TARGET, dataa2);
            delay(100);
          }
          if(strcmp(dataa2,SMS_TARGET) ==0);
          {
            SerialMon.println("Registered mobile device: "); //Process hardware (or) turn ON Led.
            delay(100);
            for(i=0;dataa2[i]!=0;i++)
            {
              SerialMon.write(dataa2[i]);
            }
            SerialMon.write("\r\n");
            SerialMon.println("Message received: ");
            delay(100);
            for(i=0;dataa3[i]!=0;i++)
            {
              SerialMon.write(dataa3[i]);
            }
            SerialMon.write("\r\n");
            delay(1000);
          }
          
    }
}
void updateSerial()
{
  delay(500);
  while (SerialMon.available()) 
  {
    SerialAT.write(SerialMon.read());//Forward what Serial received to Software Serial Port
  }
  while(SerialAT.available()) 
  {
    SerialMon.write(SerialAT.read());//Forward what Software Serial received to Serial Port
  }
}
