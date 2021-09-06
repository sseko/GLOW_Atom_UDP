
#include "M5Atom.h"

#define LED_R 0x00ff00
#define LED_G 0xff0000
#define LED_B 0x0000ff
#define LED_Y 0xffff00
#define LED_OFF 0x000000
#define LED_W 0xffffff

void led_dispal_out(int x,int y, int col){
  int point = (y*5)+x;
  if(point<0) point = 0;
  if(point>24)point = 24;
  M5.dis.drawpix(point, col);
  delay(10);
}

void led_dispal_all(int col){
  for(int i=0;i<=24;i++){
    M5.dis.drawpix(i, col);
    delay(10);
  }
}

void setup()
{ 
  M5.begin(true, false, true);
  delay(10);  //delay10ms.
  led_dispal_all(LED_B);
  Serial.begin(115200);
  init_BLE("GlowSDK");
  delay(100);
  init_udp();
  delay(100);
  led_dispal_all(LED_OFF);
  led_dispal_out(2,2,LED_B);
}

void loop()
{
  int func;
  int group;
  int id;
  unsigned int comm;

  if(receive_udp(&func,&group,&id,&comm)){
    if(comm>0x7fff)comm=0;
    serial_display(func,group,id,comm);
    sendGlowData(func,group,id,comm);
    led_dispal_out(2,2,LED_Y);
  }
  if(receive_serial(&func,&group,&id,&comm)){
    if(comm>0x7fff)comm=0;
    serial_display(func,group,id,comm);
    sendGlowData(func,group,id,comm);
    led_dispal_out(2,2,LED_R);
  }

  if (M5.Btn.wasPressed()){ 
    sendGlowData(0,group,id,0x0);
    led_dispal_out(2,2,LED_B);
  }
  M5.update();
  delay(100);
}

void serial_display(int func,int group,int id,int comm){
  Serial.print("function = ");
  Serial.println(func);
  Serial.print("group    = ");
  Serial.println(group);
  Serial.print("id       = ");
  Serial.println(id);
  Serial.print("command  = ");
  Serial.println(comm);
}
