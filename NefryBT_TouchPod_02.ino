#include <Nefry.h>
#include <NefryDisplay.h>
#include <NefrySetting.h>

void setting(){
  Nefry.disableDisplayStatus();
}

NefrySetting nefrySetting(setting);

HardwareSerial Serial1(1);  // use HardwareSerial, do not use SoftwareSerial

byte cmd[1] = {0x6D};  // データ送信コマンド(0x6d)
unsigned char res[8];  // データ受信バッファ
bool flag = false;

void setup() {
  Nefry.setLed(0,0,0);
  NefryDisplay.clear();
  NefryDisplay.setFont(ArialMT_Plain_16);
  NefryDisplay.drawString(0, 0, "CH1: ");
  NefryDisplay.drawString(0, 16, "CH2: ");
  NefryDisplay.drawString(0, 32, "CH3: ");
  NefryDisplay.drawString(0, 48, "CH4: ");
  NefryDisplay.display();
  Serial.begin(115200);
  Serial1.begin(57600, SERIAL_8N1, 23, 19);  // (speed, type, RX:D2=23, TX:D3=19);
  delay(1000);
  Serial.println("start: ");
}

void loop() {
  Serial1.write(cmd,1);
  while(Serial1.available()>0)
  {
    Serial1.readBytes(res, 8);
    flag=true;
  }
  if(flag)
  {
    flag=false;
    unsigned int P01H = (unsigned int) res[0];  // CH1 AD変換値 上位2bit/10bit (使用しない)
    unsigned int P01L = (unsigned int) res[1];  // CH1 AD変換値 下位8bit/10bit (使用する)
    unsigned int P04H = (unsigned int) res[2];  // CH4 AD変換値 上位2bit/10bit (使用しない)
    unsigned int P04L = (unsigned int) res[3];  // CH4 AD変換値 下位8bit/10bit (使用する)
    unsigned int P02H = (unsigned int) res[4];  // CH2 AD変換値 上位2bit/10bit (使用しない)
    unsigned int P02L = (unsigned int) res[5];  // CH2 AD変換値 下位8bit/10bit (使用する)
    unsigned int P03H = (unsigned int) res[6];  // CH3 AD変換値 上位2bit/10bit (使用しない)
    unsigned int P03L = (unsigned int) res[7];  // CH3 AD変換値 下位8bit/10bit (使用する)

    Serial.print(P01L);
    Serial.print(",");
    Serial.print(P02L);
    Serial.print(",");
    Serial.print(P03L);
    Serial.print(",");
    Serial.print(P04L);
    Serial.println("");

    char buf[10];
    NefryDisplay.clear();
    NefryDisplay.setFont(ArialMT_Plain_16);
    NefryDisplay.drawString(0, 0, "CH1: ");
    sprintf(buf, "%d", P01L);
    NefryDisplay.drawString(50, 0, buf);
    NefryDisplay.drawString(0, 16, "CH2: ");
    sprintf(buf, "%d", P02L);
    NefryDisplay.drawString(50, 16, buf);
    NefryDisplay.drawString(0, 32, "CH3: ");
    sprintf(buf, "%d", P03L);
    NefryDisplay.drawString(50, 32, buf);
    NefryDisplay.drawString(0, 48, "CH4: ");
    sprintf(buf, "%d", P04L);
    NefryDisplay.drawString(50, 48, buf);
    NefryDisplay.display();
  
  }
  delay(100);
}

