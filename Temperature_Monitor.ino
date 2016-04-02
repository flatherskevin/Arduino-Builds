//Include LCD library and initialize pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,8,9,10,11,12);

long iteration = 1;           //Initialize first iteration
double avgTemp = 0;           //Initialize null average temperature
double totalTemp = 0;         //Initialize null total temperature
double maxTemp = 0;           //Initialize null maximum temperature
double minTemp = 1000;        //Initialize null minimum temperature
const int buttonUpPin = 2;    //Pin button Up is wired to
int buttonUpState = 0;        //Initital button Up state
const int buttonDownPin = 1;  //Pin button Down is wired to
int buttonDownState = 0;      //Initital button Down state
int screenState = 1;

void setup(){
  lcd.begin(16,2);                            //Screen has 16 columns and 2 rows
  //analogReference(INTERNAL);                //Initializes LM35 temperature sensor
  //Serial.begin(9600);                       //Bit rate for LM35
  /*lcd.setCursor(0,0);
  lcd.write("Cur:");                           //Current temperature
  lcd.setCursor(8,0);
  lcd.write("Avg:");                           //Average temperature
  lcd.setCursor(0,1);
  lcd.write("Max:");                           //Maximum temperature
  lcd.setCursor(8,1);
  lcd.write("Min:");                           //Minimum temperature*/
  pinMode(buttonUpPin,INPUT);                  //Button Up is an input
  pinMode(buttonDownPin,INPUT);                //Button Down is an input
}

//Reads the temperature and outputs it in Celcius. 9.31 is a division factor from volts to Celcius
double TempRead(){
  int tempPin = 0;
  double voltage = analogRead(tempPin);
  //5V has a span of 1024, and 5000 milliVolts in a V
  double millivolts = (voltage / 1024) * 5000;
  double temp = millivolts / 10;
  return temp;
}

void ScreenDisplay(double tempC, double avgTemp, double maxi, double mini){
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  if (buttonUpState == HIGH && screenState < 1)
      screenState++;
  if (buttonDownState == HIGH && screenState > -1)
      screenState--;
  switch (screenState)
    {
      case 1:
        lcd.setCursor(0,0);
        lcd.write("Cur:");              //Current temperature
        lcd.setCursor(4,0);             
        lcd.print(tempC,2);
        lcd.setCursor(0,1);
        lcd.write("Avg:");              //Average temperature
        lcd.setCursor(4,1);             
        lcd.print(avgTemp,2);
      case 0:
        lcd.setCursor(0,0);
        lcd.write("Avg:");              //Average temperature
        lcd.setCursor(4,0);             
        lcd.print(avgTemp,2);
        lcd.setCursor(0,1);
        lcd.write("Max:");              //Maximum temperature
        lcd.setCursor(4,1);             
        lcd.print(maxi,2);
      case -1:
        lcd.setCursor(0,0);
        lcd.write("Max:");              //Maximum temperature
        lcd.setCursor(4,0);             
        lcd.print(maxi,2);
        lcd.setCursor(0,1);
        lcd.write("Min:");              //Minimum temperature
        lcd.setCursor(4,1);             
        lcd.print(mini,2);
    }
}

//Finds the maximum temperature and prints to the LCD
double TempMax(double tempC, double maxi){
  if (tempC > maxi)
    {
     maxi = tempC; 
    }
  //lcd.setCursor(4,1);
  //lcd.print(maxi,1);
  return maxi;
}

//Finds the minimum temperature and prints to the LCD
double TempMin(double tempC, double mini){
  if (tempC < mini)
    {
     mini = tempC; 
    }
  //lcd.setCursor(12,1);
  //lcd.print(mini,1);
  return mini;
}

//Calculates the average temperature throughout entire operation and prints to the LCD
double TempAvg(double total, long iteration){
  double avgTemp;
  avgTemp = (total) / iteration;
  //lcd.setCursor(12,0);
  //lcd.print(avgTemp,1);
  return avgTemp;
}


void loop(){
  //double tempCc[11]={10, 20, 30, 40, 50,30,45,22,33,12,16};
  //double tempC = tempCc[iteration - 1];
  double tempC = TempRead();
  //lcd.setCursor(4,0);
  //lcd.print(tempC,1);
  minTemp = TempMin(tempC, minTemp);
  maxTemp = TempMax(tempC, maxTemp);
  totalTemp = totalTemp + tempC;
  avgTemp = TempAvg(totalTemp, iteration);
  ScreenDisplay(tempC, avgTemp, maxTemp, minTemp);
  iteration++;
  delay(200);
}
