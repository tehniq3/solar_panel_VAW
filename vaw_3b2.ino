/* 
 *  voltmeter, ammmeter and wattmeter based on "3D Printed Arduino OLED Watt Meter" 
 *  from https://www.instructables.com/id/3D-Printed-Arduino-OLED-Watt-Meter/
 *  small changes made by Nicu FLORICA (niq_ro) 
 *  http://www.arduinotehniq.com
 *  https://nicuflorica.blogspot.com
 *  http://arduinotehniq.blogspot.com
 *  ver.1 - adapt sketch for my schematic and made 10 measurements instead one
 *  ver.2 -  made 100 measurements instead 10
 *  var.3 - add button for change display style (nothing, voltage, current, power, all)
 *  ver.3a - add clear screen after a time
 *  ver.3b - change style for computed current for be stable value
 *  ver.3b1 - turn to old computed current for be stable value
 *  ver.3b2 - add arithmetic average between old value and actual measured value
  */

#include "U8glib.h"
#define vinput A1
#define ainput A0
#define buton 2

float vout = 0.0;
float vin = 0.0;
float R1 = 39000.0; // resistance of R1 (39K)
float R2 = 1000.0; // resistance of R2 (1K) 
float value = 0;
int value0 = 0;
float amp = 0;
int amp0 = 0;
float watt = 0.0;
float amps = 0.0;
float kv = 0.97;
float ka = 1.0;
float acs0 = 514.35;  // adjust it until you see 0.00A without consuption
//int acs0 = 513;
byte stare = 0;
float amp1;

unsigned long tpaprindere = 15000; // time for display something
unsigned long momentaprindere; // store time 

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

void setup() {
Serial.begin(9600);  
pinMode(ainput, INPUT);
pinMode(vinput, INPUT);
pinMode(buton, INPUT);
digitalWrite(buton, HIGH);
}

void loop() { 

if (digitalRead(buton) == LOW)
{
  stare = stare + 1;
  if (stare > 4) stare = 1;
  delay(200);
  momentaprindere = millis();
}

if ((millis() - momentaprindere > tpaprindere) and (stare != 0))
{
  stare = 0;
}

if (stare == 1)
{
  u8g.firstPage();
  do {  
    draw4();    
  } while( u8g.nextPage() );
}
if (stare == 2)
{
  u8g.firstPage();
  do {  
    draw();    
  } while( u8g.nextPage() ); 
}
if (stare == 3)
{
   u8g.firstPage();
  do {  
    draw2();    
  } while( u8g.nextPage() ); 
}
if (stare == 4)
{
  u8g.firstPage();
  do { 
  draw3();    
  } while( u8g.nextPage() ); 
}
if (stare == 0)
{
   u8g.firstPage();
  do { 
  gol();    
  } while( u8g.nextPage() ); 
} 
// delay(100); 
//    u8g.firstPage();
    
 value = 0;
 amp = 0;
 for (byte i = 0; i < 100; i++) 
  {
 value0 = analogRead(vinput);
 delay(1);
 amp0 = analogRead(ainput);
 delay(1);
 value = value + value0;
 amp = amp + amp0;
  }
 value = (float)value/100.;
 
 amp = (float)amp/100.+0.5;
// amp = (float)amp/100.+0.5;

amp = (amp + amp1)/2;
amp1 = amp;
 
 vout = (float)kv * (value * 5.0) / 1024.0;
 vin = (float)vout / (R2/(R1+R2));  
 
 Serial.print("amp = ");
 Serial.print(amp);
 Serial.print(" amp0 = ");
 Serial.println(amp0);
 amps = (float)((amp - acs0) * 5.0 / 1024.0 / 0.066);  // asc712-30 - 66mV/A
if (amps < 0) amps = 0;
 watt = (float)(amps * vin);
}  // end main loop

void draw(){
  u8g.setFont(u8g_font_profont17r);
   u8g.drawStr(31, 12, "VOLTS"); 
   u8g.setFont(u8g_font_profont29r); 
  u8g.setPrintPos(26,45);
  u8g.drawRFrame(3, 18, 125, 35, 10);  
  u8g.println(vin);                     
  u8g.println("V");  
}
void draw2(){
  u8g.setFont(u8g_font_profont17r);
   u8g.drawStr(38, 12, "AMPS"); 
    u8g.setFont(u8g_font_profont29r);
  u8g.setPrintPos(26,45);
  u8g.drawRFrame(3, 18, 125, 35, 10);    
  u8g.println(amps);                        
  u8g.println("A");  
}
void draw3(){
  u8g.setFont(u8g_font_profont17r);
   u8g.drawStr(31, 12, "WATTS"); 
    u8g.setFont(u8g_font_profont29r);
  u8g.setPrintPos(26,45);
  u8g.drawRFrame(3, 18, 125, 35, 10);   
  u8g.println(watt);                      
  u8g.println("W");  
}

void draw4(){
  u8g.setFont(u8g_font_profont17r);
//   u8g.drawStr(31, 12, "WATTS"); 
//    u8g.setFont(u8g_font_profont29r);
  u8g.setPrintPos(10,20); 
// u8g.print("  ");
if (vin < 100) u8g.print(" ");  
  if (vin < 10) u8g.print(" ");     
  u8g.print(vin);               
  u8g.println("V"); 
  u8g.setPrintPos(10,40);
//  u8g.print("  ");
  if (amps < 100) u8g.print(" ");
  if (amps < 10) u8g.print(" ");
  u8g.print(amps);                        
  u8g.println("A"); 
  u8g.setPrintPos(10,60); 
//  u8g.print(" ");
  if (watt < 100) u8g.print(" ");
  if (watt < 10) u8g.print(" "); 
  u8g.print(watt);                      
  u8g.println("W");  
}

void gol() // cleen display
{ 
}
