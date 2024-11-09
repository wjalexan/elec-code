#include<Servo.h>

  Servo servoLeft;
  Servo servoRight;



  const int rightForward = 1300;
  const int leftForward = 1603;
  const int rightStopped = 1500;
  const int leftStopped = 1490;
  const int rightBackward = 1700;
  const int leftBackward = 1379;
  const int highFrequency = 41000; // WAS 41000
  const int lowFrequency = 38000;
  const int frontFreq = 40000;
  const int offSet = 65;
  bool finish = false;
  int turns = 0;
  bool turn = false;
  int timer = 0;
  bool firstTurn = true;
  bool lastTurn = false;



  const int leftLedPin=10, leftIrReceiverPin=11;   // Select these to match the IR LED/receiver pair that you are using
  const int rightLedPin=2, rightIrReceiverPin=3;
  const int midLedPin=6, midIrReceiverPin=7;
  // const int redLedPin = A0;                    // Select this to match the red LED next to the IR receiver you are using

  void setup()                                 // Built-in initialization block
  {
    pinMode(leftIrReceiverPin, INPUT);
    pinMode(leftLedPin, OUTPUT);
    pinMode(rightIrReceiverPin, INPUT);
    pinMode(rightLedPin, OUTPUT);
    pinMode(midIrReceiverPin, INPUT);
    pinMode(midLedPin, OUTPUT);
    //  pinMode(redLedPin, OUTPUT);               // Red LED pin is an output
    Serial.begin(9600);                       // Set data rate to 9600 bps
    servoLeft.attach(13);
    servoRight.attach(12);
    // irDetect(midIrReceiverPin, midLedPin, highFrequency);
    // irDetect(leftIrReceiverPin, leftLedPin, lowFrequency);
    // irDetect(rightIrReceiverPin, rightLedPin, lowFrequency);
  }  
  
  void loop()                                  // Main loop auto-repeats
  {
    if (turns == 0 && finish){
      timer++;
      if (timer >= 100){
        while (true){
          servoLeft.writeMicroseconds(leftStopped);
          servoRight.writeMicroseconds(rightStopped);
        }
      }
      
      // final straight
      // lastTurn = true;
      // Serial.println("LASTTURN");
    }
      // if (firstTurn) timer++;
      // if (lastTurn) timer--;
      if ((irDetect(leftIrReceiverPin, leftLedPin, highFrequency) == 0) && (irDetect(rightIrReceiverPin, rightLedPin, highFrequency) == 1)){
        servoLeft.writeMicroseconds(leftForward);
        servoRight.writeMicroseconds(rightForward + 145); // adjust right
        // delay(497);
      } 
      else if ((irDetect(rightIrReceiverPin, rightLedPin, highFrequency) == 0) && (irDetect(leftIrReceiverPin, leftLedPin, highFrequency) == 1)){
        servoLeft.writeMicroseconds(leftForward-offSet);
        servoRight.writeMicroseconds(rightForward); // adjust left
        // delay(550);
      } 
      else if ((irDetect(midIrReceiverPin, midLedPin, highFrequency) == 0) && (irDetect(leftIrReceiverPin, leftLedPin, lowFrequency) == 0) && (irDetect(rightIrReceiverPin, rightLedPin, lowFrequency) == 0)){

        while (true){
          servoLeft.writeMicroseconds(leftStopped);
          servoRight.writeMicroseconds(rightStopped);
        }
        
        servoLeft.writeMicroseconds(leftBackward);
        servoRight.writeMicroseconds(rightBackward);
        delay(350);
        servoRight.writeMicroseconds(rightForward);
        servoLeft.writeMicroseconds(leftStopped);
        delay(700);
        servoLeft.writeMicroseconds(leftBackward);
        servoRight.writeMicroseconds(rightForward);
        delay(625); // 180
        turn = true;
        finish = true;
        while (true){
          servoLeft.writeMicroseconds(leftStopped);
          servoRight.writeMicroseconds(rightStopped);
        }

        // delay(500);
      }
      else if ((irDetect(leftIrReceiverPin, leftLedPin, lowFrequency) == 0) && (irDetect(rightIrReceiverPin, rightLedPin, lowFrequency) == 1) && (irDetect(midIrReceiverPin, midLedPin, frontFreq) == 0)){
        firstTurn = false;
        if (finish && turns != 0) turns--;
        else turns++;
        if (turn){
          servoLeft.writeMicroseconds(leftForward);
          servoRight.writeMicroseconds(rightBackward); // right 720
          delay(4800);
          turn = !turn;
        } else{
          servoLeft.writeMicroseconds(leftForward);
          servoRight.writeMicroseconds(rightBackward-200); // long turn right
          delay(900);
        }
        
      }
      else if ((irDetect(rightIrReceiverPin, rightLedPin, lowFrequency) == 0) && (irDetect(leftIrReceiverPin, leftLedPin, lowFrequency) == 1) && (irDetect(midIrReceiverPin, midLedPin, frontFreq) == 0)){
        firstTurn = false;
        if (finish && turns != 0) turns--;
        else turns++;
        if (turn){
          servoLeft.writeMicroseconds(leftBackward);
          servoRight.writeMicroseconds(rightForward); // left 720
          delay(4620);
          turn = !turn;
        }
        else{
          servoLeft.writeMicroseconds(leftBackward + 100);
          servoRight.writeMicroseconds(rightForward); // long turn left
          delay(800);
        }
        
      } 
      else{
        servoLeft.writeMicroseconds(leftForward);
        servoRight.writeMicroseconds(rightForward);
        // delay(500);
      }
      
      // if (timer == 0 && lastTurn){
      //   servoLeft.writeMicroseconds(leftStopped);
      //   servoRight.writeMicroseconds(rightStopped);
      //   return 0;
      // }
      delay(10);
      // Serial.println(turns);
      // Serial.println(timer);
      // Serial.println(firstTurn);
      // Serial.println(lastTurn);
      // Serial.println();
      // unsigned long long int a;
      // for (int i = 0; i < 10; i++){
      //   a += irDistance(irLedPin, irReceiverPin);
      // }  
      // unsigned long int b = (a / 10);
      // Serial.println(b);
      // Serial.println(irDistance(irLedPin, irReceiverPin));

      // delay(500);

  }

  // IR Object Detection Function

  int irDetect(int irReceiverPin, int irLedPin, long highFrequency)
  {
    tone(irLedPin, highFrequency);                 // Turn on the IR LED square wave
    delay(1);                                  // Wait 1 ms
    noTone(irLedPin);                          // Turn off the IR LED
    int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
    delay(1);                                  // Down time before recheck
    return ir;                                 // Return 0 detect, 1 no detect
  }


  // unsigned long irDistance(int irLedPin, int irReceivePin)
  // {
  //   unsigned long maxhighFrequency = 0;
  //    int distance = 0;
  //    for(unsigned long f = highFrequency; f <= 42000; f += 10)
  //    {
  //       if (irDetect(f) == 0){
  //         maxhighFrequency = f;
  //       }
  //       // distance += irDetect(f);
  //       // highFrequency = f;
  //       // Serial.println(highFrequency);
  //    }
  //   //  return distance;
  //   return maxhighFrequency;
  // }