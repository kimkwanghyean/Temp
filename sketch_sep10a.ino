#include <SoftwareSerial.h>  
#include <OneWire.h>
#define BT_RXD 8
#define BT_TXD 7 
#include <SoftwareSerial.h>        // 블루투스 시리얼 통신 라이브러리 추가
int DS18S20_Pin = 2;                             //온도센서를 2번 핀으로 연결
SoftwareSerial bluetooth(BT_RXD, BT_TXD);        // 블루투스 설정 BTSerial(Tx, Rx) 
OneWire ds(DS18S20_Pin);                         //2번 핀과 연결되 OneWire 객체 생성
int piezo = 13;

void setup(){
  pinMode(piezo, OUTPUT);
  Serial.begin(9600);                           // 시리얼 통신, 속도는 9600
  bluetooth.begin(9600);                         // 블루투스 통신 시작 
  noTone(piezo); 
} 
 
void loop(){
  float temperature = getTemp();                 //온도 측정 후 변수에 저장
  Serial.println(temperature);
  
  if( 36 < temperature){                          //온도가 24도 이하일 때, 파란색 LED 점등
    tone(piezo, 523, 3000);
  }  
 else if ( 34 > temperature){
    tone(piezo, 523, 3000);
  }
  else{
     noTone(piezo); 
    }
  

  if (bluetooth.available()) {        // 블루투스에서 보낸 내용은 시리얼모니터로 전송
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {           // 시리얼모니터에서 보낸 내용은 블루투스로 전송
    bluetooth.write(Serial.read());
  } 


if ( bluetooth.available() ) 
  {
    Serial.print((char)bluetooth.read());
  }
  else
  {
    delay( 1000 );
  }
  // 블루투스 송신
  if (Serial.available()) { 
    //시리얼 모니터에서 입력된 값을 송신
    char toSend = (char)Serial.read();
    bluetooth.print(toSend);
  }

   bluetooth.print("온도 :  ");
   bluetooth.print(getTemp());
   bluetooth.println("도"); 

}
 
float getTemp(){                                   //온도 측정 후 반환하는 함수
 byte data[12];
 byte addr[8];
 if ( !ds.search(addr)) {
   ds.reset_search();
   return -1000;
 }
 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }
 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1);                                   
 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); 
 
 for (int i = 0; i < 9; i++) { 
  data[i] = ds.read();                                                          
 }
 
 ds.reset_search(); 
 byte MSB = data[1];
 byte LSB = data[0];
 float tempRead = ((MSB << 8) | LSB); 
 float TemperatureSum = tempRead / 16; 
 return TemperatureSum;  

 





















                                                                   
}
