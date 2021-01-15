//#include "WiFiModule.h"
#include <SoftwareSerial.h>

#define espBaud 9600

class URL{    
  public:
     char connectionType[4] = "TCP";
     char* ip_ptr;
     char* port_ptr;
     char* path_ptr;

     URL(char ip_p[], char port_p[], char path_p[]){
       ip_ptr = ip_p;
       port_ptr  = port_p;
       path_ptr = path_p;
     }
     
};

class WiFiModule {
  private:
    uint8_t RX = 2;
    uint8_t TX = 3; 
    SoftwareSerial* connectionPointer; //Creating a pointer -> will refer to the SoftwareSerial Object created in the constructor
    
    
  public: 
    char id[37];
    
    WiFiModule(uint8_t rx, uint8_t tx){
      RX = rx;
      TX = tx;

      SoftwareSerial sC(2,3);    //Initialising a SoftwareSerial Object
      connectionPointer = &sC;   //The pointer will point to the address of the sC Object -> The initialised SoftwareSerial Object
      pinMode(13,OUTPUT);
    };
    
    bool startUp(){
      SoftwareSerial serialConnection = *connectionPointer;
      
      digitalWrite(13,HIGH);
      delay(500);
      
      serialConnection.begin(espBaud);
      for(int i = 0; i < 10; i++){
        digitalWrite(13,HIGH);
        delay(50);
        digitalWrite(13,LOW);
        delay(50);
      }
      serialConnection.println("AT");


      return serialConnection.findUntil("OK","ERROR");
    }

    bool openConnection(){

      SoftwareSerial serialConnection = *connectionPointer;
      serialConnection.begin(espBaud);

      char *startCommand = "AT+CIPSTART=\"TCP\",\"192.168.2.115\",4000\r\n";
      serialConnection.write(startCommand);
      delay(200);

      return (serialConnection.findUntil("OK","NULL") || serialConnection.findUntil("ALREADY CONNECTED","ERROR"));
    }

    bool sendHeyThere(){
      
      if(!openConnection()){
        Serial.println("Failed to open connection");
        return false;   
      }
      
      SoftwareSerial serialConnection = *connectionPointer;
      serialConnection.begin(espBaud);

      char *command = "AT+CIPSEND=27\r\n";
      char *firstline = "GET /heyThere/ HTTP/1.1\r\n"; //23 
      char *blankline = "\r\n";
      
      serialConnection.print(command);
      serialConnection.setTimeout(2000);
      if(!serialConnection.findUntil(">","ERROR")){
        Serial.println("Failed to send http head");
        return false;
      }else{
        Serial.println("command sent");
      }
      
      serialConnection.write(firstline);
      delay(20);
      serialConnection.write(blankline);

      char response[208] = {}; //char response[255];
      bool endOfResponse = false;

      byte i = 0;
      while(!endOfResponse){
        while(serialConnection.available()){
          response[i] = serialConnection.read();
          //Serial.print(response[i]);
          if (response[i] == '}'){
            endOfResponse = true;
          }
          
          i++;
        }
      }

      // --- Snowflake --- Saving response in private variable
      //Saved response
      i = 0;
      while(response[i] != '{') i++; //Goning forward to the begin of the json
      while(response[i] != ':') i++; //Begin of ID String
      while(response[i] != '"') i++; 
      i++;                           
      
      for(int j = 0; j < 36; j++){
        id[j] = response[i];
        i++;
      }
      //Serial.println(*id);


      //Closing the connection
      serialConnection.print("AT+CIPCLOSE\r\n");
      return true;
      
    }

    bool sendUpdate(){
      SoftwareSerial serialConnection = *connectionPointer;
      Serial.println("Trying to send the update");
      
      if(!openConnection()){
        Serial.println("Failed to open connection");
        return false;   
      }
      Serial.print("opened the connection");
      
      serialConnection.print("AT+CIPSEND=94\r\n");

      serialConnection.setTimeout(2000);
      if(!serialConnection.findUntil(">","ERROR")){
        Serial.println("Failed to send http head");
        return false;
      }else{
        Serial.println("command sent");
      }
      
      char *firstline = "POST /updateStatus/ HTTP/1.1\r\n"; //30
      char *head = "Content-Length: 61\r\n";
      char *body = "{\"ID\" : \"fe064074-e92b-4bad-bb27-15b86ed502c06ed\",\"status\" : 2}\r\n"; //61
      char *blankline = "\r\n";
      //On Device

      serialConnection.print(firstline);
      Serial.print("head sent");
      serialConnection.print(blankline);
      serialConnection.print(body);
      
      char response[208] = {}; //char response[255];
      bool endOfResponse = false;

      byte i = 0;
      while(!endOfResponse){
        while(serialConnection.available()){
          response[i] = serialConnection.read();
          Serial.print(response[i]);
          if (response[i] == 'H'){
            endOfResponse = true;
          }
          
          i++;
        }
      }
    }
};

void setup(){
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("\n");
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  WiFiModule esp(2, 3);

  if(!esp.startUp()){
    Serial.print("Failed to startup");
  }
  esp.sendHeyThere();
  delay(1000);
  Serial.println("end");

  //esp.sendUpdate();
  // set the data rate for the SoftwareSerial portt
}

void loop(){ // run over and over
   
}
