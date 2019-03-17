//minima a maxima
  bool stopmM = true;
float m_Tvenku1 = 10000;
float M_Tvenku1=-1000;
float m_Hvenku1 =10000;
float M_Hvenku1 =-1000;
float m_Tvenku2 =10000;
float M_Tvenku2 =-1000;
float m_Pvenku2 =100000;
float M_Pvenku2 =0;
float m_Tkoupelna =10000;
float M_Tkoupelna =0;
float m_Hkoupelna =10000;
float M_Hkoupelna =0;
float m_Ttechnicka =10000;
float M_Ttechnicka =0;
float m_Htechnicka =1000;
float M_Htechnicka =0;
float m_Tzakrbem =10000;
float M_Tzakrbem =0;
float m_Hzakrbem =10000;
float M_Hzakrbem =0;
float m_Tspaliny =10000;
float M_Tspaliny =0;
float m_T1 =10000;
float M_T1 =0;
float m_T2 =10000;
float M_T2 =0;
float m_T3 =10000;
float M_T3 =0;
float m_T4 =10000;
float M_T4 =0;
float m_T5 =10000;
float M_T5 =0;
float m_T6 =10000;
float M_T6 =0;
float m_T7 =1000;
float M_T7 =0;
float m_T8 =1000;
float M_T8 =0;
float m_T9 =1000;
float M_T9 =0;
float m_T10 =1000;
float M_T10 =0;
float m_T11 =1000;
float M_T11 =1;
float m_T12 =1000;
float M_T12 =0;
float m_T13 =100;
float M_T13 =0;
float m_T14 =100;
float M_T14 =0;
float m_Tobyvak =100;
float M_Tobyvak =0;
float m_Tloznice =100;
float M_Tloznice =0;
float m_Tpokoj =100;
float M_Tpokoj =0;
float m_Tpracovna =100;
float M_Tpracovna =0;
int m_pct_naplneny_zas =100;
int M_pct_naplneny_zas =0;
float m_kWh_zas = 500;
float M_kWh_zas = -500;


int pct_naplneny_zas =0;
float kWh_zas = 0;


//ovladaní topení
float teplota_zadana_H = 22.2;
float teplota_zadana_D = 21.2;

bool pozadavek_topit= false;
bool pozadavek_kotel = false;
bool pozadavek_zasobnik = false;
bool topit_pokoj = false;
bool topit_obyvak = false;
bool topit_loznice = false;
bool topit_pracovna = false;

int pin_kotel =3;
int pin_krb = 2;
int pin_obyvak = 4;
int pin_loznice = 5;
int pin_pracovna = 6;





//ČOV
int stav_COV=0;
int pin_COV = 2;
long loopOn = 2700000; // time for RED
long loopOff = 7200000; // time for BLUE + YELLOW
long subLoopOn = 1200000; // time for YELLOW
long subLoopOff = 5000; // time for BLUE


bool stateMainLoop = false;
bool stateSubLoop = false;

unsigned long lastTimeMainLoop = 0;
unsigned long lastTimeSubLoop = 0;


#include <Arduino.h>

// teploměra DS1820
#include <OneWire.h>
#include <DallasTemperature.h>


float T1 = 0; // voda DO VLOŽKY
float T2 = 0; // zásobník 2/4
float T3 = 0; // kotel vstup
float T4 = 0; // kotel vystup
float T5 = 0; // podlaha obývák
float T6 = 0; // zásobník 1/4
float T7 = 0; // zásobník 4/4
float T8 = 0; //krbová vložka
float T9 = 0; //ČOV voda
float T10 = 0; //vodoměr
float T11 = 0; // zásobník2/4
float T12 = 0; // ze zásobníku do topená
float T13 = 0; // podlaha pokoj nahoře
float T14 = 0; // ČOV vzduch
float Tobyvak = 0;
float Tloznice =0;
float Tpokoj = 0;
float Tpracovna =0;


// MAX 6675
#include "max6675.h"




//MPL311
#include <Wire.h>
#include "i2c.h"

#include "i2c_MPL3115A2.h"
MPL3115A2 mpl3115;






#include "DHT.h"
 
#define DHT2PIN 38    // what pin we're connected to
#define DHT3PIN 39
#define DHT4PIN 40     // what pin we're connected to
#define DHT5PIN 41
 
// Uncomment whatever type you're using!
#define DHT2TYPE DHT11   // DHT 11 / 22 / 21
#define DHT3TYPE DHT11   
#define DHT4TYPE DHT22   
#define DHT5TYPE DHT22  
 
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
 

DHT dht2(DHT2PIN, DHT2TYPE);
DHT dht3(DHT3PIN, DHT3TYPE);
DHT dht4(DHT4PIN, DHT4TYPE);
DHT dht5(DHT5PIN, DHT5TYPE);



// Ethernet

// For W5100 based ethernet modules
#include <SPI.h>     
#include <Ethernet.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);



// proměnné

float Tvenku1 =0;
float Hvenku1 =0;
float Ttechnicka = 0;
float Htechnicka =0;
float Tkoupelna = 0;
float Hkoupelna =0;
float Tzakrbem = 0;
float Hzakrbem =0;
float Tvenku2 = 0;
float Pvenku2 = 0;

float Tspaliny=0;

float Tzasobnik1 = 0;
float Tzasobnik3 = 0;
float Tzasobnik4=0;


class ctiDHT
{
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
public: 
  ctiDHT(int interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float h3 = dht3.readHumidity();
  float t3 = dht3.readTemperature();
  float h4 = dht4.readHumidity();
  float t4 = dht4.readTemperature();
  float h5 = dht5.readHumidity();
  float t5 = dht5.readTemperature();
 
  // check if returns are valid, if they are NaN (not a number) then something went wrong!

  if (isnan(t2) || isnan(h2)) {
    Serial.println("Failed to read from DHT #2");
  } else {
    Serial.print("Humidity 2: "); 
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("Temperature 2: "); 
    Serial.print(t2);
    Serial.println(" *C");
  }
  Serial.println();
    if (isnan(t3) || isnan(h3)) {
    Serial.println("Failed to read from DHT #3");
  } else {
    Serial.print("Humidity 3: "); 
    Serial.print(h3);
    Serial.print(" %\t");
    Serial.print("Temperature 3: "); 
    Serial.print(t3);
    Serial.println(" *C");
  }
  Serial.println();
    if (isnan(t4) || isnan(h4)) {
    Serial.println("Failed to read from DHT #4");
  } else {
    Serial.print("Humidity 4: "); 
    Serial.print(h4);
    Serial.print(" %\t");
    Serial.print("Temperature 4: "); 
    Serial.print(t4);
    Serial.println(" *C");
  }
  Serial.println();
    if (isnan(t5) || isnan(h5)) {
    Serial.println("Failed to read from DHT #5");
  } else {
    Serial.print("Humidity 5: "); 
    Serial.print(h5);
    Serial.print(" %\t");
    Serial.print("Temperature 5: "); 
    Serial.print(t5);
    Serial.println(" *C");
  }
  Serial.println();
 
Tvenku1 = t5;
Hvenku1 = h5;
Ttechnicka = t3;
Htechnicka=h3;
Tkoupelna =t4;
Hkoupelna = h4;
Tzakrbem = t2;
Hzakrbem = h2;


  
  
}

    }

};



 

class ctiDS
{
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  ctiDS(int interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
  

  T1=teplota(29);
  Serial.print("voda dovložky ");
  Serial.println(T1);

  T2=teplota(33);
Serial.print("zásobník 3/4 ");
Serial.println(T2);

  T3=teplota(25);
Serial.print("kotel vstup ");
Serial.println(T3);

T4=teplota(26);
Serial.print("kotel vystup ");
Serial.println(T4);

T5=teplota(22);
Serial.print("podlaha obyvak ");
Serial.println(T5);

T6=teplota(31);
Serial.print("zasobnik 1/4 ");
Serial.println(T6);

T7=teplota(32);
Serial.print("zasobnik 4/4 ");
Serial.println(T7);

T8=teplota(28);
Serial.print("krbová bložka ");
Serial.println(T8);

T9=teplota(35);
Serial.print("voda ČOV ");
Serial.println(T9);

  
T10=teplota(36);
Serial.print("vodomer ");
Serial.println(T10);  

T11=teplota(30);
Serial.print("zasobnik 2/4 ");
Serial.println(T11);

T12=teplota(27);
Serial.print("ze zasobniku do topeni ");
Serial.println(T12);

T13=teplota(24);
Serial.print("pokoj nahoře ");
Serial.println(T12);

T14=teplota(34);
Serial.print("ČOV vzduch ");
Serial.println(T12);

Tobyvak=teplota(49);
Serial.print("obývák (49)");
Serial.println(Tobyvak);

Tloznice=teplota(5);
Serial.print("ložnice (5)");
Serial.println(Tloznice);

Tpokoj=teplota(23);
Serial.print("pokoj (23)");
Serial.println(Tpokoj);
/*
Tpracovna=teplota(48);
Serial.print("pracovna (48)");
Serial.println(Tpracovna); 

*/
    }
  }
  // teplota z DS18B20 - ty na destičce i ty vodotěsné
float teplota(int sensor_pin) {

    //returns the temperature from one DS18S20 in DEG Celsius
      OneWire ds(sensor_pin);
      byte data[12];
      byte addr[8];
        if ( !ds.search(addr)) {
         //no more sensors on chain, reset search
          ds.reset_search();
          return -1001;
          }
        if ( OneWire::crc8( addr, 7) != addr[7]) {
          Serial.println("CRC is not valid!");
          return -1002;
          }
        if ( addr[0] != 0x10 && addr[0] != 0x28) {
          Serial.print("Device is notrecognized");
          return -1003;
          }
       ds.reset();
       ds.select(addr);
       ds.write(0x44,1); // start conversion, with parasite power on at the end
       byte present = ds.reset();
       ds.select(addr);   
       ds.write(0xBE); // Read Scratchpad
        for (int i = 0; i < 9; i++) { // we need 9 bytes
          data[i] = ds.read();
        }
       ds.reset_search();
      byte MSB = data[1];
      byte LSB = data[0];
      float tempRead = ((MSB << 8) | LSB); //using two's compliment
      float TemperatureSum = tempRead / 16;
      return TemperatureSum;
}

};

class smazmM
{
  unsigned long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  smazmM(unsigned long interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
 m_Tvenku1 = 10000;
 M_Tvenku1=-1000;
 m_Hvenku1 =10000;
 M_Hvenku1 =-1000;
 m_Tvenku2 =10000;
 M_Tvenku2 =-1000;
 m_Pvenku2 =100000;
 M_Pvenku2 =0;
 m_Tkoupelna =10000;
 M_Tkoupelna =0;
 m_Hkoupelna =10000;
 M_Hkoupelna =0;
 m_Ttechnicka =10000;
 M_Ttechnicka =0;
 m_Htechnicka =1000;
 M_Htechnicka =0;
 m_Tzakrbem =10000;
 M_Tzakrbem =0;
 m_Hzakrbem =10000;
 M_Hzakrbem =0;
 m_Tspaliny =10000;
 M_Tspaliny =0;
 m_T1 =10000;
 M_T1 =0;
 m_T2 =10000;
 M_T2 =0;
 m_T3 =10000;
 M_T3 =0;
 m_T4 =10000;
 M_T4 =0;
 m_T5 =10000;
 M_T5 =0;
 m_T6 =10000;
 M_T6 =0;
 m_T7 =1000;
 M_T7 =0;
 m_T8 =1000;
 M_T8 =0;
 m_T9 =1000;
 M_T9 =0;
 m_T10 =1000;
 M_T10 =0;
 m_T11 =1000;
 M_T11 =1;
 m_T12 =1000;
 M_T12 =0;
 m_T13 =100;
 M_T13 =0;
 m_T14 =100;
 M_T14 =0;
m_pct_naplneny_zas =100;
M_pct_naplneny_zas =0;
 m_kWh_zas = 500;
 M_kWh_zas = -500;   
m_Tobyvak =100;
 M_Tobyvak =0;
 m_Tloznice =100;
 M_Tloznice =0;
 m_Tpokoj =100;
 M_Tpokoj =0;
 m_Tpracovna =100;
 M_Tpracovna =0;

    }
  }
};


class smazmM_jednou
{
  unsigned long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position

 
 public: 
  smazmM_jednou(unsigned long interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if(((millis() - lastUpdate) > updateInterval)&& stopmM)  // time to update
    {
      lastUpdate = millis();
      stopmM=false;
 m_Tvenku1 = 10000;
 M_Tvenku1=-1000;
 m_Hvenku1 =10000;
 M_Hvenku1 =-1000;
 m_Tvenku2 =10000;
 M_Tvenku2 =-1000;
 m_Pvenku2 =100000;
 M_Pvenku2 =0;
 m_Tkoupelna =10000;
 M_Tkoupelna =0;
 m_Hkoupelna =10000;
 M_Hkoupelna =0;
 m_Ttechnicka =10000;
 M_Ttechnicka =0;
 m_Htechnicka =1000;
 M_Htechnicka =0;
 m_Tzakrbem =10000;
 M_Tzakrbem =0;
 m_Hzakrbem =10000;
 M_Hzakrbem =0;
 m_Tspaliny =10000;
 M_Tspaliny =0;
 m_T1 =10000;
 M_T1 =0;
 m_T2 =10000;
 M_T2 =0;
 m_T3 =10000;
 M_T3 =0;
 m_T4 =10000;
 M_T4 =0;
 m_T5 =10000;
 M_T5 =0;
 m_T6 =10000;
 M_T6 =0;
 m_T7 =1000;
 M_T7 =0;
 m_T8 =1000;
 M_T8 =0;
 m_T9 =1000;
 M_T9 =0;
 m_T10 =1000;
 M_T10 =0;
 m_T11 =1000;
 M_T11 =1;
 m_T12 =1000;
 M_T12 =0;
 m_T13 =100;
 M_T13 =0;
 m_T14 =100;
 M_T14 =0;
m_pct_naplneny_zas =100;
M_pct_naplneny_zas =0;
 m_kWh_zas = 500;
 M_kWh_zas = -500; 
 m_Tobyvak =100;
 M_Tobyvak =0;
 m_Tloznice =100;
 M_Tloznice =0;
 m_Tpokoj =100;
 M_Tpokoj =0;
 m_Tpracovna =100;
 M_Tpracovna =0;  

    }
  }
};

class ctiMPL
{
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  ctiMPL(int interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
mpl3115.awaitMeasurement();

    float altitude;
    mpl3115.getAltitude(altitude);

    


    float temperature;
    mpl3115.getTemperature(temperature);
    mpl3115.triggerMeasurement();
    
    float pressure;
    pressure = 101325/(pow((((425*0.0065)/(temperature+273.15))+1),5.257));  // http://keisan.casio.com/exec/system/1224585971
    

    Serial.print(" Height: ");
    Serial.print(altitude);
    Serial.print(" Pressure: ");
    Serial.print(pressure);
    Serial.print(" Temp: ");
    Serial.print(temperature);
    Serial.println("");

Tvenku2 = temperature;
Pvenku2 = pressure;

    }
  }
};

class topeni
{
  unsigned long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  topeni(unsigned long interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();

// požadavek topit
//pokoj      
if(Tpokoj<teplota_zadana_D){
  topit_pokoj = true;
}
else if (Tpokoj>teplota_zadana_H) {
  topit_pokoj = false;
  
}
else {
  topit_pokoj = false;
}

//pracovna      
if(Tpracovna<teplota_zadana_D){
  topit_pracovna = true;
}
else if (Tpracovna>teplota_zadana_H) {
  topit_pracovna = false;
  
}
else {
  topit_pracovna = false;
}
//obyvak      
if(Tobyvak<teplota_zadana_D){
  topit_obyvak = true;
}
else if (Tobyvak>teplota_zadana_H) {
  topit_obyvak = false;
  
}
else {
  topit_obyvak = false;
}

//loznice      
if(Tloznice<teplota_zadana_D){
  topit_loznice = true;
}
else if (Tloznice>teplota_zadana_H) {
  topit_loznice = false;
  
}
else {
  topit_loznice = false;
}
      

      if (topit_pokoj ==true || topit_obyvak == true|| topit_pracovna==true|| topit_loznice ==true){
        pozadavek_topit = true;
      }
      else {
        pozadavek_topit = false;
      }
      if (T6 >35) {
        pozadavek_kotel = false;
        pozadavek_zasobnik = true;
             }
             else {
              pozadavek_kotel = true;
              pozadavek_zasobnik = false;
             }


// zapínání pinů

//zapínání zdroje
if (pozadavek_topit == true){
 if (pozadavek_zasobnik == true){
  digitalWrite (pin_krb, HIGH);
  digitalWrite (pin_kotel, LOW);
 } else  if (pozadavek_kotel == true){
  digitalWrite (pin_krb, LOW);
  digitalWrite (pin_kotel, HIGH);
 } 
}


// otevírání ventilů
if (pozadavek_topit == true){
 if (topit_obyvak == true){
  digitalWrite (pin_obyvak, HIGH);
 } 
 else {
  digitalWrite (pin_obyvak, LOW);  
 }
 if (topit_pracovna == true){
  digitalWrite (pin_pracovna, HIGH);
 } 
 else {
  digitalWrite (pin_pracovna, LOW);  
 }
 if (topit_loznice == true){
  digitalWrite (pin_loznice, HIGH);
 } 
 else {
  digitalWrite (pin_loznice, LOW);  
 }
}

// vypnutí topení
if (pozadavek_topit == false){
  digitalWrite (pin_krb, LOW);
  digitalWrite (pin_kotel, LOW);
}


    }
  }
};

class delejWEB
{
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  delejWEB(int interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
EthernetClient client = server.available();
 delay(10);
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
client.print("<html>");
client.print("<head>");
client.print("<style>");
client.print("table, th, td {    border: 1px solid black;}");
client.print("</style>");
client.print("</head>");
client.print("<body>");

client.print("<table>");
  client.print("<tr>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Cidlo</th>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Hodnota</th>");
        client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Minimum</th>");
            client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Maximum</th>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Jednotka</th>");
  client.print("</tr>");
  client.print("<tr> ");
    client.print("<td>Teplota venku 1</td>");
    if (Tvenku1>0){
           client.print( "<td>");
           client.print(Tvenku1);
          client.print(" </td>");
    }
    else{
      client.print("<td style='background-color:#00AFFF'>");
      client.print(Tvenku1);
      client.print("</td>"); //modrá
    }

        client.print("<td>");
    client.print(m_Tvenku1);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tvenku1);
    client.print("</td>");
      client.print("<td>\377C</td>");
      
  client.print("</tr>");
    client.print("<tr>"); 
    client.print("<td>Teplota venku 2</td>");
    if (Tvenku1>0){
            client.print("<td>");
            client.print(Tvenku2);
            client.print("</td>");
    }
    else{
      client.print("<td style='background-color:#00AFFF'>");
      client.print(Tvenku2);
      client.print("</td>"); //modrá
    }
    client.print("<td>");
    client.print(m_Tvenku2);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tvenku2);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
      
  client.print("</tr>");
   
    
    // vlhkost venku
    client.print("<tr>");
      client.print("<td>Vlhkost venku1</td>");
           client.print( "<td>");
           client.print(Hvenku1);
           client.print("</td>");
                          client.print("<td>");
    client.print(m_Hvenku1);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Hvenku1);
    client.print("</td>");

      client.print("<td>% rel.</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Tlak venku 2</td>");
           client.print( "<td>");
           client.print(Pvenku2);
           client.print("</td>");


               client.print("<td>");
    client.print(m_Pvenku2);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Pvenku2);
    client.print("</td>");
      client.print("<td>Pa</td>");
    client.print("</tr>");

    
    //mezera
    client.print("<tr>");
    client.print("<td> </td>");
        client.print("<td> </td>");
            client.print("<td> </td>");
                    client.print("<td> </td>");
            client.print("<td> </td>");
     client.print("</tr>");

     //spaliny
      client.print("<tr>");
        client.print("<td>Spaliny</td>");

        if (Tspaliny>380){
          client.print("<td style='background-color:#FF0000'>");
          client.print(Tspaliny);
          client.print("</td>");
        }
        else if (Tspaliny<380 && Tspaliny>280){
          client.print("<td style='background-color:#00FF00'>");
          client.print(Tspaliny);
          client.print("</td>");
        }
        else if (Tspaliny<=280 && Tspaliny>=50){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(Tspaliny);
          client.print("</td>");
        }
                else if (Tspaliny<50){
          client.print("<td>");
          client.print(Tspaliny);
          client.print("</td>");
        }
                       client.print("<td>");
    client.print(m_Tspaliny);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tspaliny);
    client.print("</td>");
 
        client.print("<td>\377C</td>");
      client.print("</tr>");

    


//zásobník


  //T6 + T2 +T11+ T7
  float prumerna_teplota_zas= ((T6 +T2+ T11 +T7)/4);
pct_naplneny_zas= (100 *((prumerna_teplota_zas-25)/35));
kWh_zas = ((prumerna_teplota_zas -25)*0.8);

  
      // zásobník naplnění %
      client.print("<tr>");
        client.print("<td>Naplneni zasobniku</td>");

        if (pct_naplneny_zas>100){
          client.print("<td style='background-color:#FF0000'>");
          client.print(pct_naplneny_zas);
          client.print("</td>");
        }
        else if (pct_naplneny_zas<=100 && pct_naplneny_zas>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(pct_naplneny_zas);
          client.print("</td>");
        }
        else if (pct_naplneny_zas<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(pct_naplneny_zas);
          client.print("</td>");
        }
                     client.print("<td>");
    client.print(m_pct_naplneny_zas);
    client.print("</td>");

        client.print("<td>");
    client.print(M_pct_naplneny_zas);
    client.print("</td>");

              client.print("<td>%</td>");
      client.print("</tr>");

      // zásobník naplnění %
      client.print("<tr>");
        client.print("<td>Naplneni zasobniku</td>");

        
          client.print("<td>");
          client.print(kWh_zas);
          client.print("</td>");
      
     client.print("<td>");
    client.print(m_kWh_zas);
    client.print("</td>");

        client.print("<td>");
    client.print(M_kWh_zas);
    client.print("</td>");
              client.print("<td>kWh</td>");
      client.print("</tr>");      
  

  //naplnění kWh
      // zásobník 1/4 T6
      client.print("<tr>");
        client.print("<td>Zasobnik 1/4</td>");

        if (T6>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(T6);
          client.print("</td>");
        }
        else if (T6<=60 && T6>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T6);
          client.print("</td>");
        }
        else if (T6<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T6);
          client.print("</td>");
        }
             client.print("<td>");
    client.print(m_T6);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T6);
    client.print("</td>");
              client.print("<td>\377C</td>");
      client.print("</tr>");

            // zásobník 2/4 T2
      client.print("<tr>");
        client.print("<td>Zasobnik 2/4</td>");

        if (T2>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(T2);
          client.print("</td>");
        }
        else if (T2<=60 && T2>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T2);
          client.print("</td>");
        }
        else if (T2<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T2);
          client.print("</td>");
        }
        client.print("<td>");
    client.print(m_T2);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T2);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");

      // zásobník 3/4 T11
      client.print("<tr>");
        client.print("<td>Zasobnik 3/4</td>");

        if (T11>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(T11);
          client.print("</td>");
        }
        else if (T11<=60 && T11>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T11);
          client.print("</td>");
        }
        else if (T11<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T11);
          client.print("</td>");
        }
        client.print("<td>");
    client.print(m_T11);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T11);
    client.print("</td>");
        client.print("<td>\377C</td>");
      client.print("</tr>");
       

// zásobník 4/4 T7
      client.print("<tr>");
        client.print("<td>Zasobnik 4/4</td>");

        if (T7>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(T7);
          client.print("</td>");
        }
        else if (T7<=60 && T7>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T7);
          client.print("</td>");
        }
        else if (T7<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T7);
          client.print("</td>");
        }

        client.print("<td>");
    client.print(m_T7);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T7);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");


//krbová vložka
  //vstup T1
      client.print("<tr>");
        client.print("<td>Vstup do vlozky</td>");

        if (T1>70){
          client.print("<td style='background-color:#FF0000'>");
          client.print(T1);
          client.print("</td>");
        }
        else if (T1<=70 && T1>45){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T1);
          client.print("</td>");
        }
        else if (T1<=45 && T1 > 25){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T1);
          client.print("</td>");
        } 
         else if (T1<=25){
          client.print("<td>");
          client.print(T1);
          client.print("</td>");
        } 

        client.print("<td>");
    client.print(m_T1);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T1);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");

  //vystup T8
      client.print("<tr>");
        client.print("<td>Krbova vlozka</td>");

        if (T8>75){
          client.print("<td style='background-color:#FF0000'>");
          client.print(T8);
          client.print("</td>");
        }
        else if (T8<=75 && T8>55){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T8);
          client.print("</td>");
        }
        else if (T8<=55 && T8 > 25){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(T8);
          client.print("</td>");
        } 
         else if (T8<=25){
          client.print("<td>");
          client.print(T8);
          client.print("</td>");
        } 
        client.print("<td>");
    client.print(m_T8);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T8);
    client.print("</td>");
        client.print("<td>\377C</td>");
      client.print("</tr>"); 

client.print("<tr>");
client.print("</tr>");

  //do topení ze zásobíku T12
      client.print("<tr>");
        client.print("<td>Do topeni ze zasobniku</td>");

        if (T12>40){
          client.print("<td style='background-color:#FF0000'>");
          client.print(T12);
          client.print("</td>");
        }
        else if (T12<=40 && T12>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(T12);
          client.print("</td>");
        }

         else if (T12<=30){
          client.print("<td>");
          client.print(T12);
          client.print("</td>");
        } 

        client.print("<td>");
    client.print(m_T12);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T12);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>"); 

client.print("<tr>");
client.print("</tr>");


//kotel vstup
    client.print("<tr>");
      client.print("<td>Kotel vstup</td>");
           client.print( "<td>");
           client.print(T4);
           client.print("</td>");

           client.print("<td>");
    client.print(m_T4);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T4);
    client.print("</td>");
      client.print("<td>\377C</td>");
    client.print("</tr>");

//kotel vystup
    client.print("<tr>");
      client.print("<td>Kotel vystup</td>");
           client.print( "<td>");
           client.print(T3);
           client.print("</td>");

           client.print("<td>");
    client.print(m_T3);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T3);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>");

        client.print("<td> </td>");
        client.print("<td> </td>");
                client.print("<td> </td>");
            client.print("<td> </td>");
            client.print("<td> </td>");
     client.print("</tr>");

// pokoje
//obyvak
      client.print("<tr>");
        client.print("<td>Obyvak</td>");

        if (Tobyvak>teplota_zadana_H){
          client.print("<td style='background-color:#FFD000'>");
          client.print(Tobyvak);
          client.print("</td>");
        }
        else if (Tobyvak<=teplota_zadana_H && Tobyvak>teplota_zadana_D){
          client.print("<td style='background-color:#00FF00'>");
          client.print(Tobyvak);
          client.print("</td>");
        }
        else if (Tobyvak<=teplota_zadana_D){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(Tobyvak);
          client.print("</td>");
        }

        client.print("<td>");
    client.print(m_Tobyvak);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tobyvak);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");


//pokoj
      client.print("<tr>");
        client.print("<td>pokoj</td>");

        if (Tpokoj>teplota_zadana_H){
          client.print("<td style='background-color:#FFD000'>");
          client.print(Tpokoj);
          client.print("</td>");
        }
        else if (Tpokoj<=teplota_zadana_H && Tpokoj>teplota_zadana_D){
          client.print("<td style='background-color:#00FF00'>");
          client.print(Tpokoj);
          client.print("</td>");
        }
        else if (Tpokoj<=teplota_zadana_D){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(Tpokoj);
          client.print("</td>");
        }

        client.print("<td>");
    client.print(m_Tpokoj);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tpokoj);  
        client.print("</td>");
      client.print("<td>\377C</td>");
    client.print("</tr>");

//loznice
      client.print("<tr>");
        client.print("<td>loznice</td>");

        if (Tloznice>teplota_zadana_H){
          client.print("<td style='background-color:#FFD000'>");
          client.print(Tloznice);
          client.print("</td>");
        }
        else if (Tloznice<=teplota_zadana_H && Tloznice>teplota_zadana_D){
          client.print("<td style='background-color:#00FF00'>");
          client.print(Tloznice);
          client.print("</td>");
        }
        else if (Tloznice<=teplota_zadana_D){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(Tloznice);
          client.print("</td>");
        }

        client.print("<td>");
    client.print(m_Tloznice);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tloznice);
        client.print("</td>");
      client.print("<td>\377C</td>");
    client.print("</tr>");


//pracovna
      client.print("<tr>");
        client.print("<td>pracovna</td>");

        if (Tpracovna>teplota_zadana_H){
          client.print("<td style='background-color:#FFD000'>");
          client.print(Tpracovna);
          client.print("</td>");
        }
        else if (Tpracovna<=teplota_zadana_H && Tpracovna>teplota_zadana_D){
          client.print("<td style='background-color:#00FF00'>");
          client.print(Tpracovna);
          client.print("</td>");
        }
        else if (Tpracovna<=teplota_zadana_D){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(Tpracovna);
          client.print("</td>");
        }

        client.print("<td>");
    client.print(m_Tpracovna);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tpracovna);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");    

     

//za krbovou vlozkou
    client.print("<tr>");
      client.print("<td>Za krbovou vlozkou</td>");
           client.print( "<td>");
           client.print(Tzakrbem);
           client.print("</td>");

           client.print("<td>");
    client.print(m_Tzakrbem);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tzakrbem);
    client.print("</td>");
      client.print("<td>\377C</td>");
    client.print("</tr>");     
 
    client.print("<tr>");
      client.print("<td>Za krbovou vlozkou</td>");
           client.print( "<td>");
           client.print(Hzakrbem);
           client.print("</td>");
                      client.print("<td>");
    client.print(m_Hzakrbem);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Hzakrbem);
    client.print("</td>");

    
      client.print("<td>% rel</td>");
    client.print("</tr>"); 


//koupelna
    client.print("<tr>");
      client.print("<td>Koupelna</td>");
           client.print( "<td>");
           client.print(Tkoupelna);
           client.print("</td>");

                      client.print("<td>");
    client.print(m_Tkoupelna);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Tkoupelna);
    client.print("</td>");
  
    
      client.print("<td>\377C</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Koupenla</td>");
           client.print( "<td>");
           client.print(Hkoupelna);
           client.print("</td>");

                                 client.print("<td>");
    client.print(m_Hkoupelna);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Hkoupelna);
    client.print("</td>");
    
      client.print("<td>% rel</td>");
    client.print("</tr>");

//technicka
    client.print("<tr>");
      client.print("<td>Technicka</td>");
           client.print( "<td>");
           client.print(Ttechnicka);
           client.print("</td>");

                                 client.print("<td>");
    client.print(m_Ttechnicka);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Ttechnicka);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Technicka</td>");
           client.print( "<td>");
           client.print(Htechnicka);
           client.print("</td>");
    client.print("<td>");
               client.print(m_Htechnicka);
    client.print("</td>");

        client.print("<td>");
    client.print(M_Htechnicka);
    client.print("</td>");
    
      client.print("<td>% rel</td>");
    client.print("</tr>");

// podlaha obyvak
    client.print("<tr>");
      client.print("<td>Podlaha obyvak</td>");
           client.print( "<td>");
           client.print(T5);
           client.print("</td>");
    client.print("<td>");
               client.print(m_T5);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T5);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>");    

// podlaha pokoj
    client.print("<tr>");
      client.print("<td>Podlaha pokoj nahore</td>");
           client.print( "<td>");
           client.print(T13);
           client.print("</td>");
    client.print("<td>");
                          client.print(m_T13);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T13);
        client.print("</td>");
        
      client.print("<td>\377C</td>");
    client.print("</tr>") ; 

//mezera
client.print("<tr>");
client.print("<td></td>");
client.print("<td></td>");
client.print("<td></td>");
        client.print("<td> </td>");
            client.print("<td> </td>");
client.print("</tr>");

// ČOV T9
    client.print("<tr>");
      client.print("<td>COV voda</td>");
           client.print( "<td>");
           client.print(T9);
           client.print("</td>");
    client.print("<td>");
                          client.print(m_T9);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T9);
      client.print("</td>");
      
      client.print("<td>\377C</td>");
    client.print("</tr>");  

// ČOV T14
    client.print("<tr>");
      client.print("<td>COV vzduch</td>");
           client.print( "<td>");
           client.print(T14);
           client.print("</td>");
  client.print("<td>");
                          client.print(m_T14);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T14);
      client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>") ;

// ČOV T10
    client.print("<tr>");
      client.print("<td>Vodomer</td>");
           client.print( "<td>");
           client.print(T10);
           client.print("</td>");
  client.print("<td>");
                          client.print(m_T10);
    client.print("</td>");

        client.print("<td>");
    client.print(M_T10);
      client.print("</td>");
      
      client.print("<td>\377C</td>");
    client.print("</tr>") ;    


//ČOV stav


            client.print("<tr>");
      client.print("<td>Stav COV</td>");
           if (stav_COV == 0){
            client.print("<td style='background-color:#FF0000'>CHYBA</td>");
           }
           else if (stav_COV == 1){
            client.print("<td style='background-color:#00FF00'>aerace</td>");
           }
           else if (stav_COV == 2 || stav_COV ==3 || stav_COV ==4){
            client.print("<td>pauza</td>");
            if (stav_COV == 2){
              client.print("<td></td>");
            }
            else if (stav_COV == 3){
              client.print("<td  style='background-color:#00AFFF'>michani</td>");
            }else if (stav_COV == 4){
              client.print("<td></td>");
                            client.print("<td>ver. 2.9</td>");
                                          client.print("<td>20. 2. 2017</td>"); // verze verze verze verze verze verze verze verze verze verze verze verze verze verze verze verze verze verze
            }
           }
           client.print("</tr>");

//topení stav


            client.print("<tr>");
      client.print("<td>Stav topeni</td>");
           if (pozadavek_topit==true){
            client.print("<td style='background-color:#FF0000'>topit: ANO</td>");
           }
           else if (pozadavek_topit == false){
            client.print("<td>topit: NE</td>");
           }
          

           if (pozadavek_kotel == true && pozadavek_zasobnik == true){
              client.print("<td  style='background-color:#FF0000'>oba zdroje</td>");
            }else if (pozadavek_kotel == true && pozadavek_zasobnik == false){
                            client.print("<td  style='background-color:#00FF00'>kotel</td>");
            }else if (pozadavek_kotel == false && pozadavek_zasobnik == true){
                            client.print("<td  style='background-color:#00FF00'>krb</td>");
            }else if (pozadavek_kotel == false && pozadavek_zasobnik == false){
                            client.print("<td>vypnuto</td>");
            }
            

              
              client.print("<td></td>");
                            client.print("<td></td>");
                                        
         
           client.print("</tr>");
           client.print("<tr>");
                 client.print("<td>Ventily</td>");
 client.print("<td>Obyvak</td>");
  client.print("<td>Pracovna</td>");
   client.print("<td>Loznice</td>");
    client.print("<td></td>");

          client.print("</tr>");
           client.print("<tr>");
                 client.print("<td>Otevreno</td>");

if (topit_obyvak == true){
  client.print("<td style='background-color:#FFD00>ANO</td>");  
} else {
    client.print("<td>NE</td>");
}

if (topit_pracovna == true){
  client.print("<td style='background-color:#FFD00>ANO</td>");  
} else {
    client.print("<td>NE</td>");
}
if (topit_loznice == true){
  client.print("<td style='background-color:#FFD00>ANO</td>");  
} else {
    client.print("<td>NE</td>");
}
client.print("<td></td>");
          client.print("</tr>");                 
                 
 
client.print("</table>");

client.print("</body>");
client.print("</html>");


          
          client.println("</html>");
          break;
         
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

    }
  }
};

class ctiTermo
{
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  ctiTermo(int interval1, int interval2, float teplota)
  {
    updateInterval = interval1;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
int thermoDO = 45;
int thermoCS = 46;
int thermoCLK = 47;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
   Serial.print("C = "); 
   float TSP=(thermocouple.readCelsius());
   Serial.println(TSP);
Tspaliny=TSP;

    }
  }
};


ctiMPL pauza1(10000);
ctiDHT pauza2(5000);
ctiTermo pauza3(3000, 300000, 100);
delejWEB pauza4(500);
ctiDS pauza5(5000);
smazmM pauza6(86400000);
smazmM_jednou pauza7(30000);
topeni pauza8 (600000);

void setup(){


    // Open serial communications and wait for port to open:
  Serial.begin(9600);
    //inicializace_DS();

//Ethenrnet
     // Start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());


    
// DHT 
  Serial.println("DHTxx test!");
 
  
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();

  



// MPL
Serial.print("Probe MPL3115A2: ");
    if (mpl3115.initialize()) Serial.println("Sensor found");
    else
    {
        Serial.println("Sensor missing");
        //while (1) {}
    }

    // onetime-measure:
    mpl3115.setEnabled(0);
    mpl3115.triggerMeasurement();
    
  pinMode(pin_COV, OUTPUT);
    pinMode(pin_krb, OUTPUT);
      pinMode(pin_kotel, OUTPUT);
        pinMode(pin_obyvak, OUTPUT);
          pinMode(pin_pracovna, OUTPUT);
            pinMode(pin_loznice, OUTPUT);
    smazmM pauza6(0);

    
}


void loop(){
  pauza1.Update();
  pauza2.Update();
  pauza3.Update();
  pauza4.Update();
  pauza5.Update();
  pauza6.Update(); 
  pauza7.Update();
  pauza8.Update();

 COV();
 aktualizacemM();

  
}
 


  // ČOV---------------------------------------------------------------------------

  void COV(){
        unsigned long now = millis();

        if(stateMainLoop && now - lastTimeMainLoop >= loopOn) {
                // it is time to stop main loop
                lastTimeMainLoop = now;
                lastTimeSubLoop = now;
                stateSubLoop = false;
                stateMainLoop = false;
                doMailLoopStop();
        }else if (!stateMainLoop && now - lastTimeMainLoop >= loopOff ) {
                // time to start main loop
                lastTimeMainLoop = now;
                stateMainLoop = true;
                // if is running sub loop -> force to kill before start main loop
                if(stateSubLoop) {
                        doSubLoopStop();
                        stateSubLoop = false;

                        
                }
                
                doMainLoopStart();
        }
        

        if(!stateMainLoop) {

                if(stateSubLoop && now - lastTimeSubLoop >= subLoopOn) {
                        lastTimeSubLoop = now;
                        stateSubLoop = false;
                        doSubLoopStop();
                }else if(!stateSubLoop && now - lastTimeSubLoop >= subLoopOff) {
                        lastTimeSubLoop = now;
                        stateSubLoop = true;
                        doSubLoopStart();
                }
        }

}


void doMainLoopStart(){
        Serial.println("Start main loop"); // start red
        digitalWrite(pin_COV, HIGH);
        stav_COV = 1;
}

void doMailLoopStop(){
        Serial.println("Stop main loop"); // end red
        digitalWrite(pin_COV, LOW);
        stav_COV =2;

}
void doSubLoopStart(){
        Serial.println("Start sub loop"); // start yellow
        digitalWrite(pin_COV, HIGH);
        stav_COV=4;

}

void doSubLoopStop(){
        Serial.println("Stop sub loop"); // end yellow
        digitalWrite(pin_COV, LOW);
        stav_COV=3;
}

void aktualizacemM(){

  if(m_Tvenku1>Tvenku1){
    m_Tvenku1=Tvenku1;
  }
    if(M_Tvenku1<Tvenku1){
    M_Tvenku1=Tvenku1;
  }

    if(m_Hvenku1>Hvenku1){
    m_Hvenku1=Hvenku1;
  }
    if(M_Hvenku1<Hvenku1){
    M_Hvenku1=Hvenku1;
  }

  if(m_Tvenku2>Tvenku2){
    m_Tvenku2=Tvenku2;
  }
    if(M_Tvenku2<Tvenku2){
    M_Tvenku2=Tvenku2;
  }

    if(m_Pvenku2>Pvenku2){
    m_Pvenku2=Pvenku2;
  }
    if(M_Pvenku2<Pvenku2){
    M_Pvenku2=Pvenku2;
  }
    if(m_Tkoupelna>Tkoupelna){
    m_Tkoupelna=Tkoupelna;
  }
    if(M_Tkoupelna<Tkoupelna){
    M_Tkoupelna=Tkoupelna;
  }

    if(m_Hkoupelna>Hkoupelna){
    m_Hkoupelna=Hkoupelna;
  }
    if(M_Hkoupelna<Hkoupelna){
    M_Hkoupelna=Hkoupelna;
  }

    if(m_Ttechnicka>Ttechnicka){
    m_Ttechnicka=Ttechnicka;
  }
    if(M_Ttechnicka<Ttechnicka){
    M_Ttechnicka=Ttechnicka;
  }

    if(m_Htechnicka>Htechnicka){
    m_Htechnicka=Htechnicka;
  }
    if(M_Htechnicka<Htechnicka){
    M_Htechnicka=Htechnicka;
  }

    if(m_Tzakrbem>Tzakrbem){
    m_Tzakrbem=Tzakrbem;
  }
    if(M_Tzakrbem<Tzakrbem){
    M_Tzakrbem=Tzakrbem;
  }

    if(m_Hzakrbem>Hzakrbem){
    m_Hzakrbem=Hzakrbem;
  }
    if(M_Hzakrbem<Hzakrbem){
    M_Hzakrbem=Hzakrbem;
  }

    if(m_Tspaliny>Tspaliny){
    m_Tspaliny=Tspaliny;
  }
    if(M_Tspaliny<Tspaliny){
    M_Tspaliny=Tspaliny;
  }

    if(m_T1>T1){
    m_T1=T1;
  }
    if(M_T1<T1){
    M_T1=T1;
  }

    if(m_T2>T2){
    m_T2=T2;
  }
    if(M_T2<T2){
    M_T2=T2;
  }
    if(m_T3>T3){
    m_T3=T3;
  }
    if(M_T3<T3){
    M_T3=T3;
  }

    if(m_T4>T4){
    m_T4=T4;
  }
    if(M_T4<T4){
    M_T4=T4;
  }

        if(m_T5>T5){
    m_T5=T5;
  }
    if(M_T5<T5){
    M_T5=T5;
  }

       if(m_T6>T6){
    m_T6=T6;
  }
    if(M_T6<T6){
    M_T6=T6;
  }

       if(m_T7>T7){
    m_T7=T7;
  }
    if(M_T7<T7){
    M_T7=T7;
  }

       if(m_T8>T8){
    m_T8=T8;
  }
    if(M_T8<T8){
    M_T8=T8;
  }
     if(m_T9>T9){
    m_T9=T9;
  }
    if(M_T9<T9){
    M_T9=T9;
  }

      if(m_T10>T10){
    m_T10=T10;
  }
    if(M_T10<T10){
    M_T10=T10;
  }

      if(m_T11>T11){
    m_T11=T11;
  }
    if(M_T11<T11){
    M_T11=T11;
  }

      if(m_T12>T12){
    m_T12=T12;
  }
    if(M_T12<T12){
    M_T12=T12;
  }
      if(m_T13>T13){
    m_T13=T13;
  }
    if(M_T13<T13){
    M_T13=T13;
  }

       if(m_T14>T14){
    m_T14=T14;
  }
    if(M_T14<T14){
    M_T14=T14;
  }

       if(m_pct_naplneny_zas>pct_naplneny_zas){
    m_pct_naplneny_zas=pct_naplneny_zas;
  }
    if(M_pct_naplneny_zas<pct_naplneny_zas){
    M_pct_naplneny_zas=pct_naplneny_zas;
  }

    if(m_kWh_zas>kWh_zas){
    m_kWh_zas=kWh_zas;
  }
    if(M_kWh_zas<kWh_zas){
    M_kWh_zas=kWh_zas;
  }

       if(m_Tobyvak>Tobyvak){
    m_Tobyvak=Tobyvak;
  }
    if(M_Tobyvak<Tobyvak){
    M_Tobyvak=Tobyvak;
  }

         if(m_Tloznice>Tloznice){
    m_Tloznice=Tloznice;
  }
    if(M_Tloznice<Tloznice){
    M_Tloznice=Tloznice;
  }
         if(m_Tpracovna>Tpracovna){
    m_Tpracovna=Tpracovna;
  }
    if(M_Tpracovna<Tpracovna){
    M_Tpracovna=Tpracovna;
  }
         if(m_Tpokoj>Tpokoj){
    m_Tpokoj=Tpokoj;
  }
    if(M_Tpokoj<Tpokoj){
    M_Tpokoj=Tpokoj;
  }
}

 
  



// změny -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
/* 2.9
-oprava tabulky
-aktivace teploměrů obývák, ložnice
 *  2.8 
 -přidány teploměry do místností
 -přidána regulace topení
 2.7 vyházeny zakomentované části, původní kód ponechán ve verzi 2.6
 -předělán web do tabulky
 -každý DS1820 má vlastníPIN
 -přidán int stav_COV pro monitoring pinu dmychadla pro zobrazení na ebu
 -pin dmychadla přesunut na č.50
 2.6 přidán kód na ČOV od Péti
 */