//PIN DEFINE
  int enable = 2;
  int forward = 5;
  int reverse = 3;
  int selector_in = A0;
  int light_in = A1;
  int light_mod = A2;
  
//Selector Values  
  double selector_raw = 0;
  int selector_state = 0;
  double state_Off = 540; //500
  double state_Auto = 620; //600
  double state_Fwrd = 695; //670
  double state_Rvrs = 745; // 720

//Light sensor values
  double light_raw = 0;
  double dark_setting = 1023;
  bool Daylight = false;
  double light_mod_raw = 0;

 //Iteration control
  int i = 0;
  int rampTime = 30;
  int flatTime = 20000;
  int power = 0;
  int max_power = 255;  //max = 255
  int Door_Status = 0; //Starts closed

void Selector_Read(){
selector_raw = analogRead(selector_in); 
if (selector_raw < state_Off){
  selector_state = 0;
}else{if(selector_raw < state_Auto){
  selector_state = 1;
}else{if(selector_raw < state_Fwrd){
  selector_state = 2;
}else{if(selector_raw < state_Rvrs){
  selector_state = 3;}}}}}

void Light_Read(){
light_raw = analogRead(light_in);
light_mod_raw = analogRead(light_mod);
dark_setting = 323 + 700 * light_mod_raw / 1023;
Serial.println(light_raw);
Serial.println(dark_setting);
if(light_raw > dark_setting){
  Daylight = false;
}else{Daylight = true;}}

void Close_Door(){
digitalWrite(forward, LOW);
for(double m = 0; m < 3.14 / 2; m += .01){ //Start ramp
  power = 255*sin(m);
  delay(rampTime);
  analogWrite(reverse,power);}
  delay(flatTime);
for(double m = 3.14 / 2; m < 3.14; m += .01){ //Stop ramp
  power = 255*sin(m);
  delay(rampTime);
  analogWrite(reverse,power);
  Door_Status = 0;}
} //CLOSED

void Open_Door(){
digitalWrite(reverse, LOW);
for(double m = 0; m < 3.14 / 2; m += .01){ 
  power = max_power*sin(m);
  delay(rampTime);
  analogWrite(forward,power);}
  delay(flatTime);
for(double m = 3.14 / 2; m < 3.14; m += .01){ 
  power = max_power*sin(m);
  delay(rampTime);
  analogWrite(forward,power);  
  Door_Status = 1;}
} //OPENED


void setup() {
Serial.begin(9600);
pinMode(forward, OUTPUT);
pinMode(reverse, OUTPUT);
pinMode(enable, OUTPUT);
pinMode(light_mod, INPUT);
digitalWrite(enable, HIGH);
Light_Read();
Close_Door();
}

void loop() {
int selector_temp = selector_state;
bool Daylight_temp = Daylight;

Selector_Read();
if(selector_temp == selector_state && selector_state != 1 || selector_state == 0){
delay(2000);
}
  else{
    if(selector_state == 1){
      Light_Read();
      if(Daylight_temp == Daylight){
      delay(2000);}
        else{
          if(Door_Status == 0){
            Open_Door();
          }
            else{
              Close_Door();
            }}}
      else{if(selector_state == 2){
        Open_Door();}
        else{Close_Door();}}}
      
    }
      
    
