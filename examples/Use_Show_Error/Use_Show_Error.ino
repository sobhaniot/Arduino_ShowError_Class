#include "ShowError.h"
// two way declare instance
//ShowError T(LED PIN,ON TIME,OFF TIME,DELAY BETWEEN EACH ALARME,HIGH Active or LOW Active);
//ShowError T(LED PIN,default 200 ,default 200,default 2000,default HIGH);
ShowError T(LED_BUILTIN, 200, 200, 2000, LOW);
//ShowError T(LED_BUILTIN);

bool test[9];

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Start Test");
  Serial.println("Enter Error Code Betweeen 1 and 8");
}

void loop() {
  //You can input integer number range (1,8) in serial input

  T.HandleError();

  if (Serial.available() > 0) {
    uint8_t ErrorCode = (uint8_t)Serial.parseInt();
    if (ErrorCode > 0 && ErrorCode < 9) {
      if (test[ErrorCode]) {
        test[ErrorCode] = false;
        T.StopError(ErrorCode);
      } else {
        test[ErrorCode] = true;
        T.StartError(ErrorCode);
      }
      Serial.print("Error Code : ");
      Serial.println(ErrorCode);
    }
  }
  delay(5);
}
