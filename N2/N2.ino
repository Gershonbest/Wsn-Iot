#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 transmit(8, 9); // CE, CSN
const uint64_t address[4] = {001,010,011,100};// Logical pipe address
#include <DHT.h>

#define DHTPIN 2                // do not connect to pin 0 or pin 1
#define DHTTYPE DHT11           // Define DHT11 module
DHT dht(DHTPIN, DHTTYPE);       //create DHT object called dht
const unsigned long interval = 4000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already
struct package
  {
    float temperature = 0;
    float humidity = 0;
    int node_ID = 0;
    //float BatVolt;
  };

typedef struct package Package;
Package data;
int sensorval;

void setup() {
  Serial.begin(9600);
  dht.begin();
  transmit.begin();
  transmit.openWritingPipe(address[1]);          
  transmit.setPALevel(RF24_PA_MAX);             //set RF power output to maximum
  transmit.setDataRate(RF24_250KBPS);           //set datarate to 250kbps
  transmit.setChannel(100);                     //set frequency to channel 100
  transmit.stopListening();
  }
// void ReadBatVolt(){
//  sensorval=analogRead(A0);
//  data.BatVolt=sensorval*(3.3/1023);
//  Serial.println(data.BatVolt);
//  }
void loop() {
   //ReadBatVolt();
   unsigned long now = millis();  
   if(now - last_sent >= interval)
   {
     last_sent = now;
 
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.node_ID = 2;
  transmit.write(&data, sizeof(data));
     
  //delay(3000);     
   }                            
}
