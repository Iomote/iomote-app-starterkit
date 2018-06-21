#include <iomoteClass.h>
#include <Arduino.h>
#include <HTS221.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Please install the following libraries used on starter kit example:
// SmartEverything HTS221 by Seve Version 1.1.2
// LiquidCrystal I2C by Frank de Brabander Version 1.1.2

#define Serial SerialUSB // use Serial1 for external connector pins UART or SerialUSB for CDC USB Uart

// Iomote User LED
#define IOMOTE_LED	7

// User Messages buffer:
#define MAX_MSG_LEN  3750
char buff[MAX_MSG_LEN];

time_t now = 0;
int16_t pend = 0, old_pend = 0;
int adcValue = 0;
double humData, tempData;
void printAdcOnDisplay(int adcVal);
void printTempHumOnDisplay(float t, float h);
void printPendOnDisplay(int16_t pend);
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

void setup() 
{
  Iomote.begin("starter kit t&h", 1,0,4);

  // Wait up to 2 seconds for PC cable connection...  
  SerialUSB.begin(115200);
	int maxSerialWaiting = 100;//2000;
	while((!SerialUSB) && (maxSerialWaiting > 0))
	{
		maxSerialWaiting--;
		delay(1);
	}
  Serial.println("Sensors to Azure App");

  // Initialise I2C communication as MASTER 
  Wire.begin();

  // Initialise HTS221 sensor
  smeHumidity.begin();
	pinMode(IOMOTE_LED, OUTPUT);
	digitalWrite(IOMOTE_LED, true);

  int8_t res = Iomote.devKey(buff);
  if(res == 0)
  {
    Serial.write("Dev Key: ");
    Serial.println(buff);
  }

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

//lcd.print("                    "); // 20 blank chars
  lcd.print("Sensors to Azure App");

  Iomote.messagesPending(&pend);
  old_pend = pend;

  printPendOnDisplay(pend);

  // Read sensor data...
  // Humidity:
  humData = smeHumidity.readHumidity();
  Serial.print("Humidity   : ");
  Serial.print(humData);
  Serial.println(" %");
  // Temperature:
  tempData = smeHumidity.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(tempData);
  Serial.println(" *C");

  printTempHumOnDisplay(tempData, humData);  
}


void loop()
{
  
  Iomote.messagesPending(&pend);
  int adcCurrentValue = analogRead(1);

  if(adcValue != adcCurrentValue)
  {
    adcValue = adcCurrentValue;
    printAdcOnDisplay(adcValue);
  }

  if(old_pend != pend)
  {

    old_pend = pend;

    Serial.write("Messages on sending queue: ");
    Serial.println(pend);

    printPendOnDisplay(pend);
  }

  if(Iomote.rtc.getEpoch() != now)
  {
    now = Iomote.rtc.getEpoch();
    // Read sensor data...// Humidity:
    humData = smeHumidity.readHumidity();
    Serial.print("Humidity   : ");
    Serial.print(humData);
    Serial.println(" %");
    // Temperature:
    tempData = smeHumidity.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(tempData);
    Serial.println(" *C");
    printTempHumOnDisplay(tempData, humData); 
  }

  if(Iomote.buttonRead() == 0)
  {    
    // Clear message buffer
    memset(&buff, '\0', MAX_MSG_LEN); 
    
    // Prepare data to be sent on Cloud:
    sprintf(buff, "{\"hum\":%f,\"temp\":%f}", humData, tempData);
    
    // Add message to sending Queue:
    int8_t sendRes = Iomote.sendMessage(buff);
    if(sendRes == 0)
    {
      Serial.println("Data sent!");
    }
    else
    {
      Serial.write("Problems sending data: ");
      Serial.println(sendRes);
    }
    // Perform a small pause with blink effect:
    digitalWrite(IOMOTE_LED, LOW);
    delay(250);
    digitalWrite(IOMOTE_LED, HIGH);
    delay(250);              
    digitalWrite(IOMOTE_LED, LOW);
    delay(250);
    digitalWrite(IOMOTE_LED, HIGH);
    delay(250);              
  }
}

void printAdcOnDisplay(int adcVal)
{
  // calculate bar graph items
  char barTicks[21] = "adc:[              ]";
  float ticksCnt = adcVal * 15 / 4096;
  int ticksTot = (int)ticksCnt;
  if(ticksTot > 14)
    ticksTot = 14;
  for(int i = 0; i < ticksTot; i++)
    barTicks[i+5] = 255;
  lcd.setCursor(0,2);
  lcd.print(barTicks);
}

void printTempHumOnDisplay(float t, float h)
{
  lcd.setCursor(0,1); // second row
  // clear row
  lcd.print("                    "); // 20 blank chars
  lcd.setCursor(0,1);
  lcd.print("t:");
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C   h:");
  lcd.print(h);
  lcd.print("%");
}

void printPendOnDisplay(int16_t pend)
{

    lcd.setCursor(0,3); // third row
    // clear row
    lcd.print("                    "); // 20 blank chars
    lcd.setCursor(0,3);
    lcd.print("Pending msgs:"); 
    lcd.print(pend);
}
