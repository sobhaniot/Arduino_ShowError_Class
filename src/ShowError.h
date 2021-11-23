//class for timer counter with millis
#ifndef ShowError_h
#define ShowError_h
#include <Arduino.h>

class ShowError {

  public:
    ShowError(uint8_t pin, int OnTime, int OffTime, int waitTime, uint8_t HighLow);
    void StartError(uint8_t errorCode);
    void StopError(uint8_t errorCode);
    void HandleError();

  private:
    bool wait(int waitTime);
    void doBlink();
    void doWaitBetweenBlink(uint8_t &numBlink);
    void checkErrorCount();

#define START_BLINK       0
#define BLINK_ON          1
#define BLINK_OFF         2
#define FINISH_BLINK      3
#define WAIT_BETWEEN      9

    uint8_t _pin;
    unsigned long _previousTime;
    unsigned long _startCounting;
    uint8_t _errorActiveList = 0;
    uint8_t _countblinkList[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t flagErrorCount = 0;
    int _OnTime;
    int _OffTime;
    int _waitTime;
    uint8_t _flagWaitOnOff = START_BLINK;
    uint8_t _HighLow;
};

#endif

ShowError::ShowError(uint8_t pin, int OnTime = 200, int OffTime = 200, int waitTime = 2000, uint8_t HighLow = HIGH) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin,~_HighLow);
  _pin = pin;
  _OnTime = OnTime;
  _OffTime = OffTime;
  _waitTime = waitTime;
  _previousTime = millis();
  _startCounting = millis();
  _HighLow = HighLow;
}

bool ShowError::wait(int waitTime) {
  unsigned long diff = millis() - _previousTime;
  if (diff > waitTime) {
    _previousTime = millis();
    return true;
  } else {
    return false;
  }
}



void ShowError::doBlink() {
  if (_flagWaitOnOff == START_BLINK) {
    if (wait(_OffTime)) {
      digitalWrite(_pin, _HighLow);
      _flagWaitOnOff = BLINK_ON;       // raft be marhaleye ON
      //Serial.println("HIGHT");
    }
  }
  if (_flagWaitOnOff == BLINK_ON) {
    if (wait(_OnTime)) {
      digitalWrite(_pin, ~_HighLow);
      _flagWaitOnOff = BLINK_OFF;       // raft be marhaleye payane blink
      //Serial.println("LOW");
    }
  }
  if (_flagWaitOnOff == FINISH_BLINK) {
    digitalWrite(_pin, ~_HighLow);
    if (wait(_waitTime)) {
      _flagWaitOnOff = WAIT_BETWEEN;       //finish one period of show error
    }
  }
}
void ShowError::doWaitBetweenBlink(uint8_t &numBlink) {
  if (_flagWaitOnOff == START_BLINK || _flagWaitOnOff == BLINK_ON) {
    doBlink();
  }
  if (_flagWaitOnOff == BLINK_OFF) {
    numBlink -= 1;
    if (numBlink == 0) {
      _flagWaitOnOff = FINISH_BLINK;    // raft be marhaleye WAIT
    } else {
      _flagWaitOnOff = START_BLINK;
    }
  }
  if (_flagWaitOnOff == FINISH_BLINK) {
    doBlink();
  }
}
void ShowError::checkErrorCount() {
  if (_flagWaitOnOff == WAIT_BETWEEN) {
    flagErrorCount++;
    if (flagErrorCount > 7) flagErrorCount = 0;
  }

  if (_countblinkList[flagErrorCount]) {
    if (_flagWaitOnOff == WAIT_BETWEEN) _flagWaitOnOff = START_BLINK;
    doWaitBetweenBlink(_countblinkList[flagErrorCount]);
  } else {
    _flagWaitOnOff = WAIT_BETWEEN;
  }
}

void ShowError::HandleError() {
  for (int i = 0; i < 8; i++) { // inghesmat agar shomarandeye blink sefr shavad vali hanooz code error faal bashad dobare jaygozin mikonad ta error ra namayesh dahad

    if ( !_countblinkList[i] && (_errorActiveList & (B00000001 << i))) {
      _countblinkList[i] = i + 1;
    }
  }
  checkErrorCount();
}
void ShowError::StartError(uint8_t errorCode) {
  Serial.println(errorCode);
  if (errorCode > 0 && errorCode < 9) {
    _errorActiveList |= B00000001 << (errorCode - 1);
//    Serial.println(_errorActiveList, BIN);
  } else {
    Serial.println("invalid number for error");
  }
}


void ShowError::StopError(uint8_t errorCode) {
  if (errorCode > 0 && errorCode < 9) {
    _errorActiveList &= ~(B00000001 << (errorCode - 1));
    _flagWaitOnOff = START_BLINK;
    flagErrorCount = 0;
    //Serial.println(_errorActiveList, BIN);
  } else {
    Serial.println("invalid number for error");
  }
}
