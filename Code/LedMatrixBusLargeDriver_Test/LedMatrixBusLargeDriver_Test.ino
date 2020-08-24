const struct Pins {
    uint8_t ClkOut = 13;
    uint8_t ClkSelEn = 12;
    uint8_t DataOut = 11;
    uint8_t ClkEn = 10;
    uint8_t LatchOut = 9;
    uint8_t LedOeOut = 8;
} pins;

bool clkVal = 1;
int counter = 0;
bool ledVal = 0;

void setup() {
    pinMode(pins.ClkOut, OUTPUT);
    pinMode(pins.ClkSelEn, OUTPUT);
    pinMode(pins.DataOut, OUTPUT);
    pinMode(pins.ClkEn, OUTPUT);
    pinMode(pins.LatchOut, OUTPUT);
    pinMode(pins.LedOeOut, OUTPUT);

    // 0
    digitalWrite(pins.ClkOut, 1);
    digitalWrite(pins.ClkSelEn, 0);
    digitalWrite(pins.DataOut, 1);
    digitalWrite(pins.ClkEn, 0);
    digitalWrite(pins.LatchOut, 0);
    digitalWrite(pins.LedOeOut, 0);
}

void loop() {
  // 1
  digitalWrite(pins.ClkEn, 1);

  // 2
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 0); 

  // 3
  digitalWrite(pins.ClkEn, 1);

  // 4 start panel select
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);

  // 5
  digitalWrite(pins.ClkEn, 1);

  // 6
  digitalWrite(pins.ClkEn, 0);
  
  // 7
  digitalWrite(pins.ClkEn, 1);
  
  // 8
  digitalWrite(pins.ClkEn, 0);

  // 9
  digitalWrite(pins.ClkEn, 1);

  // 10
  digitalWrite(pins.ClkEn, 0);

  // 11
  digitalWrite(pins.ClkEn, 1);

  // 12
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 0);
  
  // 13
  digitalWrite(pins.ClkEn, 1);

  // 14
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);

  // 15 start led data
  digitalWrite(pins.ClkEn, 1);
  digitalWrite(pins.ClkOut, 0);

  // 16
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);
  
  // 17
  digitalWrite(pins.ClkEn, 1);

  // 18
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 0);

  // 19
  digitalWrite(pins.ClkEn, 1);
  
  // 20
  digitalWrite(pins.ClkEn, 0);

  // 21
  digitalWrite(pins.ClkEn, 1);

  // 22
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);

  // 23
  digitalWrite(pins.ClkEn, 1);

  // 24
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 0);

  // 25
  digitalWrite(pins.ClkEn, 1);

  // 26
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);

  // 27
  digitalWrite(pins.ClkEn, 1);

  // 28
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.DataOut, 1);

  // 29
  digitalWrite(pins.ClkEn, 1);
  
  // 30
  digitalWrite(pins.ClkEn, 0);

  // 31
  digitalWrite(pins.ClkEn, 1);

  // 32
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.ClkSelEn, 1);

  // 33
  digitalWrite(pins.ClkEn, 1);
  digitalWrite(pins.ClkSelEn, 0);
  digitalWrite(pins.LatchOut, 1);
  
  // 34
  digitalWrite(pins.ClkEn, 0);
  digitalWrite(pins.LatchOut, 0);

  delay(1000);
}
