/*

TEST TEST
Auto-plant-water-tron-o-matic

Every 30 minutes
Read the date and time
Read temperature from sensor
Read light from sensor
Read moisture from sensor
 if dry
  Activate relay to pump water to plant
Log date,time,temp,light,moisture,water to sd card in csv

*/

//#include <SD.h>   // read/write to SD card
//#include <Time.h> // keep track of time

const int relayPin        = 5; // this pin will drive transistor->relay->pump
const int tempPin         = 0; // analog read temp
const int lightPin        = 1; // analog read light
const int moisturePin     = 2; // analog read moisture
const int moistureVoltage = 3; // sent voltage to soil
//const int chipSelect      = 4;

const int readTime  = 10000; // time between sensor read
const int waterTime = 5000;// time to run pump

//int cardReady = 0;

void setup()
{
  Serial.begin(9600);
  /************************************************
  
  THIS IS REALLY IMPORTANT!!! DON'T FORGET
  
  /************************************************/
  
  // setTime(hr,min,sec,day,month,yr); // set the time
  //setTime(21,30,0,8,3,13);
  
  /************************************************
  
  SERIOUSLY ... SET THE TIME
  
  /***********************************************/
  
  pinMode(relayPin, OUTPUT); // relayPin is output
  pinMode(moistureVoltage, OUTPUT); // this is an output pin
  /*
  //DEBUG some SD card stuff
  Serial.print("Initializing SD card ...");
  // default chip select pin
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)){
    Serial.println("Card failed or not present");
    Serial.println("Data will not be logged");
  }
  else {
    Serial.println("Card initialized");
    cardReady = 1;
  }
  */
}

void loop()
{
  // get date and time
  //time_t t = now(); // get the current time, put it in t
  
  // read Temp
  float voltage, degreesC, degreesF;
  
  voltage = getVoltage(tempPin);
  degreesC = (voltage - 0.5) * 100;
  degreesF = degreesC * (9.0/5.0) +32;
  
  // DEBUG print temp to serial
  Serial.print(" C: ");
  Serial.print(degreesC);
  Serial.print(" F: ");
  Serial.println(degreesF);
  
  // read Light
  int lightLevel;
  lightLevel = analogRead(lightPin);
  // this is returning 0 for high light and 1023 for low light
  // maybe reverse it like this?
  // lightLevel = 1023 - analogRead(lightPin);
  
  // DEBUG print light to serial
  Serial.print(" Light: ");
  Serial.println(lightLevel);
    
  // read Moisture
  int moistureLevel;
  digitalWrite(moistureVoltage, HIGH); // start the sensor
  delay(1000); // wait a sec
  moistureLevel = analogRead(moisturePin); // read the value
  digitalWrite(moistureVoltage, LOW); // stop the sensor
  
  // DEBUG print moisture to serial
  Serial.print(" Moisture: ");
  Serial.println(moistureLevel);
  
  if(moistureLevel < 707){ // if the moisture is low, start the pump
    //
    // this value is from
    // http://www.instructables.com/id/Garduino-Gardening-Arduino/step7/Test-and-Calibrate-Your-Sensors/
    //
    digitalWrite(relayPin, HIGH); // turn on the pump
    delay(waterTime); // run the pump
    digitalWrite (relayPin, LOW); // turn off the pump
    
  }
  // log data
  /*
  // is the card ready?
  if (cardReady == 1){
    
  }
  else {
    
  }
  */  
  
  // wait until next read time
  delay(readTime);
  
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814); // turn the 0-1023 into a 0-5 actual voltage
}