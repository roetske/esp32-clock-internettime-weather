/* 
-----------------------------
function description
-----------------------------
20x4 lcd =>
row 0 date
row 1 - row2 time in big letters - winterhour summerhour automatically set
row 3 scrolling current weather from openweather.org 
does not work with wifi ac only!! agbn works
polling data from openweather in json document is done dynamic
main advantage json format is that you 
can process the data very fast see code

wifi trough autoconnect lib in arduino
when no wifi sets creates access point esp32ap => select wifi access point  with you phone
you get connect webpage where you select your ssid and set password
ssid and password is automatically set in flash by autoconnect

time from pool.org
but you can use time info from openweather also and eliminate polling time from pool.ntp.org
remark time is in format epochtime
credit and thanks to the many who share their code on github

---------------------------------
board
------
esp32 dev mod (choose in arduino)
hardware
--------
esp32 wroom nodemcu
(levelconverter 3.3v to 5v for i2c signals to lcd is option)
lcd 20x4 blue
i2c piggyback
ptc fuse 0.5A 

remark 3.3v is not sufficent for lcd power with 5v
(levelconverter is for signals i2c 3.3v esp to lcd 5v
 but you can do without)
info
--------------------------------------------------------------------
set clock esp32
setTime(30, 24, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
setTime(1609459200);  // 1st Jan 2021 00:00:00

getTime()          //  (String) 15:24:38
getDate()          //  (String) Sun, Jan 17 2021
getDate(true)      //  (String) Sunday, January 17 2021
getDateTime()      //  (String) Sun, Jan 17 2021 15:24:38
getDateTime(true)  //  (String) Sunday, January 17 2021 15:24:38
getTimeDate()      //  (String) 15:24:38 Sun, Jan 17 2021
getTimeDate(true)  //  (String) 15:24:38 Sunday, January 17 2021

getMicros()        //  (long)    723546
getMillis()        //  (long)    723
getEpoch()         //  (long)    1609459200
getSecond()        //  (int)     38    (0-59)
getMinute()        //  (int)     24    (0-59)
getHour()          //  (int)     3     (0-12)
getHour(true)      //  (int)     15    (0-23)
getAmPm()          //  (String)  pm
getAmPm(true)      //  (String)  PM
getDay()           //  (int)     17    (1-31)
getDayofWeek()     //  (int)     0     (0-6)
getDayofYear()     //  (int)     16    (0-365)
getMonth()         //  (int)     0     (0-11)
getYear()          //  (int)     2021

getTime("%A, %B %d %Y %H:%M:%S")   // (String) returns time with specified format 

epoch time via wifi
server 0.be.pool.ntp.org
     server 1.be.pool.ntp.org
     server 2.be.pool.ntp.org
     server 3.be.pool.ntp.org

server 0.europe.pool.ntp.org
server 1.europe.pool.ntp.org
server 2.europe.pool.ntp.org
server 3.europe.pool.ntp.org  


openweather api key get by registring its free
you can test response openweahter by doing httpss request option metric
https://api.openweathermap.org/data/2.5/weather?q=Gent,BE&units=metric&PT&APPID=d301b027de9432c915fda2d1b10753e3
this is string from openweather in json standard set to metric
{"coord":{"lon":3.7167,"lat":51.05}
,"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}]   --searche for hours could not get weather because of [0] see code
,"base":"stations"
,"main":{"temp":5.91,"feels_like":3.15,"temp_min":3.86,"temp_max":8.3,"pressure":1024,"humidity":84,"sea_level":1024,"grnd_level":1023}
,"visibility":10000,"wind":{"speed":3.69,"deg":275,"gust":9.81}
,"clouds":{"all":25},"dt":1645575617 ==epochtijd
,"sys":{"type":2,"id":2011240,"country":"BE","sunrise":1645598570,"sunset":1645636473}
,"timezone":3600,"id":2797656,"name":"Ghent","cod":200}
comments:
2 libs in conflict time.h en Time.h last was the good one
refreshing lcd is slow made own clear
had to put delay when deleting otherwise bignumbers sometimes show nor refreshed correctly
*/
//**************************************************************************
//imported libraries
//**************************************************************************
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <AutoConnect.h>
#include <HTTPClient.h>
#include <TimeLib.h> 
#include <ESP32Time.h>
#include <elapsedMillis.h>
#include <Wire.h>
#include <ArduinoJson.h>
//lcd
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 20 colums and 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4); //adress colums and rows
//**************************************************************************
//var
//**************************************************************************
#define blinkled 2 //for heartbeat
#define UPDATESECS 3600UL//3600UL every hour   --1800UL // update every 30 minute
//i2c ports for you lcd gpio21 gpio22
#define sda 21
#define scl 22
const char *version = "v1";

//autoconnect
WebServer Serverroetske;//lol
AutoConnect Portal(Serverroetske);
AutoConnectConfig Config;
// NTP server to request epoch time
// A UDP instance to let us send and receive packets over UDP
WiFiUDP  udp;
unsigned int localPort = 2390;                        // local port to listen for UDP packets
const char* timeServer = "2.be.pool.ntp.org";         //  my own local time server choose or change to your choice
const char* timeServer2 = "0.europe.pool.ntp.org";    // fall back to regional time server
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// Variable to save current epoch time
unsigned long epochTime; 
int localTimeOffset = 3600 ;// your localtime offset from UCT +1hour for me

//time
ESP32Time rtc;
int interval = 30000; //refresh time lcd time
int heartbeatinterval = 1000; //blinking blue led on nodemcu
time_t update = now() +  UPDATESECS;
String datestringtime;//05:45
String datestringdatum; //09/10/2017-Mon

elapsedMillis scanauto;
elapsedMillis heartbeat;
elapsedMillis i2c_scan;
elapsedMillis refreshscroll;

//i2c
int a_lcd = 0x27;
boolean i2c_ok = false;
//bigfont
int x = 0;
int offsetrow = 1;
//scrolling text openweather
int scroll = 1000; //how fast you want scroll text openweather
int screenWidth = 20;
int stringStart, stringEnd = 0;
int scrollCursor = screenWidth; //start from left to right
int counterscrollmessage = 1;

String test = "123456789101112121415161718192021222324252627282930x ";//last char must be space for scroll
String wthr  = "";//weahterstring
String tmp  = "";//tempstring
String hp  = "";//hum and pr
String wnd  =  "";//wind
bool bnowifi = false;
//**************************************************************************
//setup
//**************************************************************************
void setup() 
{  
   Serial.begin(115200);
  //heartbeatoutput blue led on nodemcu
   pinMode(blinkled, OUTPUT);
   digitalWrite(blinkled,LOW);
   //setup i2c pins
   Wire.begin(sda,scl); // set pin comm i2c
   //Wire.setClock(400000);//i2c speed
   init_create();//init lcd and create templ char for big numbers
   delay(1000);
   clearall();
   delay(1000);
   //bootscreen
   //lcd.setCursor(col, row)
   lcd.setCursor(0,0);
   lcd.print ("Initialising");
   lcd.setCursor(0,1);
   lcd.print ("=============");
   lcd.setCursor(0,2);
   lcd.print(version);
   delay(2000);
   clearline(2);
   //autoconnect
   lcd.print("Connect to wifi....");
   Config.autoReconnect = true;    // Attempt automatic reconnection.
   Config.reconnectInterval = 6;   // Seek interval time is 180[s].
   Config.retainPortal = true;   // Keep the captive portal open.
   Config.psk  = ""; //set or leave blank password esp32ap default 12345678
   Portal.config(Config);
   Portal.begin(); 
  if (WiFi.status() == WL_CONNECTED)
   {
    clearall();
    lcd.print("Wifi ip:");
    lcd.setCursor(0,1);
    lcd.print(WiFi.localIP().toString());
    delay(5000);
    //get time from pool.org
    setNTPtime();
    //get weather    
    getWeatherData();    
    //debug
    clearall();
    delay(1000);
    lcd.setCursor(0,0);
    datestringtime =rtc.getTime("%H:%M");
    datestringdatum =rtc.getDate();
    lcd.print(datestringdatum);
    wificheck();
    bigfont_time();
    
    //debug
    //Serial.println("WiFi: " + WiFi.localIP().toString());
    //Serial.println(rtc.getTime()); //  (String) 15:24:38
    //Serial.println(rtc.getDate());
    
   }
   else
   { //No wifi this does not work
    clearall();
    delay(1000);
    lcd.home(); 
              //01234567890123456789
    lcd.print ("Connect to esp32ap");
    lcd.setCursor(0,1);
    lcd.print("Wifi phone"); 
    delay(5000); 
    clearall();
    delay(1000);    
  }
}


//**************************************************************************
//loop
//**************************************************************************
void loop() 
{
  
  Portal.handleClient();
  //scroll weather line 3
  if (refreshscroll >= scroll)
     { int a = counterscrollmessage;
       String message = "";
       if (a >=5)
       {counterscrollmessage = 1;
        a=1;
       }
       switch(a)
        {case 1:
           message =  wthr;
           break;
         case 2: 
           message =  tmp;
           break;
         case 3: 
           message =  hp;
           break;
         case 4: 
           message =  wnd;
           break;
         default:
            message =  wthr;
         }
        //scrollmessage(test);
        scrollmessage(message);
        refreshscroll =0;
     }
  
  if (scanauto >= interval) //interval refresh
  {     
        
        datestringtime =rtc.getTime("%H:%M");
        datestringdatum =rtc.getDate();
        //debug
        //Serial.println(datestringtime);
        //Serial.println(datestringdatum);
        //naar lcd
        clearline(1);
        clearline(2);
        delay(200);
        lcd.setCursor(0,0);
        lcd.print(datestringdatum);
        wificheck();
        bigfont_time(); 
        if (now() > update) //check  internet and update every 60 minutes
        {  
           //Portal.handleClient();
           if (WiFi.status() == WL_CONNECTED)
            {
              setNTPtime();
              getWeatherData();
              //debug
              //Serial.println(rtc.getTime());//  (String) 15:24:38
              //Serial.println(rtc.getDate());
              //Serial.println(rtc.getTime("%H:%M"));
            }    
         }
        //reset scan
       scanauto = 0;
  }
  if (i2c_scan > 1000)
    { 
      i2c_active(a_lcd);
      i2c_scan = 0;
    }
  f_heartbeat();
  //show_bigfont_show_all();
}
//subs
void f_heartbeat()
{
  if (heartbeat > heartbeatinterval)
  {
    digitalWrite(blinkled,!digitalRead(blinkled));
    heartbeat = 0;
  }  
}
//i2c check
void i2c_active(int addres) 
 {  
    int errori2c;
    Wire.beginTransmission(addres);
    errori2c = Wire.endTransmission();
    Wire.endTransmission(addres);
    if (errori2c != 0)
     { 
      i2c_ok = false;
       //Serial.print("I2cNOK:=");
       //Serial.print(i2c_ok);
       //Serial.print("//");
       //Serial.println(addres);
     }
    else
      {i2c_ok = true ;}   
 }




