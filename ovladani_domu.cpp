char verze[] = "verze: 3.0|datum: 1.5.2019";

int stopmM = 1;
//minima a maxima

float hodnoty[28];  
/*0:tepltoa venku1, 1:vlhkost venku, 2:teplota venku2, 3:tlak venku, 4:teplota koupelna, 5:vlhkost koupelna, 
 * 6:teplota technicka, 7:vlhkost technicka, 8:teplota za krbem, 9:vlhkost za krbem, 10:teplota spalin, 11:voda do vložky
 * 12:zásobník1/4, 13:zásobník2/4, 14:zásobník3/4, 15:zásobník4/4, 16:podlaha obývák, 17:teplota dešťovka, 18:teplota vodoměr 
 * 19:ze zásobníku do topení, 20:teplota přípojka starobinec, 21:kWh zasobnik, 22:výška destovky, 23:objem dešťovky, 24:krbova vložka
 * 25:procento naplnění zásobníku
*/

float a15min_hodnoty[28];
float a30min_hodnoty[28];
float a6h_hodnoty[28];
float a12h_hodnoty[28];

float minima[28];
float maxima[28];


#include <Arduino.h>

// teploměra DS1820
#include <OneWire.h>
#include <DallasTemperature.h>


// MAX 6675
#include "max6675.h"




//MPL311
#include <Wire.h>
#include "i2c.h"

#include "i2c_MPL3115A2.h"
MPL3115A2 mpl3115;

// Ultrazvukový vodotěsný modul pro měření vzdálenosti
// JSN-SR04T

// připojení potřebné knihovny
#include <NewPing.h>
// nastavení propojovacích pinů
#define pinTrigger    3
#define pinEcho       4
#define maxVzdalenost 300
// inicializace měřícího modulu z knihovny
NewPing sonar(pinTrigger, pinEcho, maxVzdalenost);



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
 
hodnoty[0] = t5;
hodnoty[1] = h5;
hodnoty[6] = t3;
hodnoty[7]=h3;
hodnoty[4] =t4;
hodnoty[5]= h4;
hodnoty[8] = t2;
hodnoty[9] = h2;


  
  
}

    }

};

class Cti_destovka
{
int updateInterval;
int interval;
  public:
Cti_destovka (int interval)
  {
    updateInterval = interval;
  }
  void Update()
  {
   
  // načtení vzdálenosti v centimetrech do vytvořené proměnné vzdalenost
  int vzdalenost = sonar.ping_cm();
  // pauza před dalším měřením
  delay(50);
  // pokud byla detekována vzdálenost větší než 0,
  // provedeme další měření
  if (vzdalenost > 0) {
    vzdalenost = 0;
    // pro získání stabilnějších výsledků provedeme 5 měření
    // a výsledky budeme přičítat do proměnné vzdalenost
    for(int i=0; i<5; i++) {
      vzdalenost += sonar.ping_cm();
      delay(50);
    }
    // v proměnné vzdálenost máme součet posledních 5 měření
    // a musíme tedy provést dělení 5 pro získání průměru
    vzdalenost = vzdalenost/5;
    // vytištění informací po sériové lince
    Serial.print("Vzdalenost mezi senzorem a predmetem je ");
    Serial.print(vzdalenost);
    Serial.println(" cm.");
    
hodnoty[22] = vzdalenost;
  }
  // pokud byla detekována vzdálenost 0, je předmět mimo měřící rozsah,
  // tedy příliš blízko nebo naopak daleko
  else {
    Serial.println("Vzdalenost mezi senzorem a predmetem je mimo merici rozsah.");
    delay(500);
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
  
hodnoty[11]=teplota(29);
  Serial.print("voda dovložky ");
  Serial.println(hodnoty[11]);

  hodnoty[14]=teplota(33);
Serial.print("zásobník 3/4 ");
Serial.println(hodnoty[14]);


hodnoty[16]=teplota(22);
Serial.print("podlaha obyvak ");
Serial.println(hodnoty[16]);

hodnoty[12]=teplota(31);
Serial.print("zasobnik 1/4 ");
Serial.println(hodnoty[12]);

hodnoty[15]=teplota(32);
Serial.print("zasobnik 4/4 ");
Serial.println(hodnoty[15]);

hodnoty[24]=teplota(28);
Serial.print("krbová bložka ");
Serial.println(hodnoty[24]);

hodnoty[20]=teplota(35);
Serial.print("přípojka starobinec ");
Serial.println(hodnoty[20]);

hodnoty[18]=teplota(36);
Serial.print("vodomer ");
Serial.println(hodnoty[18]);  

hodnoty[13]=teplota(30); //puvodne 30, vyzkousim 25 a 26
Serial.print("zasobnik 2/4 ");
Serial.println(hodnoty[13]);

hodnoty[19]=teplota(27);
Serial.print("ze zasobniku do topeni ");
Serial.println(hodnoty[19]);

/*hodnoty[13]=teplota(24);
Serial.print("pokoj nahoře podlaha??? ");
Serial.println(hodnoty[13]);*/

hodnoty[17]=teplota(34);
Serial.print("teplota destovka ");
Serial.println(hodnoty[17]);

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
      for (int i = 0; i<28; i++)
      {
        minima[i] = hodnoty[i];
        maxima[i] = hodnoty[i];
      }
Serial.println("smazani mM");

    }
  }
};

// hodnota pulhodiny stara
class a30min
{
  unsigned long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  a30min(unsigned long interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      for (int i = 0; i<28; i++)
      {
        a30min_hodnoty[i] = a15min_hodnoty[i];
        a15min_hodnoty [i] = hodnoty[i];
        
      }

    }
  }
};

// hodnota pulhodiny stara
class a12hodin
{
  unsigned long  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
 public: 
  a12hodin(unsigned long interval)
  {
    updateInterval = interval;
  }
    void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      for (int i = 0; i<28; i++)
      {
        a12h_hodnoty[i] = a6h_hodnoty[i];
        a6h_hodnoty [i] = hodnoty[i];
        
      }

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
    if (stopmM<2);
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      stopmM = stopmM+1;;      
      
      for (int i = 0; i<28; i++)
      {
        minima[i] = hodnoty[i];
        maxima[i] = hodnoty[i];
     Serial.println("smazánomM - jednou"); 
    }
  }
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

hodnoty[2] = temperature;
hodnoty[3] = pressure;

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
       /*   client.println("<!DOCTYPE HTML>");
client.print("<html>");
client.print("<head>");
client.print("<style>");
client.print("table, th, td {    border: 1px solid black;}");
client.print("</style>");
client.print("</head>");*/
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.print("<style type=\"text/css\">.tg  {border-collapse:collapse;border-spacing:0;border-color:#aabcfe;}.tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#aabcfe;color:#669;background-color:#e8edff;}.tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#aabcfe;color:#039;background-color:#b9c9fe;}.tg .tg-33uu{background-color:#e8edff;text-align:right;vertical-align:top}.tg .tg-so6c{background-color:#b9c9fe;color:#003399;text-align:left;vertical-align:top}.tg .tg-3uz3{background-color:#e8edff;text-align:left;vertical-align:top}.tg .tg-0lax{text-align:left;vertical-align:top}.tg .tg-4kqm{background-color:#e8edff;text-align:left;vertical-align:top}.tg .tg-ppl9{background-color:#e8edff;text-align:right;vertical-align:top}");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
client.print("<body>");

client.print("<table>");
  client.print("<tr>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Cidlo</th>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Hodnota</th>");
        client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Minimum</th>");
            client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Maximum</th>");
                        client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>15-30 minut</th>");
                                    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>6-12 hodin</th>");
    client.print("<th style='background-color:#0AA0AA;color:#FFFFFF'>Jednotka</th>");
  client.print("</tr>");
  client.print("<tr> ");
    client.print("<td>Teplota venku 1</td>");
    if (hodnoty[0]>0){
           client.print( "<td>");
           client.print(hodnoty[0]);
          client.print(" </td>");
    }
    else{
      client.print("<td style='background-color:#00AFFF'>");
      client.print(hodnoty[0]);
      client.print("</td>"); //modrá
    }

        client.print("<td>");
    client.print(minima[0]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[0]);
    client.print("</td>");


              client.print("<td>");
    client.print(a30min_hodnoty[0]);
    client.print("</td>");

              client.print("<td>");
    client.print(a12h_hodnoty[0]);
    client.print("</td>");
      client.print("<td>\377C</td>"); 
      
  client.print("</tr>");
    client.print("<tr>"); 
    client.print("<td>Teplota venku 2</td>");
    if (hodnoty[2]>0){
            client.print("<td>");
            client.print(hodnoty[2]);
            client.print("</td>");
    }
    else{
      client.print("<td style='background-color:#00AFFF'>");
      client.print(hodnoty[2]);
      client.print("</td>"); //modrá
    }
    client.print("<td>");
    client.print(minima[2]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[2]);
    client.print("</td>");

            client.print("<td>");
    client.print(a30min_hodnoty[2]);
    client.print("</td>");

            client.print("<td>");
    client.print(a12h_hodnoty[2]);
    client.print("</td>");
      client.print("<td>\377C</td>");
      
  client.print("</tr>");
   
    
    // vlhkost venku
    client.print("<tr>");
      client.print("<td>Vlhkost venku1</td>");
           client.print( "<td>");
           client.print(hodnoty[1]);
           client.print("</td>");
                          client.print("<td>");
    client.print(minima[1]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[1]);
    client.print("</td>");

        client.print("<td>");
    client.print(a30min_hodnoty[1]);
    client.print("</td>");

            client.print("<td>");
    client.print(a12h_hodnoty[1]);
    client.print("</td>");
      client.print("<td>% rel.</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Tlak venku 2</td>");
           client.print( "<td>");
           client.print(hodnoty[3]);
           client.print("</td>");


               client.print("<td>");
    client.print(minima[3]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[3]);
    client.print("</td>");
           client.print("<td>");
    client.print(a30min_hodnoty[3]);
    client.print("</td>");
    
           client.print("<td>");
    client.print(a12h_hodnoty[3]);
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

        if (hodnoty[10]>380){
          client.print("<td style='background-color:#FF0000'>");
          client.print(hodnoty[10]);
          client.print("</td>");
        }
        else if (hodnoty[10]<380 && hodnoty[10]>280){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[10]);
          client.print("</td>");
        }
        else if (hodnoty[10]<=280 && hodnoty[10]>=50){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[10]);
          client.print("</td>");
        }
                else if (hodnoty[10]<50){
          client.print("<td>");
          client.print(hodnoty[10]);
          client.print("</td>");
        }
                       client.print("<td>");
    client.print(minima[10]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[10]);
    client.print("</td>");

 client.print("<td>");
    client.print(a30min_hodnoty[10]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[10]);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");

    


//zásobník


  //T6 + T2 +T11+ T7
  float prumerna_teplota_zas= ((hodnoty[12] +hodnoty[13]+ hodnoty[14] +hodnoty[15])/4);
int pct_naplneny_zas= (100 *((prumerna_teplota_zas-25)/35));
int kWh_zas = ((prumerna_teplota_zas -25)*0.8);
hodnoty[25] = pct_naplneny_zas;
hodnoty[21] = kWh_zas;

  
      // zásobník naplnění %
      client.print("<tr>");
        client.print("<td>Naplneni zasobniku</td>");

        if (hodnoty[25]>100){
          client.print("<td style='background-color:#FF0000'>");
          client.print(hodnoty[25]);
          client.print("</td>");
        }
        else if (hodnoty[25]<=100 && hodnoty[25]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[25]);
          client.print("</td>");
        }
        else if (hodnoty[25]<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[25]);
          client.print("</td>");
        }
                     client.print("<td>");
    client.print(minima[25]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[25]);
    client.print("</td>");

            client.print("<td>");
     client.print(a30min_hodnoty[25]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[25]);
    client.print("</td>");

              client.print("<td>%</td>");
      client.print("</tr>");

      // zásobník naplnění %
      client.print("<tr>");
        client.print("<td>Naplneni zasobniku</td>");

        
          client.print("<td>");
          client.print(hodnoty[21]);
          client.print("</td>");
      
     client.print("<td>");
    client.print(minima[21]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[21]);
    client.print("</td>");

        client.print("<td>");
        client.print(a30min_hodnoty[21]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[21]);
    client.print("</td>");
    
              client.print("<td>kWh</td>");
      client.print("</tr>");      
  

  //naplnění kWh
      // zásobník 1/4 T6
      client.print("<tr>");
        client.print("<td>Zasobnik 1/4</td>");

        if (hodnoty[12]>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(hodnoty[12]);
          client.print("</td>");
        }
        else if (hodnoty[12]<=60 && hodnoty[12]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[12]);
          client.print("</td>");
        }
        else if (hodnoty[12]<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[12]);
          client.print("</td>");
        }
             client.print("<td>");
    client.print(minima[12]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[12]);
    client.print("</td>");

                 client.print("<td>");
    client.print(a30min_hodnoty[12]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[12]);
    client.print("</td>");
    
              client.print("<td>\377C</td>");
      client.print("</tr>");

            // zásobník 2/4 T2
      client.print("<tr>");
        client.print("<td>Zasobnik 2/4</td>");

               if (hodnoty[13]>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(hodnoty[13]);
          client.print("</td>");
        }
        else if (hodnoty[13]<=60 && hodnoty[13]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[13]);
          client.print("</td>");
        }
        else if (hodnoty[13]<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[13]);
          client.print("</td>");
        }
             client.print("<td>");
    client.print(minima[13]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[13]);
    client.print("</td>");

                 client.print("<td>");
    client.print(a30min_hodnoty[13]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[13]);
    client.print("</td>");
    
              client.print("<td>\377C</td>");
      client.print("</tr>");

      // zásobník 3/4 T11
      client.print("<tr>");
        client.print("<td>Zasobnik 3/4</td>");

                if (hodnoty[14]>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(hodnoty[14]);
          client.print("</td>");
        }
        else if (hodnoty[14]<=60 && hodnoty[14]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[14]);
          client.print("</td>");
        }
        else if (hodnoty[14]<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[14]);
          client.print("</td>");
        }
             client.print("<td>");
    client.print(minima[14]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[14]);
    client.print("</td>");

                 client.print("<td>");
    client.print(a30min_hodnoty[14]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[14]);
    client.print("</td>");
    
              client.print("<td>\377C</td>");
      client.print("</tr>");
       

// zásobník 4/4 T7
      client.print("<tr>");
        client.print("<td>Zasobnik 4/4</td>");

                if (hodnoty[15]>60){
          client.print("<td style='background-color:#FFD000'>");
          client.print(hodnoty[15]);
          client.print("</td>");
        }
        else if (hodnoty[15]<=60 && hodnoty[15]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[15]);
          client.print("</td>");
        }
        else if (hodnoty[15]<=30){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[15]);
          client.print("</td>");
        }
             client.print("<td>");
    client.print(minima[15]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[15]);
    client.print("</td>");

                 client.print("<td>");
    client.print(a30min_hodnoty[15]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[15]);
    client.print("</td>");
    
              client.print("<td>\377C</td>");
      client.print("</tr>");


//krbová vložka
  //vstup T1
      client.print("<tr>");
        client.print("<td>Vstup do vlozky</td>");

        if (hodnoty[11]>70){
          client.print("<td style='background-color:#FF0000'>");
          client.print(hodnoty[11]);
          client.print("</td>");
        }
        else if (hodnoty[11]<=70 && hodnoty[11]>45){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[11]);
          client.print("</td>");
        }
        else if (hodnoty[11]<=45 && hodnoty[11] > 25){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[11]);
          client.print("</td>");
        } 
         else if (hodnoty[11]<=25){
          client.print("<td>");
          client.print(hodnoty[11]);
          client.print("</td>");
        } 

        client.print("<td>");
    client.print(minima[11]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[11]);
    client.print("</td>");

            client.print("<td>");
    client.print(a30min_hodnoty[11]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[11]);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>");

  //vystup T8
      client.print("<tr>");
        client.print("<td>Krbova vlozka</td>");

        if (hodnoty[24]>75){
          client.print("<td style='background-color:#FF0000'>");
          client.print(hodnoty[24]);
          client.print("</td>");
        }
        else if (hodnoty[24]<=75 && hodnoty[24]>55){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[24]);
          client.print("</td>");
        }
        else if (hodnoty[24]<=55 && hodnoty[24] > 25){
          client.print("<td style='background-color:#00AFFF'>");
          client.print(hodnoty[24]);
          client.print("</td>");
        } 
         else if (hodnoty[24]<=25){
          client.print("<td>");
          client.print(hodnoty[24]);
          client.print("</td>");
        } 
        client.print("<td>");
    client.print(minima[24]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[24]);
    client.print("</td>");

        client.print("<td>");
    client.print(a30min_hodnoty[24]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[24]);
    client.print("</td>");
    
        client.print("<td>\377C</td>");
      client.print("</tr>"); 

client.print("<tr>");
client.print("</tr>");

  //do topení ze zásobíku T12
      client.print("<tr>");
        client.print("<td>Do topeni ze zasobniku</td>");

        if (hodnoty[19]>40){
          client.print("<td style='background-color:#FF0000'>");
          client.print(hodnoty[19]);
          client.print("</td>");
        }
        else if (hodnoty[19]<=40 && hodnoty[19]>30){
          client.print("<td style='background-color:#00FF00'>");
          client.print(hodnoty[19]);
          client.print("</td>");
        }

         else if (hodnoty[19]<=30){
          client.print("<td>");
          client.print(hodnoty[19]);
          client.print("</td>");
        } 

        client.print("<td>");
    client.print(minima[19]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[19]);
    client.print("</td>");

        client.print("<td>");
    client.print(a30min_hodnoty[19]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[19]);
    client.print("</td>");
        
        client.print("<td>\377C</td>");
      client.print("</tr>"); 

client.print("<tr>");
client.print("</tr>");

//technická místnost
    client.print("<tr>");
      client.print("<td>Technicka</td>");
           client.print( "<td>");
           client.print(hodnoty[6]);
           client.print("</td>");

           client.print("<td>");
    client.print(minima[6]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[6]);
    client.print("</td>");

           client.print("<td>");
    client.print(a30min_hodnoty[6]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[6]);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>");     
 
    client.print("<tr>");
      client.print("<td>Technicka</td>");
           client.print( "<td>");
           client.print(hodnoty[7]);
           client.print("</td>");
                      client.print("<td>");
    client.print(minima[7]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[7]);
    client.print("</td>");

           client.print("<td>");
    client.print(a30min_hodnoty[7]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[7]);
    client.print("</td>");

    
      client.print("<td>% rel</td>");
    client.print("</tr>"); 


//koupelna
    client.print("<tr>");
      client.print("<td>Koupelna</td>");
           client.print( "<td>");
           client.print(hodnoty[4]);
           client.print("</td>");

                      client.print("<td>");
    client.print(minima[4]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[4]);
    client.print("</td>");

               client.print("<td>");
    client.print(a30min_hodnoty[4]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[4]);
    client.print("</td>");
  
    
      client.print("<td>\377C</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Koupenla</td>");
           client.print( "<td>");
           client.print(hodnoty[5]);
           client.print("</td>");

                                 client.print("<td>");
    client.print(minima[5]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[5]);
    client.print("</td>");

               client.print("<td>");
    client.print(a30min_hodnoty[5]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[5]);
    client.print("</td>");
    
      client.print("<td>% rel</td>");
    client.print("</tr>");

//za krbem
    client.print("<tr>");
      client.print("<td>TZa krbem</td>");
           client.print( "<td>");
           client.print(hodnoty[8]);
           client.print("</td>");

                                 client.print("<td>");
    client.print(minima[8]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[8]);
    client.print("</td>");

           client.print("<td>");
    client.print(a30min_hodnoty[8]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[8]);
    client.print("</td>");
    
    
      client.print("<td>\377C</td>");
    client.print("</tr>");

    client.print("<tr>");
      client.print("<td>Za krbem</td>");
           client.print( "<td>");
           client.print(hodnoty[9]);
           client.print("</td>");
    client.print("<td>");
               client.print(minima[9]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[9]);
    client.print("</td>");

               client.print("<td>");
    client.print(a30min_hodnoty[9]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[9]);
    client.print("</td>");
    
      client.print("<td>% rel</td>");
    client.print("</tr>");

// podlaha obyvak
    client.print("<tr>");
      client.print("<td>Podlaha obyvak</td>");
           client.print( "<td>");
           client.print(hodnoty[16]);
           client.print("</td>");
    client.print("<td>");
               client.print(minima[16]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[16]);
    client.print("</td>");

               client.print("<td>");
    client.print(a30min_hodnoty[16]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[16]);
    client.print("</td>");
    
      client.print("<td>\377C</td>");
    client.print("</tr>");    

//mezera
client.print("<tr>");
client.print("<td></td>");
client.print("<td></td>");
client.print("<td></td>");
        client.print("<td> </td>");
            client.print("<td> </td>");
            client.print("<td> </td>");
client.print("</tr>");

// ČOV T9 - předěláno na přípojku do špejcharu
    client.print("<tr>");
      client.print("<td>pripojka starobinec</td>");
           client.print( "<td>");
           client.print(hodnoty[20]);
           client.print("</td>");
    client.print("<td>");
                          client.print(minima[20]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[20]);
      client.print("</td>");

      
               client.print("<td>");
    client.print(a30min_hodnoty[20]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[20]);
    client.print("</td>");
    
      
      client.print("<td>\377C</td>");
    client.print("</tr>");  
    
// dešťovka
    client.print("<tr>");
      client.print("<td>destovka - vyska vody</td>");
           client.print( "<td>");
           client.print(hodnoty[22]);
           client.print("</td>");
    client.print("<td>");
                          client.print(minima[22]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[22]);
      client.print("</td>");


               client.print("<td>");
    client.print(a30min_hodnoty[22]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[22]);
    client.print("</td>");
          
      
      client.print("<td>cm</td>");
    client.print("</tr>");  

// dešťovka litry

int litry_vody = (180-hodnoty[22]) *6.15;
hodnoty[23] = litry_vody;
    client.print("<tr>");
      client.print("<td>destovka - objem</td>");
           client.print( "<td>");
           client.print(hodnoty[23]);
           client.print("</td>");
    client.print("<td>");
                          client.print(minima[23]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[23]);
      client.print("</td>");


               client.print("<td>");
    client.print(a30min_hodnoty[23]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[23]);
    client.print("</td>");
          
      
      client.print("<td>litry</td>");
    client.print("</tr>");

// ČOV T14
    client.print("<tr>");
      client.print("<td>destovka-teplota</td>");
           client.print( "<td>");
           client.print(hodnoty[17]);
           client.print("</td>");
  client.print("<td>");
                          client.print(minima[17]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[17]);
      client.print("</td>");

               client.print("<td>");
    client.print(a30min_hodnoty[17]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[17]);
    client.print("</td>");
          
    
      client.print("<td>\377C</td>");
    client.print("</tr>") ;

// ČOV T10
    client.print("<tr>");
      client.print("<td>Vodomer</td>");
           client.print( "<td>");
           client.print(hodnoty[18]);
           client.print("</td>");
  client.print("<td>");
                          client.print(minima[18]);
    client.print("</td>");

        client.print("<td>");
    client.print(maxima[18]);
      client.print("</td>");

      
               client.print("<td>");
    client.print(a30min_hodnoty[18]);
    client.print("</td>");

        client.print("<td>");
    client.print(a12h_hodnoty[18]);
    client.print("</td>");
          
      
      client.print("<td>\377C</td>");
    client.print("</tr>") ;  

      client.print("<tr>");
      client.print("<td>");
      client.print(verze);
      client.print("</td></tr>");


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
hodnoty[10]=TSP;

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
Cti_destovka pauza9 (60000);
a30min pauza10(900000);
a12hodin pauza11(21600000);

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
//  pauza8.Update();
  pauza9.Update();
  pauza10.Update();
  pauza11.Update();

  Serial.println("aktualizace m");
for(int i = 0; i<28; i++)
{
  if (minima[i]>hodnoty[i])
  {  Serial.println("aktualizace m");
    minima[i] = hodnoty[i];
  }
  if (maxima[i]<hodnoty[i])
  {  Serial.println("aktualizace M");
    maxima[i]= hodnoty[i];
  }
}

//aktualizacemM();
}

/*
 void ultrazvuk(){
  // načtení vzdálenosti v centimetrech do vytvořené proměnné vzdalenost
  int vzdalenost = sonar.ping_cm();
  // pauza před dalším měřením
  delay(50);
  // pokud byla detekována vzdálenost větší než 0,
  // provedeme další měření
  if (vzdalenost > 0) {
    vzdalenost = 0;
    // pro získání stabilnějších výsledků provedeme 5 měření
    // a výsledky budeme přičítat do proměnné vzdalenost
    for(int i=0; i<5; i++) {
      vzdalenost += sonar.ping_cm();
      delay(50);
    }
    // v proměnné vzdálenost máme součet posledních 5 měření
    // a musíme tedy provést dělení 5 pro získání průměru
    vzdalenost = vzdalenost/5;
    // vytištění informací po sériové lince
    Serial.print("Vzdalenost mezi senzorem a predmetem je ");
    Serial.print(vzdalenost);
    Serial.println(" cm.");
    hodnoty[22] = vzdalenost;
  }
  // pokud byla detekována vzdálenost 0, je předmět mimo měřící rozsah,
  // tedy příliš blízko nebo naopak daleko
  else {
    Serial.println("Vzdalenost mezi senzorem a predmetem je mimo merici rozsah.");
    delay(500);
  }
 }

*/

void aktualizacemM(){
Serial.println("aktualizace mM");
for(int i = 0; i<28; i++)
{
  if (minima[i]>hodnoty[i])
  {
    minima[i] = hodnoty[i];
  }
  if (maxima[i]<hodnoty[i])
  {
    maxima[i]=hodnoty[i];
  }
}
}





/* změny -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 3.0
-přidán ultrazvuk
-přidána teplota vody
-opravena teplota u přípojky
-přidány 30min a 12h hodnoty
-proměnné přesunuty do polí
-odstraněny nevyužité části kódu

*/

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
