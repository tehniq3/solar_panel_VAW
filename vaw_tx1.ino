/* 
 *  http://arduinotehniq.blogspot.com
 *  ver.1 - adapt sketch for my schematic and made 10 measurements instead one
 *  ver.2 -  made 100 measurements instead 10
 *  var.3 - add button for change display style (nothing, voltage, current, power, all)
 *  ver.3a - add clear screen after a time
 *  ver.3b - change style for computed current for be stable value
 *  ver.3b1 - turn to old computed current for be stable value
 *  ver.3b2 - add arithmetic average between old value and actual measured value
 *  ver. v_a_w_tx_1, add Serial send to another uC
  */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 7); // RX, TX

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
unsigned long tptrimitere = 30000; // time for display something
unsigned long momenttrimitere; // store time 

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

void setup() {
Serial.begin(9600);  
mySerial.begin(9600);
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
 /*
 Serial.print("amp = ");
 Serial.print(amp);
 Serial.print(" amp0 = ");
 Serial.println(amp0);
 */
 amps = (float)((amp - acs0) * 5.0 / 1024.0 / 0.066);  // asc712-30 - 66mV/A
if (amps < 0) amps = 0;
 watt = (float)(amps * vin);

 Serial.print(vin);
 Serial.print("V, ");
 Serial.print(amps);
 Serial.println("A");

if (millis() - momenttrimitere > tptrimitere)
{
int v0 = 100*vin; // 18.57V => 1857
int v1 = v0/1000; // v1 = 1
int vr = v0%1000; // vr = 857
int v2 = vr/100;  // v2 = 8
vr = vr%100;      // vr = 57
int v3 = vr/10;   // v3 = 5
int v4 = vr%10;   // v4 = 7

int a0 = 100*amps; // 21.34A => 2134
int a1 = a0/1000;  // a1 = 2
int ar = a0%1000;  // ar = 134
int a2 = ar/100;   // a2 = 1
ar = ar%100;       // ar = 34
int a3 = ar/10;    // a3 = 3
int a4 = ar%10;    // a4 = 4

Serial.print("--------------");
Serial.print("*");
Serial.print(v1);
Serial.print(v2);
Serial.print(v3);
Serial.print(v4);
Serial.print(a1);
Serial.print(a2);
Serial.print(a3);
Serial.print(a4);
Serial.print("#");
Serial.println("--------------");

//------Sending Data to receiver--------//
    mySerial.print('*'); // Starting char
    mySerial.print(v1); 
    mySerial.print(v2); 
    mySerial.print(v3); 
    mySerial.print(v4); 
    mySerial.print(a1); 
    mySerial.print(a2); 
    mySerial.print(a3); 
    mySerial.print(a4); 
    mySerial.print('#'); // Ending char
    //------------------------------------//
  Serial.println("tx");
   momenttrimitere = millis();
} // end transmission

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
