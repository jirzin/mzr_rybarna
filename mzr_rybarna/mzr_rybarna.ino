const int pinNumber = 4;

// analog inputs
const int aPins[] = {
  A0, A2, A1, A3
};

// initial analog inputs values
int aVals[] = {
  1024, 1024, 1024, 1024, 1024
};


// pin nubers for pwm LEd signal
const int pwmPins[] = {
  6, 3, 5, 9
};
// 6 - stul rybarna
// 3 - global
// 5 - lednice 1
// 9 - lednice 2

// pwm values to be writen as outputs
int pwmVals[] = {
  0, 0, 0, 0
};

// scaling of sensors
int a0Scale[] = {40, 1010, 10};
int a1Scale[] = {20, 920, 10};
int a2Scale[] = {800, 970, 10};
int a3Scale[] = {600, 970, 10};

// smoothing constants
const int smoothFactor = 200;


// lowpass filter coefficient
const float smoothSamples = 20.0;


int sensMax = 0;
int sensMin = 1024;

// smoothed buffers
float smoothed0 = 0.0;
float smoothed1 = 0.0;
float smoothed2 = 0.0;
float smoothed3 = 0.0;


int f = 0;

void setup() {
  Serial.println("hello this is rybarna arduino");
  delay(1000);
  Serial.println("setting pwm outputs");
  for (int i = 0; i < 5; i++) {
    Serial.print("pin ");
    Serial.print(i);
    Serial.print(" setup:");
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], pwmVals[i]);
    Serial.println(" ok");
    delay(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //f++;

  readAnalogPins();

  for (int i = 0; i < pinNumber; i++) {

    switch (i) {
      case 0:

        //pwmVals[i] = constrain(smooth(255 - valCorrection(aVals[i], a0Scale[0], a0Scale[1], a0Scale[2]), smoothBuff0),10,255);
        //pwmVals[i] = smooth(255 - valCorrection(aVals[i], a0Scale[0], a0Scale[1], a0Scale[2]), smoothBuff0);
        pwmVals[i] = smooth(valCorrection(aVals[1], a0Scale[0], a0Scale[1], a0Scale[2]), pwmVals[i], smoothSamples);
        
        break;
      case 1:
        //pwmVals[i] = constrain(smooth(255 - valCorrection(aVals[i], a1Scale[0], a1Scale[1], a1Scale[2]), smoothBuff1),10,255);
        //pwmVals[i] = smooth(255 - valCorrection(aVals[i], a1Scale[0], a1Scale[1], a1Scale[2]), smoothBuff1);
        //pwmVals[i] = smooth(valCorrection(min(min(aVals[1], aVals[2]), aVals[3]), a1Scale[0], a1Scale[1], a1Scale[2]), pwmVals[i], smoothSamples);
        break;
      case 2:
        //pwmVals[i] = constrain(smooth(255 - valCorrection(aVals[i], a2Scale[0], a2Scale[1], a2Scale[2]), smoothBuff2),10,255);
        pwmVals[i] = smooth(valCorrection(aVals[i], a2Scale[0], a2Scale[1], a2Scale[2]), pwmVals[i], smoothSamples);
        break;
      case 3:
        //pwmVals[i] = constrain(smooth(255 - valCorrection(aVals[i], a3Scale[0], a3Scale[1], a3Scale[2]), smoothBuff3),10,255);
        pwmVals[i] = smooth(valCorrection(aVals[i], a3Scale[0], a3Scale[1], a3Scale[2]), pwmVals[i], smoothSamples);
        break;
    }

    //    int as = smooth(aVals[i],
    //Serial.print(aVals[i]);
    //Serial.print(" | ");
    //Serial.print(" | ");
    //Serial.print(valCorrection(aVals[i],a0Scale[0],a0Scale[1],a0Scale[2]));

    //Serial.print(" | ");
  }

  writePwms();
//  if(millis()%1000<500){
//    analogWrite(pwmPins[2],255);
//  }else{
//    analogWrite(pwmPins[2],0);
//  }
  

  

  //sensMax = max(sensMax, aVals[1]);
  //  sensMin = smooth(aVals[3],sensMin,100.0);
  //
  //  Serial.println("");
  //  Serial.print(sensMax);
  //  Serial.print(" | ");
  //  Serial.print(sensMin);

  Serial.println("");
  for (int i = 0; i < pinNumber; i++) {
    Serial.print(pwmVals[i]);
    Serial.print(" | ");
  }
  Serial.print(" +++++ ");
  for (int i = 0; i < pinNumber; i++) {
    Serial.print(aVals[i]);
    Serial.print(" | ");
  }

//  Serial.println("");
//  for (int i = 0; i < smoothFactor; i++) {
//    Serial.print(smoothBuff0[i]);
//    Serial.print(" ");
//  }
  delay(10);
}

void readAnalogPins() {
  for (int i = 0; i < pinNumber; i++) {
    aVals[i] = analogRead(aPins[i]);
  }
}
//
//void inputProcessing(){
//
//}

void writePwms() {
  for (int i = 0; i < pinNumber; i++) {
    switch (i) {
      case (0):
        analogWrite(pwmPins[2], constrain(255 - map(pwmVals[0],0,230,0,255),10,255));
        break;
      case (1):
        analogWrite(pwmPins[1], constrain(min(min(pwmVals[0],pwmVals[2]),pwmVals[3]),20,255 ));//pwmVals[1]);
        break;
      case (2):
        //analogWrite(pwmPins[2], 255 - pwmVals[2]);
        analogWrite(pwmPins[0],constrain(255-map(pwmVals[2],0,236,0,255),10,255));
        break;
      case (3):
        analogWrite(pwmPins[3], constrain(255 - map(pwmVals[3],0,236,0,255),10,255)); // lednice
        break;
    }
  }
}

int valCorrection(int in, int minim, int maxim, int curve) {
  //  int out = (int)(pow((map(constrain(in,minim,maxim),minim,maxim,0,255)/255),curve)*255);
  int out = constrain(in, minim, maxim);
  out = map(out, minim, maxim, 0, 255);
  out = (int)(pow(out / 255.0, curve) * 255);
  return out;
}

float floatConstrain(float num, float mini, float maxi) {
  if (num <= mini) {
    return mini;
  };
  if (num >= maxi) {
    return maxi;
  };
  if (num > mini && num < maxi) {
    return num;
  };
}

float smooth(int input, int buff, float samples) {
  return (buff + ((input - buff) / samples));
}

