#include <WiFi.h>
#include <FirebaseESP32.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <PZEM004Tv30.h>

/* Hardware Serial2 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif

//1. Change the following info
#define FIREBASE_HOST "smart-energy-meter-526b3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyA9BSbKMZGNysMPKEoQccnF2-p5zKQ5C0Q"
#define WIFI_SSID "Mainak S20Fe"
#define WIFI_PASSWORD "7585937979Mm"


//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;


float voltage,current,power,energy,frequency,pf,amount=7.30;

void setup() {
    Serial.begin(9600);
    lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  

    // Uncomment in order to reset the internal energy counter
    // pzem.resetEnergy()
    
/*  For WiFi connection*/
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


   //3. Set your Firebase info

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);


  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("SmartEnergyMeter");
  lcd.setCursor(0,1);
  
  lcd.print("------AAMPS-----");
  delay(4000);
  lcd.clear();
}

void loop() {
        
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    energy = pzem.energy();
    frequency = pzem.frequency();
    pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("Power Factor: ");           Serial.println(pf);
        Serial.print("Bill: Rs. ");     Serial.print(energy * amount);Serial.print(" /-");

       String b = String(energy * amount);
       String v = String(voltage);
       String p = String(power);
       String c = String(current);
       String pof = String(pf);
       lcd.setCursor(0,0);
       lcd.print("Rs ");
       lcd.setCursor(3,0);
       lcd.print(b);
       lcd.setCursor(9,0);
       lcd.print("pf "+pof);

       lcd.setCursor(0,1);
       lcd.print(v+"V"+" "+c+"A");
       //lcd.clear();

       //delay(3000);
    }

    Serial.println();
    Serial.println();
    delay(1000);

   //Firebase.setFloat(firebaseData, "/Voltage", voltage)

    //voltage update
    if(Firebase.setFloat(firebaseData, "/Voltage", voltage))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");

   //current update
    if(Firebase.setFloat(firebaseData, "/Current", current))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, "); 

   //power update
    if(Firebase.setFloat(firebaseData, "/Power", power))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");   


   //Energy update
    if(Firebase.setFloat(firebaseData, "/Energy", energy))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");     
   
  //frequency update
    if(Firebase.setFloat(firebaseData, "/Frequency", frequency))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");

  //PowerFactor update
    if(Firebase.setFloat(firebaseData, "/Power_Factor", pf))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");

  //Bill update
    if(Firebase.setFloat(firebaseData, "/Bill", energy * amount))
          Serial.println("Set int data success");
    else
      Serial.print("Error in setFloat, ");

}
