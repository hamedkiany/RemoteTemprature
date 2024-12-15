
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "dht11.h"
#include "login.h"


dht11 DHT11;

#define DHT11PIN D2
#define MosfetControlPIN D3
#define BatteryLevelPIN A0
#define EEPROM_SIZE 64
#define MIN_TEMP_ADDR 0 // Address for minimum temperature
#define MAX_TEMP_ADDR 4 // Address for maximum temperature
#define Sleep_Time_ADDR 8 // Address for Sleep Time

//volatile float sleeptime = 1 * 60 * 1000000UL  ;// 15 minutos
// Wifi network station credentials
unsigned long previousMillis = 0UL;
unsigned long interval =    2  * 1000000UL;

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

const int ledPin = LED_BUILTIN;

int ledStatus = 0;
float temperatura = 0;
String  messageT = "";

void saveTemperatureRange(float minTemp, float maxTemp) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(MIN_TEMP_ADDR, minTemp); // Save minimum temperature
  EEPROM.put(MAX_TEMP_ADDR, maxTemp); // Save maximum temperature
  EEPROM.commit();
}

void readTemperatureRange(float &minTemp, float &maxTemp) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(MIN_TEMP_ADDR, minTemp); // Read minimum temperature
  EEPROM.get(MAX_TEMP_ADDR, maxTemp); // Read maximum temperature
}

void saveSleepTimeRange(float Sleep) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(Sleep_Time_ADDR, Sleep); // Save Sleep Timer
  EEPROM.commit();
}

void readSleepTimeRange(float &Sleep) {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(Sleep_Time_ADDR, Sleep); // Read Sleep Timer
}

String* splitString(String data, char delimiter, int &numParts) {
  static String parts[10]; // Static array to hold the parts 
  numParts = 0;

  int start = 0;
  int end = data.indexOf(delimiter);

  while (end != -1) {
    parts[numParts++] = data.substring(start, end);
    start = end + 1;
    end = data.indexOf(delimiter, start);
  }

  parts[numParts++] = data.substring(start); // Add the last part
  return parts;
}


void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/ledon")
    {
      digitalWrite(ledPin, LOW); // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      from_name = "Hello " + from_name;
      bot.sendMessage(chat_id, from_name, "");
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff")
    {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }
  if (text == "/Tempratue")
    {
      temperatura = DHT11.read(DHT11PIN); // ReadTempPIn
      Serial.println((float)DHT11.temperature, 2);

      messageT = ((float)DHT11.temperature);
      bot.sendMessage(chat_id, messageT , "");
    }
  if (text == "/OTA")
    {
      ArduinoOTA.handle(); // activate ota
      bot.sendMessage(chat_id, "Activated OTA" , "");
    }
    if (text == "/status")
    {
      float minTemp = 0, maxTemp = 0 , sleep = 0;
      readTemperatureRange(minTemp, maxTemp);
      readSleepTimeRange(sleep);
      String result = "Min Temp: " +String(minTemp, 2) + " Max Temp: " + String(maxTemp, 2) + "  Led Status: " + String(ledStatus, 2) + " Sleep Time: " + String(sleep, 2) + " ";
//      if (ledStatus)
//      {
//        bot.sendMessage(chat_id, "Led is ON", "");
//      }
//      else
//      {
//        bot.sendMessage(chat_id, "Led is OFF", "");
//      }
  bot.sendMessage(chat_id, result, "");
    }
    if (text == "/BatteryLevel")
    {
    String temporary = "Battery Charge is : ";
    messageT = temporary + ( readvoltage() * 100) ; // make message 
    bot.sendMessage(chat_id, messageT, ""); // send message
    }



    if (text.startsWith("/sleep") > 0 )
    {
      float sleeptime= 0;
      int numParts;
  // Call the function
      String* words = splitString(text, ' ', numParts);
      if(numParts > 1){

      sleeptime =words[1].toInt(); //* 60 * 1000000UL ;
      saveSleepTimeRange(sleeptime);
      
      bot.sendMessage(chat_id, "Wifi go to sleep", "");
      Serial.println(sleeptime);

      delay( 100 );
    }
    else{
      bot.sendMessage(chat_id, "Error in Syntax", "");
    }
//      WiFi.disconnect( true );
      delay( 1 );
      
    // WAKE_RF_DISABLED to keep the WiFi radio disabled when we wake up
      //ESP.deepSleep( sleeptime);//, WAKE_RF_DISABLED );
      sleepfuncion();
    }

    

    if (text.startsWith("/TempratueRange") > 0 )
    {
      float minTemp=0 , maxTemp=0;
      int numParts;
  // Call the function
      String* words = splitString(text, ' ', numParts);
      if(numParts > 2){

      minTemp =words[1].toInt();
      maxTemp =words[2].toInt();      
      saveTemperatureRange(minTemp,maxTemp);
      
      bot.sendMessage(chat_id, "Set range", "");
      Serial.println(minTemp);
      Serial.println(maxTemp);

      delay( 100 );
    }
    else{
      bot.sendMessage(chat_id, "Error in Syntax", "");
    }
  }

    if (text == "/start")
    {
      String welcome = "Welcome to Remote Temprature, " + from_name + ".\n";
      welcome += "This is Temprature Bot.\n\n";
      welcome += "/ledon : to switch the Led ON\n";
      welcome += "/ledoff : to switch the Led OFF\n";
      welcome += "/Tempratue : Returns current Tempratue\n";
      welcome += "/TempratueRange Min Max : Set range of Tempratue for Alarm\n";
      welcome += "/sleep SleepTime: Goto sleep for Sleeptime min \n";
      welcome += "/BatteryLevel: show battery level \n";
      welcome += "/OTA : Activate OTA\n";
      welcome += "/status : Returns current status of LED and all variable\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  pinMode(MosfetControlPIN, OUTPUT); // initialize digital MosfetControlPIN as an output.
  pinMode(BatteryLevelPIN, INPUT); // initialize ADC BatteryLevelPIN as an input.
  

  delay(10);
  digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)
  digitalWrite(MosfetControlPIN, LOW); // initialize pin as off (active HIGH)
  initOTA();
  connectWiFi();
  float minTemp = 0, maxTemp = 0 , sleep = 0;
  readTemperatureRange(minTemp, maxTemp);
  readSleepTimeRange(sleep);



}

void loop()
{
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval)
  {
	/* The Arduino executes this code once every second
 	*  (interval = 1000 (ms) = 1 second).
 	*/
  readvoltage();
  readtemp();
  //ESP.deepSleep( sleeptime, WAKE_RF_DISABLED );

 	// Don't forget to update the previousMillis value
 	previousMillis = currentMillis;
  }
  ArduinoOTA.handle();
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
void readtemp()
{
int chk = DHT11.read(DHT11PIN);
float minTemp=0 , maxTemp=0;
readTemperatureRange(minTemp,maxTemp);

  String chat_id2 = "308137514";
//  Serial.print("Humidity (%): ");
//  Serial.println((float)DHT11.humidity, 2);
//  Serial.print("Temperature (oC): ");
//  Serial.println((float)DHT11.temperature, 2);
  float messageT1 = (float)DHT11.temperature;
//  Serial.println(messageT1);
//  Serial.println("Min: " + String(minTemp));
  Serial.println("Max: " + String(maxTemp));

delay( 100 );
  if(float(messageT1) > maxTemp || float(messageT1) < minTemp)
  {
    messageT = messageT1;
    bot.sendMessage(chat_id2, messageT, "");
  }

}
float readvoltage()
{
  String chat_id2 = "308137514"; // create string for ID that was recive message
  float val = 0 , bateryLevel = 0;
  digitalWrite(MosfetControlPIN, HIGH); // Turn on Mosfet
  delay(10);
  val = analogRead(BatteryLevelPIN);     // read the input pin
  val = val * ( 3.3 / 1023); // Convert to analog voltage
  bateryLevel = 3.2544 * val - 0.0846; // Calculate Battery Voltage
  bateryLevel = (bateryLevel-5.4)/2; // Calculate percentage 
  if (bateryLevel < 0.35) // check battery level under 35%
  {
    String temporary = "Battery Charge is : "; // create string for message
    bateryLevel = (bateryLevel*100); // conver to percent
    messageT = temporary +  bateryLevel ; // make message 
    bot.sendMessage(chat_id2, messageT, ""); // send message
  }
  digitalWrite(MosfetControlPIN, LOW); // Turn of Mosfet
  return bateryLevel;
  
}

void connectWiFi(){
    // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}
void sleepfuncion()
{
    Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
  //ESP.deepSleep(30e6); 
}
void initOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
//    Serial.printf("Error[%u]: ", error);
//    if (error == OTA_AUTH_ERROR) {
//      Serial.println("Auth Failed");
//    } else if (error == OTA_BEGIN_ERROR) {
//      Serial.println("Begin Failed");
//    } else if (error == OTA_CONNECT_ERROR) {
//      Serial.println("Connect Failed");
//    } else if (error == OTA_RECEIVE_ERROR) {
//      Serial.println("Receive Failed");
//    } else if (error == OTA_END_ERROR) {
//      Serial.println("End Failed");
//    }
  });
  ArduinoOTA.begin();
}
