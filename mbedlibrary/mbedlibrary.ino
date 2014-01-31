// MBed compatability layer for Teensy 3.1 by Matt Parsons.
// Proof of concept MBed style API library on top of Teensyduino,
// hopefully this will will advanced and optimised by the Teensy comunity.
//
// Version 0.1
// 10/01/2014

//********************************DigitalIO********************************

#define PullDown  0; // pin mode: pulldown
#define PullUp    1; // pin mode: pullup

enum IOPins {p5=0,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30};

//**************** DigitalOut
class DigitalOut {
 public:
        DigitalOut(int);
        void write(int);
        int read(void);
        int operator=(int value);
 private:
        int pinNumber;
        int value; 
};

DigitalOut::DigitalOut(int pinNumber){
      this->pinNumber=pinNumber;

      pinMode(pinNumber,OUTPUT);
      digitalWrite(pinNumber,LOW);  //Default to low
      value=0;
}

void DigitalOut::write(int value){
        this->value=value;
        
        if(value !=0){
          digitalWrite(pinNumber,HIGH);
        } else {
          digitalWrite(pinNumber,LOW);
        }
}

int DigitalOut::read(void){
    return value;
}

int DigitalOut::operator=(int value){
        this->write(value);
        return value;
}

//**************** DigitalIn
class DigitalIn{
 public:
        DigitalIn(int);
        int read(void);
        void mode(int);
        int operator=(int);
 private:
        int pinNumber;
        int modeValue; 
};

DigitalIn::DigitalIn(int pinNumber){
    this->pinNumber = pinNumber;
    
    pinMode(pinNumber,INPUT);
}

int DigitalIn::read(void){
    return digitalRead(pinNumber);  
}

void DigitalIn::mode(int modeValue){
        this->modeValue = modeValue;
        
        if(modeValue == 1){
           pinMode(pinNumber,INPUT_PULLUP);
        } else {
           pinMode(pinNumber,INPUT);
        } 
}

int DigitalIn::operator=(int value){   //ignore and value passed to this class
        return this->read();
}


//**************** DigitalInOut - Bidirectional IO pin.
class DigitalInOut {
 public:
        DigitalInOut(int);
        void write(int);
        int read(void);
        void output();
        void input();
        void mode(int);
        int operator=(int);
 private:
        int pinNumber;
        int value; 
        int modeValue;
};

DigitalInOut::DigitalInOut(int pinNumber){
        this->pinNumber = pinNumber;
        
        pinMode(pinNumber,OUTPUT);    //Default to output
        digitalWrite(pinNumber,LOW);  //Default to low
        value=0;
}

void DigitalInOut::write(int value){
        this->value=value;
        
        if(value !=0){
          digitalWrite(pinNumber,HIGH);
        } else {
          digitalWrite(pinNumber,LOW);
        }
}

int DigitalInOut::read(void){
    return digitalRead(pinNumber);  
}


void DigitalInOut::output(void){
        pinMode(pinNumber,OUTPUT); 
}

void DigitalInOut::input(void){
        pinMode(pinNumber,INPUT); 
}


void DigitalInOut::mode(int modeValue){
        this->modeValue = modeValue;
        
        if(modeValue == 1){
           pinMode(pinNumber,INPUT_PULLUP);
        } else {
           pinMode(pinNumber,INPUT);
        } 
}


int DigitalInOut::operator=(int value){
        this->write(value);
        return this->read();
}

//**************** PwmOut
class PwmOut {
  public:
      PwmOut(int);
      void write(float);  //Percent of period
      float read(void);   //Percent of period
      void period(float);  //float value in seconds
      void period_ms(int);
      void period_us(int);
      void pulsewidth(float);
      void pulsewidth_ms(int);
      void pulsewidth_us(int);
      float operator=(float);
      void resolution(int);  //Teensy specific function
  private:
      int pinNumber;
      int res;
      int frequency;
      int duty;
      int dutyMax;
      float percentage;
};

PwmOut::PwmOut(int pinNumber){
        this->pinNumber = pinNumber;
        duty=0;          // default to 0
        res=10;   // analogWrite 10bit value (0 to 1023)
        dutyMax=pow(2,res);
        frequency=50;    //default to 50Hz
        percentage=0;
        
        pinMode(pinNumber,OUTPUT);
        analogWriteResolution(res); 
        analogWriteFrequency(pinNumber, frequency);
        analogWrite(pinNumber,duty);
}

void PwmOut::write(float percentage){
        this->percentage = percentage;
        duty = (int)(percentage * dutyMax);
        analogWrite(pinNumber,duty); 
}

float PwmOut::read(void){
     return percentage;
}

void PwmOut::period(float value){
        frequency = (int)(1/value);
        analogWriteFrequency(pinNumber, frequency);
}

void PwmOut::period_ms(int value){
        float temp = (float) value/1000;
         frequency = (int)1/temp;
         analogWriteFrequency(pinNumber,frequency);
}

void PwmOut::period_us(int value){
        float temp = (float) value/1000000;
         frequency = (int)1/temp;
         analogWriteFrequency(pinNumber,frequency);
}

void PwmOut::pulsewidth(float value){
        value = dutyMax * (value / (1/frequency) );
        duty=(int)value;
        analogWrite(pinNumber,duty);
}

void PwmOut::pulsewidth_ms(int value){
        float temp = dutyMax * (value / (1/frequency) * 1000 );
        duty=(int)temp;
        analogWrite(pinNumber,duty);
}

void PwmOut::pulsewidth_us(int value){
        float temp = dutyMax * (value / (1/frequency) * 1000000 );
        duty=(int)temp;
        analogWrite(pinNumber,duty);
}

float PwmOut::operator=(float percentage){
        this->write(percentage);
        return percentage;
}

void PwmOut::resolution(int res){
        this->res = res;
        analogWriteResolution(res);
        dutyMax = pow(2,res);
}


//********************************Time and Interrupts********************************

//****************Timer
class Timer {
  public:
        Timer(void);
        void start(void);
        void stop(void);
        void reset(void);
        float read(void);
        int read_ms(void);
        int read_us(void);
  private:
        elapsedMicros time; 
        int stopTime;
        byte stopped;
};

Timer::Timer(void){
        time=0;
        stopTime=0;
        stopped=1;       
}

void Timer::start(void){
        time=0; 
        stopped=0;
}

void Timer::stop(void){
        stopTime=time;
        stopped=1;
}

void Timer::reset(void){
        time=0;
        stopTime=0; 
}

float Timer::read(void){
  
       if(stopped==1){
         return (float) (stopTime / 1000000);
       }
       
       return (float) ( (stopTime+time) / 1000000); 
}

int Timer::read_ms(void){
  
       if(stopped==1){
         return (stopTime / 1000);
       }
       
       return ( (stopTime+time) / 1000); 
}

int Timer::read_us(void){
  
       if(stopped==1){
         return stopTime;
       }
       
       return stopTime+time; 
}

/****************TimeOut - Doesn't work yet

class Timeout{
  public:
        void attach(void(*)(void),float);
  private:
        static void(*function)(void);
        int time_us;
        IntervalTimer timer;
        static void wait(void);
};

void Timeout::attach(void(*fctn)(void),float time){
    time=time*1000000;
    time_us = (int) time;
    function = fctn;
    timer.begin(&wait,time_us);
}

void Timeout::wait(void){
     function();
     timer.end(); 
}
*/

//********************************
void setup() {
}

void loop(){
  causeAnError=true;
}
