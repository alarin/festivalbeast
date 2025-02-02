#include <Arduino.h>

#include <FastAccelStepper.h>
#include <SimpleKalmanFilter.h>
#include <DifferentialSteering.h>

int PIN_THROTTLE = 34;
int PIN_STEERING = 35;

int PIN_STEPPER_R_STEP = 17; //define Pulse pin
int PIN_STEPPER_R_DIR = 5; //define Direction pin
int PIN_STEPPER_R_ENA = 16;

int PIN_STEPPER_L_STEP = 25; //define Pulse pin
int PIN_STEPPER_L_DIR = 26; //define Direction pin
int PIN_STEPPER_L_ENA = 27;

int STEPPER_MAX_SPEED = 3200;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper_r = NULL;
FastAccelStepper *stepper_l = NULL;

SimpleKalmanFilter throttleKF(1, 1, 0.01);
SimpleKalmanFilter steeringKF(1, 1, 0.01);
DifferentialSteering diffSteer;


void setup() {
  pinMode (PIN_THROTTLE, INPUT);
  // Serial.begin(115200);
  
  engine.init();
  
  stepper_r = engine.stepperConnectToPin(PIN_STEPPER_R_STEP);
  
  if (stepper_r) {
    stepper_r->setEnablePin(PIN_STEPPER_R_ENA);
    stepper_r->setDirectionPin(PIN_STEPPER_R_DIR);
    stepper_r->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper_r->setSpeedInHz(STEPPER_MAX_SPEED);  // the parameter is us/step !!!
    stepper_r->setAcceleration(STEPPER_MAX_SPEED);
  }

  stepper_l = engine.stepperConnectToPin(PIN_STEPPER_L_STEP);
  
  if (stepper_l) {
    stepper_l->setEnablePin(PIN_STEPPER_L_ENA);
    stepper_l->setDirectionPin(PIN_STEPPER_L_DIR);
    stepper_l->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper_l->setSpeedInHz(STEPPER_MAX_SPEED);  // the parameter is us/step !!!
    stepper_l->setAcceleration(STEPPER_MAX_SPEED);
  }

  diffSteer.begin(32);
  // stepper_l->setSpeedInHz(STEPPER_MAX_SPEED);
  // stepper_r->setSpeedInHz(STEPPER_MAX_SPEED);
  // stepper_l->runForward();
  // stepper_r->runBackward();
}

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = 0;
  while(abs(ch) < 100) {
    ch = pulseIn(channelInput, HIGH, 3000);
  }
  // if (abs(ch) < 100) return defaultValue; //filtering
  ch -= 1500;
  if (abs(ch) < 10) return defaultValue; //death zone
  int throttle = map(ch, -500, 500, minLimit, maxLimit); 
  // Serial.print(ch);
  // Serial.print(" ");
  // Serial.println(throttle);
  return throttle;
}

void moveBySteps(int throttle) {
    if (stepper_l->queueEntries() < 5) {
      stepper_l->move(throttle * 50);
      stepper_r->move(-throttle * 50);
    }
}

void moveContinious(int throttle, int steering) {
    diffSteer.computeMotors(throttle, steering);
    int leftMotor = diffSteer.computedLeftMotor();
    int rightMotor = diffSteer.computedLeftMotor();

    uint32_t speedLeftHz = (u_int32_t) abs(STEPPER_MAX_SPEED * leftMotor/127.0);
    uint32_t speedRightHz = (u_int32_t) abs(STEPPER_MAX_SPEED * rightMotor/127.0);

    stepper_l->setSpeedInHz(speedLeftHz);
    stepper_r->setSpeedInHz(speedRightHz);

    if (throttle == 0) {
      stepper_l->stopMove();
      stepper_r->stopMove();    
    } else {
      if (leftMotor > 0) {
        stepper_l->runForward();        
      } else {
        stepper_l->runBackward();
      }
      if (rightMotor > 0) {
        stepper_r->runForward();        
      } else {
        stepper_r->runBackward();
      }
      // if (throttle > 0) {
      //   stepper_l->runForward();
      //   stepper_r->runBackward();
      // } else {
      //   stepper_l->runBackward();
      //   stepper_r->runForward();
      // }
    }
}

long start = millis();

void loop() {
    int throttle = readChannel(PIN_THROTTLE, -127, 127, 0);
    int filteredThrottle = throttleKF.updateEstimate(throttle);

    int steering = readChannel(PIN_STEERING, -127, 127, 0);
    int filteredSteering = throttleKF.updateEstimate(throttle);
    // Serial.print(">throttle:");
    // Serial.print(throttle);
    // Serial.print(",fThrottle:");
    // Serial.println(filteredThrottle);
    // if ((millis() - start) > 1000*5) {
    //   stepper_l->stopMove();
    //   stepper_r->stopMove();
    // }
    moveContinious(filteredThrottle, filteredSteering);
}