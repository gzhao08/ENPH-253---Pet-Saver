#include "SteeringManager.h"
#include "../GlobalConstants.h"
#include "esp_task_wdt.h"

//volatile boolean drive = false; // Global variable to control driving state

SteeringManager::SteeringManager()
    : leftMotor(LEFT_MOTOR_PIN_A,LEFT_MOTOR_PIN_B,LEFT_MOTOR_PWM_CHANNEL_A,LEFT_MOTOR_PWM_CHANNEL_B,12), 
      rightMotor(RIGHT_MOTOR_PIN_A,RIGHT_MOTOR_PIN_B,RIGHT_MOTOR_PWM_CHANNEL_A,RIGHT_MOTOR_PWM_CHANNEL_B,12),
      pidController(&input, &output, &setpoint, KP_DEFAULT, Ki, Kd, DIRECT)
{

}

/**
 * Initialize the DCMotor
 * Sets up PID, motors, and IR sensors
 * @param outerLeftPin Pin for the outer left IR sensor
 * @param innerLeftPin Pin for the inner left IR sensor 
 * @param innerRightPin Pin for the inner right IR sensor
 * @param outerRightPin Pin for the outer right IR sensor
 */
void SteeringManager::begin() {
    // Motor
    leftMotor.begin();
    rightMotor.begin();
    // PID
    this->pidController.SetMode(AUTOMATIC);
    this->pidController.SetOutputLimits(-leftMotor.getMaxDutyCycle(), leftMotor.getMaxDutyCycle()); // Set output limits to motor max duty cycle
    this->pidController.SetSampleTime(this->PIDSampleTime);
    this->pidController.SetTunings(KP_DEFAULT, 0, 0);

    

    // IR
    this->array.begin(OUTER_LEFT_PIN,INNER_LEFT_PIN,INNER_RIGHT_PIN,OUTER_RIGHT_PIN);
}

/**
 * Drive both motors forward with the same duty cycle
 * @param duty the positive duty cycle to drive the motors forwards with
 */
void SteeringManager::forward(int duty) {
    while (robotState == RobotState::FORWARD) {
        leftMotor.drivePWM(duty);
        rightMotor.drivePWM(duty);
        delay(5);
    }
}

/**
 * Drive both motors backward with the same duty cycle
 * @param duty the positive duty cycle to drive the motors backwards with
 */
void SteeringManager::backward(int duty) {
    // portENTER_CRITICAL(&mux);
    //drive = true;
    // portEXIT_CRITICAL(&mux);
    while (robotState == RobotState::BACKWARD) {
        leftMotor.drivePWM(-duty);
        rightMotor.drivePWM(-duty);
        delay(5);
    }
 //   this->stop();
}

/**
 * Drive both motors backward with the same duty cycle
 * @param duty the positive duty cycle to drive the motors backwards with
 */
void SteeringManager::backward(int duty, int timeInMS) {
    unsigned long startTime = millis();
    unsigned long currentTime = startTime;
    while (currentTime - startTime <= timeInMS) {
        leftMotor.drivePWM(-duty);
        rightMotor.drivePWM(-duty);
        currentTime = millis();
    }
    this->stop();
}

/**
 * Stop both motors
 * Sets the PWM channels to 0
 */
void SteeringManager::stop() {
    leftMotor.stop();
    rightMotor.stop();
    robotState = RobotState::IDLE;
}

/**
 * Stop both motors
 * Sets the PWM channels to 0
 */
void SteeringManager::quickStop() {
    this->backward(2200,100);
    this->array.takeReading(false);
    this->array.getError();
    this->array.update();
    leftMotor.stop();
    rightMotor.stop();
    this->array.takeReading(false);
    this->array.getError();
    this->array.update();
    robotState = RobotState::STOPPED;
}

/**
 * Stop both motors
 * Sets the PWM channels to 0
 * For ramp end backward
 */
void SteeringManager::quickStopTwo() {
    this->backward(4096,500);
    this->array.takeReading(false);
    this->array.getError();
    this->array.update();
    leftMotor.stop();
    rightMotor.stop();
    this->array.takeReading(false);
    this->array.getError();
    this->array.update();
    robotState = RobotState::QUICK_STOP;
}

/**
 * Reverse the motors in place until the IR sensors detect that they are not on the line
 * This is used to turn in place
 * @param duty the positive duty cycle to drive the motors with
 */
void SteeringManager::turnAround(int duty, boolean clockwise) {

    duty = clockwise ? duty : -duty;
    
    // counter clockwise
    while (this->array.isOnLine()) {
        // turn in place until off line
        this->array.takeReading(true);
        this->array.getError();
        this->array.update();
        leftMotor.drivePWM(duty);
        rightMotor.drivePWM(-duty);
    }

    Serial.println("Off line now");

    while (!this->array.isOnLine()) {
        // turn in place until back on line
        leftMotor.drivePWM(duty);
        rightMotor.drivePWM(-duty);
        this->array.takeReading(true);
        this->array.getError();
        this->array.update();
    }
    Serial.println("Finish reverse");
    startLineFollow();
}

void SteeringManager::turnBackwards(int duty) {
    Serial.println("Turning backwards");
    // counter clockwise
    while (this->array.isOnLine()) {
        // turn in place until off line
        this->array.takeReading(false);
        this->array.getError();
        this->array.update();
        leftMotor.drivePWM(500);
        rightMotor.drivePWM(-2100);
        delay(1);
    }

    Serial.println("Off line now");

    while (!this->array.isCentered()) {
        // turn in place until back on line
        leftMotor.drivePWM(500);
        rightMotor.drivePWM(-2100);
        this->array.takeReading(false);
        this->array.getError();
        this->array.update();
        delay(1);
    }
    Serial.println("Finish turnBackwards()");
    startLineFollow();
}

void SteeringManager::turnBackwards_CCW() {
    Serial.println("Turning backwards");
    // counter clockwise
    while (this->array.isOnLine()) {
        // turn in place until off line
        this->array.takeReading(false);
        this->array.getError();
        this->array.update();
        leftMotor.drivePWM(-2100);
        rightMotor.drivePWM(500);
        delay(1);
    }

    Serial.println("Off line now");

    while (!this->array.isCentered()) {
        // turn in place until back on line
        leftMotor.drivePWM(-2100);
        rightMotor.drivePWM(500);
        this->array.takeReading(false);
        this->array.getError();
        this->array.update();
        delay(1);
    }
    Serial.println("Finish turnBackwards()");
    startLineFollow();
}

/**
 * Line follow using the IR sensors and PID controller
 * @param baseSpeed the base speed to drive the motors at
 * The PID controller will adjust the speed of the motors based on the error from the IR sensors
 */
void SteeringManager::lineFollow(int baseSpeed) {
    
    while (!drive) {
        Serial.println("lineFollow(): waiting for drive to be true");
    }

    recordStartTime();
    Serial.println("lineFollow(): Reading Started");
    this->array.takeReading(false);
    input = this->array.getError();
    unsigned long lastComputeTime = millis();
    this->array.update();
    // Serial.printf("Kp: %lf", this->pidController.GetKp());
    while (drive) {
        // only poll and calculate PID at PID sample rate
        if (millis() - lastComputeTime >= this->PIDSampleTime) {
            // compute PID
            pidController.Compute();
            lastComputeTime = millis();
            // drive motors
            leftMotor.drivePWM(baseSpeed  -output);
            rightMotor.drivePWM(baseSpeed+output);
            
        }
        // update IR data every cycle so that error is accurate
        this->array.takeReading(false);
        input = this->array.getError();
        // array.showState();
        // Serial.printf(" -- %lf\n", output);
        this->array.update();
        delay(1);
        Serial.printf("lineFollow -- drive : %d\n", drive);
    }
    Serial.println("lineFollow(): Reading Stopped");
}


void SteeringManager::lineFollow() {
    
    recordStartTime();
    Serial.println("lineFollow(): Reading Started");
    this->array.takeReading(false);
    input = this->array.getError();
    unsigned long lastComputeTime = millis();
    this->array.update();
    // Serial.printf("Kp: %lf", this->pidController.GetKp());
    while (robotState == RobotState::LINE_FOLLOW) {
        // only poll and calculate PID at PID sample rate
        if (millis() - lastComputeTime >= this->PIDSampleTime) {
            // compute PID
            pidController.Compute();
            lastComputeTime = millis();
            // drive motors
            leftMotor.drivePWM(currentSpeed - output);
            rightMotor.drivePWM(currentSpeed + output);
            
        }
        // update IR data every cycle so that error is accurate
        this->array.takeReading(false);
        input = this->array.getError();
        // array.showState();
        // Serial.printf(" -- %lf\n", output);
        this->array.update();
        delay(1);
    }
    Serial.println("lineFollow(): Reading Stopped");
}

/**
 * Set the PID controller tunings
 * @param kp Proportional gain
 * @param kd Derivative gain
 */
void SteeringManager::setPID(double kp, double kd) {
    pidController.SetTunings(kp,kd,0);
}