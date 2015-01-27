#include <GSM.h>
#include <GSM3CircularBuffer.h>
#include <GSM3MobileAccessProvider.h>
#include <GSM3MobileCellManagement.h>
#include <GSM3MobileClientProvider.h>
#include <GSM3MobileClientService.h>
#include <GSM3MobileDataNetworkProvider.h>
#include <GSM3MobileMockupProvider.h>
#include <GSM3MobileNetworkProvider.h>
#include <GSM3MobileNetworkRegistry.h>
#include <GSM3MobileServerProvider.h>
#include <GSM3MobileServerService.h>
#include <GSM3MobileSMSProvider.h>
#include <GSM3MobileVoiceProvider.h>
#include <GSM3ShieldV1.h>
#include <GSM3ShieldV1AccessProvider.h>
#include <GSM3ShieldV1BandManagement.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <GSM3ShieldV1CellManagement.h>
#include <GSM3ShieldV1ClientProvider.h>
#include <GSM3ShieldV1DataNetworkProvider.h>
#include <GSM3ShieldV1DirectModemProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <GSM3ShieldV1ModemVerification.h>
#include <GSM3ShieldV1MultiClientProvider.h>
#include <GSM3ShieldV1MultiServerProvider.h>
#include <GSM3ShieldV1PinManagement.h>
#include <GSM3ShieldV1ScanNetworks.h>
#include <GSM3ShieldV1ServerProvider.h>
#include <GSM3ShieldV1SMSProvider.h>
#include <GSM3ShieldV1VoiceProvider.h>
#include <GSM3SMSService.h>
#include <GSM3SoftSerial.h>
#include <GSM3VoiceCallService.h>

/*
 SMS sender
 
 This sketch, for the Arduino GSM shield,sends an SMS message 
 you enter in the serial monitor. Connect your Arduino with the 
 GSM shield and SIM card, open the serial monitor, and wait for 
 the "READY" message to appear in the monitor. Next, type a 
 message to send and press "return". Make sure the serial 
 monitor is set to send a newline when you press return.
 
 Circuit:
 * GSM shield 
 * SIM card that can send SMS
 
 created 25 Feb 2012
 by Tom Igoe
 
 This example is in the public domain.
 
 http://arduino.cc/en/Tutorial/GSMExamplesSendSMS
 
 */

// Include the GSM library
//#include <GSM.h>

#define PINNUMBER ""/*PIN*/
const int sensorMov = A0;
const int sensorGas = A1;
const int pin_start = 4;
const int pin_led_gsm = 5;
int sensrValue; /* value reading of'ADC */
int start = false;
int button_read;
float Gas_value;
float Mov_value;
const char txtMsgGas[200] = "GAS ALARM";
const char txtMsgMov[200]= "THIEF ALARM";
char remoteNum[20] = "";  // telephone number to send sms
char txtMsg[200];
// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

void setup()
{
   pinMode(sensorGas,INPUT);
   pinMode(sensorMov,INPUT);
   pinMode(pin_start,INPUT);
   pinMode(pin_led_gsm,OUTPUT);
   digitalWrite (pin_led_gsm, HIGH); //Led gsm blu
  // critical, time-sensitive code here
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  
  Serial.println("SMS Messages Sender");

  // connection state
  boolean notConnected = true;
 
  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  
  Serial.println("GSM initialized");
  digitalWrite (pin_led_gsm, LOW);
  
}

void loop()
{
  int i = 0;
  button_read = digitalRead(pin_start);
  if (button_read == HIGH)
  {
    StartSensor();
  }
  while (start)
  {
    sensrValue = analogRead(sensorGas);
    Gas_value = AnalgConv();
    sensrValue = analogRead(sensorMov);
    Mov_value = AnalgConv();
    Serial.print ("\n\rValore GAS: ");
    Serial.print (Gas_value);
    Serial.print ("\tValore MOV: ");
    Serial.print (Mov_value);
    Serial.flush();
    if (1 >= Gas_value)
    {

      for (i = 0; i<sizeof(txtMsg); i++)
      {
        txtMsg[i] = txtMsgGas[i];
      }
      SendSMS();  
    }
    delay(100);
    if(1 <= Mov_value)
    {
      for (i = 0; i<sizeof(txtMsg); i++)
      {
        txtMsg[i] = txtMsgMov[i];
      }
      SendSMS();   
    }
  
    delay(5000);
  }
}

float AnalgConv (void)
{
  float analog_value;
  analog_value = (sensrValue/1024.0)*5.0; /*prendo il valore campionato, lo divido per il valore massimo e lo moltiplico per la Vc = 5V*/
  return analog_value;
}

void SendSMS()
{
    Serial.print("\nMobile number: ");
    Serial.println(remoteNum);    
    // sms text
    Serial.print("Now, enter SMS content: ");
    //readSerial(txtMsg);
    Serial.println("SENDING");
    Serial.println();
    Serial.println("Message:");
    Serial.println(txtMsg);
    
    // send the message
    sms.beginSMS(remoteNum);
    sms.print(txtMsg);
    sms.endSMS(); 
    Serial.println("\nCOMPLETE!\n");
    start = false;
}
  void StartSensor(void)
{
  start = !start;
  if (start == true)
  {
    Serial.println("\n\rStart Sensor\n\r");
  }
  else
  {
    Serial.println("\n\rStop Sensor\n\r");
  }

  delay(5000);
}

