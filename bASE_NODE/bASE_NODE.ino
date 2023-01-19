#include <Wire.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h> 
#include <ESP8266HTTPClient.h>
//#include <ESP8266HttpClient.h>

#include <WiFiManager.h>
#include <SPI.h>
#include <Wire.h> 
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>


//----------------------------------------Thinger IO configuration

#define DEVICE_CREDENTIAL "_LF85$uTF4eVo%hO"       //
//#define DEVICE_CREDENTIAL "EF4vhiAP$@dprcr7" 
#define USERNAME "GershBlocks"                                  //
#define DEVICE_ID "BaseNodeWSN"  
//----------------------------------------------------------------

//-------------------------------------------------------------//
//          SSID and Password of your WiFi Router/Hotspot      //
//-------------------------------------------------------------//

//          #define ssid "LORDS" //--> Your wifi name or SSID.
//          #define password "glory1234" //--> Your wifi password.


#define ssid "Bolaji B" //--> Your wifi name or SSID.//.....................................
#define password "password777" //--> Your wifi password.
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);  //--> Initialize Thinger IO (ThingerESP8266)

RF24 receive(D2, D4); 

const uint64_t address[4] = {001,010,011,100}; 

typedef struct package // structure to keep temporary data from the network.
  {
    float temperature = 0;
    float humidity = 0;
    int node_ID =0;
    float BatVolt;
  } Package;

Package data;

float temp1,temp2,temp3,temp4;
//float hum5=78;
float hum1,hum2,hum3,hum4;
float battery1,battery2,battery3,battery4;

void setup(){
  SPI.begin();
  receive.begin();
  receive.openReadingPipe(1,address[0]);    
  receive.openReadingPipe(2,address[1]);
  receive.openReadingPipe(3,address[2]); 
  receive.openReadingPipe(4,address[3]); 
  receive.setPALevel(RF24_PA_MAX);       
  receive.setDataRate(RF24_250KBPS);     
  receive.setChannel(100);               
  receive.startListening();  

    Serial.begin(9600);
//    nodemcu.begin(9600);
   // while (!Serial) continue;
    thing.add_wifi(ssid, password);
    WiFi.begin(ssid, password); //--> Connect to your WiFi router
    pinMode(LED_BUILTIN, OUTPUT);
    //----------------------------------------Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    //----------------------------------------
  }
     digitalWrite(LED_BUILTIN, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.

  //----------------------------------------Sends Sensor data (Temperature and Humidity) to Thinger IO
  // Symbol or operator ">>" means to transmit data
  thing["WSN NODE 1"] >> [](pson& out){
      out["Node 1 Temperature"] = temp1;
      out["Node 1 Humidity"] = hum1;
  };

  thing["WSN NODE 2"] >> [](pson& out){
      out["Node 2 Temperature"] = temp2;
      out["Node 2 Humidity"] = hum2;
  };
  
  thing["WSN NODE 3"] >> [](pson& out){
      out["Node 3 Temperature"] = temp3;
      out["Node 3 Humidity"] = hum3;
  };
  }

  


  void getRadioPacket()// function to extract incoming data from network.
{
  if (receive.available())                //check when received data available
  {
    receive.read(&data, sizeof(data));
    if(data.node_ID==1)
    {
      temp1 = data.temperature;
      hum1 = data.humidity;
      battery1 = data.BatVolt;
      }
      else if(data.node_ID==2)
      {
        temp2 = data.temperature;
        hum2 = data.humidity;
        battery2 = data.BatVolt;
        }
        else if(data.node_ID==3)
        {
          temp3 = data.temperature;
          hum3 = data.humidity;
          battery3 = data.BatVolt;
          }
          else 
          {
            temp4 = data.temperature;
            hum4 = data.humidity;
            battery4 = data.BatVolt;
            }
   Serial.print("DATA from node::");
   Serial.println(data.node_ID);
   Serial.print("Temperature :");
   Serial.print(data.temperature);
   //Serial.print(temp1);
   Serial.println("C");
   Serial.print("Humidity :");
   Serial.print(data.humidity);
   Serial.println("%");
   Serial.print("Battery Voltate :");
   Serial.print(data.BatVolt);
   Serial.println("%");
   delay(10);
  }
  }

  void loop() {
    getRadioPacket();
    thing.handle();
                  
   
}
