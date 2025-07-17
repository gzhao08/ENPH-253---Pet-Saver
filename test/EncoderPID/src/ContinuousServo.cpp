#include "ContinuousServo.h"

// ContinuousServo::ContinuousServo()
ContinuousServo::ContinuousServo(int motorPin1, int motorPin2, 
int pwmChannel1, int pwmChannel2) {
    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2;
    this->pwmChannel1 = pwmChannel1;
    this->pwmChannel2 = pwmChannel2;
}

void ContinuousServo::begin(MagneticEncoder* enc) {
    ledcSetup(pwmChannel1, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
    ledcAttachPin(motorPin1, pwmChannel1);

    ledcSetup(pwmChannel2, 100, 12); // (pwmchannel to use,  frequency in Hz, number of bits)
    ledcAttachPin(motorPin2, pwmChannel2);

    this->encoder = enc;

    this->pidController = new PID(
        &this->Input, &this->Output, &this->Setpoint, 
        this->Pk, this->Ik, this->Dk, DIRECT);
    this->pidController->SetMode(AUTOMATIC);              // PID Setup
    this->pidController->SetOutputLimits(-BIT_12_LIMIT, BIT_12_LIMIT);
    this->pidController->SetSampleTime(20);
}

void ContinuousServo::drivePWM(int signedDuty) {
    int duty = (int) abs(signedDuty);
    duty = constrain(duty, 0, BIT_12_LIMIT);

    if (signedDuty <= 0) {
        ledcWrite(pwmChannel2, 0);
        ledcWrite(pwmChannel1, duty);
    } else {
        ledcWrite(pwmChannel1, 0);
        ledcWrite(pwmChannel2, duty);
    }
}

void ContinuousServo::moveBy(int degrees) {

}

void ContinuousServo::moveTo(int degrees) {
    int rotations = degrees / 360;
    int angleIncrement = degrees % 360;

    int startAngle = this->encoder->readAngle();
  
    int currentRotation = 0;
    float currentAngle = startAngle;
    float prevAngle = startAngle;
    float angleDifferenceThreshold = 1;

  // // Blocking
  // Count rotations
    while (currentRotation != rotations) {
        if (currentRotation < rotations) {
        this->drivePWM(-4096); // Increase encoder
    } else if (currentRotation > rotations) {
        this->drivePWM(4096);; // Decrease encoder
    } else {
        this->drivePWM(0); // Stop
    }

    currentAngle = this->encoder->readAngle();

    Serial.print("Reference angle: ");
    Serial.println(startAngle);

    Serial.print("Prev angle: ");
    Serial.println(prevAngle);

    Serial.print("Current angle: ");
    Serial.println(currentAngle);


    float prevAngleDifference = this->encoder->angleDifference(startAngle, prevAngle);
    float currentAngleDifference = this->encoder->angleDifference(currentAngle, startAngle);

    if (abs(prevAngleDifference) < 90 && abs(currentAngleDifference) < 90) { // Might bug
      if (prevAngleDifference > 0 && currentAngleDifference > 0) { // Might bug, think about it
        currentRotation++;
      } else if (prevAngleDifference < 0 && currentAngleDifference < 0) {
        currentRotation--;
      }
    }

    prevAngle = currentAngle;

    Serial.print("Current rotation: ");
    Serial.println(currentRotation);
    Serial.println("-----");
  }

  for (int i = 0; i < 100; i++) {
    int targetAngle = (startAngle + angleIncrement) % 360;
    this->PIDSequence(targetAngle);
    delay(10);
  }
}

void ContinuousServo::PIDSequence(int targetValue) {
    double angle = this->encoder->readAngle();
    if (angle != -1) {
        // PID Feedback
        float angleError = this->encoder->angleDifference(targetValue, angle);
        Input = angleError;
        this->pidController->Compute();
        this->drivePWM(Output);
    
        Serial.print(angle, 2);
        Serial.println(" deg");

        Serial.print("Angle error: ");
        Serial.println(angleError);
        Serial.print("PID Output: ");
        Serial.println(Output);
    } else {
        Serial.println("No AS5600 detected.");
    }


    Serial.println("-----");
}

void ContinuousServo::testSequence() {
    moveTo(50);
    delay(500);
    moveTo(180);
    delay(500);
    moveTo(490);
}

/**
 * Run this inside void loop to enable servo feedback
 */
void ContinuousServo::loop() {

}




// PID Tuning
// int newKp = map(analogRead(P_Pin), 0, BIT_12_LIMIT, 0, 1000);
// double newKd = ((double) map(analogRead(D_Pin), 0, BIT_12_LIMIT, 0, 1000)) / 30;
// Serial.print("Kp: ");
// Serial.println(newKp);
// Serial.print("Kd: ");
// Serial.println(newKd);
// myPID.SetTunings(newKp, 0, newKd);