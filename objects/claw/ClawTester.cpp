#include "ClawTester.h"
ClawGrabber grabber(GRABBER_MOTOR_PIN, GRABBER_PWM_CHANNEL);

int muxPin = 8;
WireManager wireManager(muxPin); 
WireManager wireManager2(-1); 

DelayManager positionDelayManager(4000);

int armMotorPin1 = ARM_MOTOR_PIN_1;
int armMotorPin2 = ARM_MOTOR_PIN_2;
int armPwmChannel1 = ARM_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int armPwmChannel2 = ARM_MOTOR_PWM_CHANNEL_2; 
int armMuxLine = 1; 
bool armEncoderOnTerminalSide = true; //false for arm, 
int armSwitchPin = MICROSWITCH_ARM; 
bool armNormallyOpen = true; 
//muxLine: 1 is 1, 0 is 2, -1 is not muxing
ClawArm arm(armMotorPin1, armMotorPin2, armPwmChannel1, armPwmChannel2, armMuxLine, armEncoderOnTerminalSide, 
armSwitchPin, armNormallyOpen); 

int verticalStageMotorPin1 = VERTICAL_STAGE_MOTOR_PIN_1;
int verticalStageMotorPin2 = VERTICAL_STAGE_MOTOR_PIN_2;
int verticalStagePwmChannel1 = VERTICAL_STAGE_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int verticalStagePwmChannel2 = VERTICAL_STAGE_MOTOR_PWM_CHANNEL_2; 
int verticalStageMuxLine = 0; 
bool verticalStageEncoderOnTerminalSide = true; //false for arm, 
int verticalStageSwitchPin = MICROSWITCH_VERTICAL_STAGE; 
bool verticalStageNormallyOpen = true; 

ClawVerticalStage verticalStage(verticalStageMotorPin1, verticalStageMotorPin2, verticalStagePwmChannel1, verticalStagePwmChannel2, verticalStageMuxLine, verticalStageEncoderOnTerminalSide,
verticalStageSwitchPin, verticalStageNormallyOpen);

int baseMotorPin1 = BASE_MOTOR_PIN_1;
int baseMotorPin2 = BASE_MOTOR_PIN_2;
int basePwmChannel1 = BASE_MOTOR_PWM_CHANNEL_1; //motor pin 1 goes to B
int basePwmChannel2 = BASE_MOTOR_PWM_CHANNEL_2; 
int baseMuxLine = -1; 
bool baseEncoderOnTerminalSide = false; //false for arm, 
int baseSwitchPin = MICROSWITCH_BASE; 
bool baseNormallyOpen = true; 

ClawBase base(baseMotorPin1, baseMotorPin2, basePwmChannel1, basePwmChannel2, baseMuxLine, baseEncoderOnTerminalSide, baseSwitchPin, baseNormallyOpen);

float HARD_IRON_OFFSET_X = 0;
float HARD_IRON_OFFSET_Y = 0;
float HARD_IRON_OFFSET_Z = 0;


bool armHomed = false;
bool verticalHomed = false;
bool baseHomed = false;

Adafruit_LIS3MDL lis3mdl;


void calibrateMagnet();
void sensePet();
float getMagnetReadingMagSq();

void ClawTester::use() {
    // Setup
    Serial.setTimeout(3000);

    // 2. Begin wire manager
    wireManager.begin(&Wire);
    wireManager2.begin(&Wire1);
    // 3. Begin servo
    arm.begin(&wireManager); 
    verticalStage.begin(&wireManager);
    base.begin(&wireManager2);
    // servo.begin();
    grabber.begin();

    // Magnetometer setup
    if (!lis3mdl.begin_I2C(LIS3MDL_I2CADDR_DEFAULT, &Wire)) {
        Serial.println("Failed to find LIS3MDL chip");
        while (1) delay(10);
    }

    Serial.println("LIS3MDL Found!");
    lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
    lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
    lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
    lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);


    while (true) {
        bool quit = false;
        if (Serial.available()) {
            String line = Serial.readStringUntil('\n');
            int intCode = line.toInt();

            switch (intCode) {
                case 1: // Check arm
                    while (true) {
                    Serial.println("Arm position: " + String(arm.getPosition()));
                    arm.loop();

                    if (Serial.available()) {
                        line = Serial.readStringUntil('\n');
                        Serial.println("Quit");
                        break;
                    }
                    }
                    break;
                
                case 2:
                    while (true) {
                    Serial.println("Vertical stage position: " + String(verticalStage.getPosition()));
                    verticalStage.loop();

                    if (Serial.available()) {
                        line = Serial.readStringUntil('\n');
                        Serial.println("Quit");

                        break;
                    }
                    }
                    break;


                case 3:
                    while (true) {
                    Serial.println("Base position: " + String(base.getPosition()));
                    base.loop();

                    if (Serial.available()) {
                        line = Serial.readStringUntil('\n');
                        Serial.println("Quit");
                        break;
                    }
                    }
                    break;

                
                case 4:
                    Serial.println("Command: Arm homing");
                    arm.homingSequence();
                    armHomed = true;
                    break;
                case 5:
                    Serial.println("Command: Vertical homing");
                    verticalStage.homingSequence();
                    verticalHomed = true;
                    break;
                case 6:
                    Serial.println("Command: Base homing");
                    base.homingSequence();
                    baseHomed = true;
                    break;

                case 7:
                    Serial.println("Command: Arm test sequence");
                    arm.testSequence();
                    break;
                case 8:
                    Serial.println("Command: Vertical test sequence");
                    verticalStage.testSequence();
                    break;
                case 9:
                    Serial.println("Command: Base test sequence");
                    base.testSequence();
                    break;
                
                case 10:
                {
                    Serial.println("Command: Arm set position");
                    while (true) {
                    arm.loop();
                    if (Serial.available()) {
                        String posSetArm = Serial.readStringUntil('\n');
                        if (posSetArm == "q" || posSetArm.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }            
                        if (posSetArm.toInt() != -1) {
                        arm.setPosition(posSetArm.toInt());
                        Serial.println("Set position  of arm to: " + String(posSetArm.toInt()));
                        } else {
                        Serial.println("Quit");
                        break;
                        }
                    }
                    }
                    break;
                }
                case 11:
                {
                    Serial.println("Command: Vertical set position");
                    while (true) {
                    verticalStage.loop();
                    if (Serial.available()) {
                        String posSetVert = Serial.readStringUntil('\n');
                        if (posSetVert == "q" || posSetVert.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }
                        if (posSetVert.toInt() != -1) {
                        verticalStage.setPosition(posSetVert.toInt());
                        Serial.println("Set position of vertical stage to: " + String(posSetVert.toInt()));
                        } else {
                        Serial.println("Quit");
                        break;
                        }
                    }
                    }
                    break;
                }
                case 12:
                {
                    Serial.println("Command: Base set position");
                    while (true) {
                    base.loop();
                    if (Serial.available()) {
                        String posSetBase = Serial.readStringUntil('\n');
                        if (posSetBase == "q" || posSetBase.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }
                        if (posSetBase.toInt() != -1) {
                        base.setPosition(posSetBase.toInt());
                        Serial.println("Set position of base to: " + String(posSetBase.toInt()));
                        } else {
                        Serial.println("Quit");
                        break;
                        }
                    }
                    }
                    break;
                }
                case 13:
                    Serial.println("Command: Grabber close");
                    grabber.close();
                    break;
                case 14:
                    Serial.println("Command: Grabber open");
                    grabber.open();
                    break;
                case 15:
                    Serial.println("Command: Grabber parallel");
                    grabber.parallel();
                    break;
                case 16:
                {
                    Serial.println("Command: Grabber set position");
                    while (true) {
                    if (Serial.available()) {
                        String posGrabber = Serial.readStringUntil('\n');
                        if (posGrabber == "q" || posGrabber.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }
                        if (posGrabber.toInt() != -1) {
                        grabber.setPositionDegrees(posGrabber.toInt());
                        Serial.println("Set position of grabber to: " + String(posGrabber.toInt()));
                        } else {
                        Serial.println("Quit");
                        break;
                        }
                    }
                    }
                    break;
                }
                case 17:
                {
                    Serial.println("Command: PID Tuning arm");
                    if (!armHomed || !verticalHomed || !baseHomed) {
                    Serial.println("Claw not fully homed. Can't PID Tune");
                    break;
                    }
                    verticalStage.setPosition(60);
                    base.setPosition(0);
                    DelayManager positionDelay(2000);
                    positionDelay.reset();
                    const int posArm1 = 50;
                    const int posArm2 = 150;
                    int posArm = posArm1;
                    while (true) {
                    if (Serial.available()) {
                        String Pks = Serial.readStringUntil('\n');
                        float Pk = Pks.toFloat();
                        String Dks = Serial.readStringUntil('\n');
                        float Dk = Dks.toFloat();
                        if (Pks == "q" || Pks.length() == 0 || Dks == "q" || Pks.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }
                        if (Dk == -1 || Pk == -1) {
                        Serial.println("Quit");
                        break;
                        } else {
                        Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
                        arm.continuousServo.setPDTuning(Pk, Dk);
                        }
                    }


                    arm.loop();
                    verticalStage.loop();
                    base.loop();
                    if(positionDelay.checkAndReset()) {
                        if (posArm == posArm1) {
                        posArm = posArm2;
                        } else {
                        posArm = posArm1;
                        }
                        Serial.printf("Arm position: %.3f\n", arm.getPosition());
                        Serial.println(posArm);
                        arm.setPosition(posArm);
                    }

                    }
                    break;
                }
                case 18:
                {
                    Serial.println("Command: PID Tuning vertical");     
                    if (!armHomed || !verticalHomed || !baseHomed) {
                    Serial.println("Claw not fully homed. Can't PID Tune");
                    break;
                    }   
                    base.setPosition(0);
                    arm.setPosition(100);
                    DelayManager positionDelay(2000);
                    positionDelay.reset();
                    const int posVert1 = 30;
                    const int posVert2 = 70;
                    int posVert = posVert1;
                    while (true) {
                    if (Serial.available()) {
                        String Pks = Serial.readStringUntil('\n');
                        float Pk = Pks.toFloat();
                        String Dks = Serial.readStringUntil('\n');
                        float Dk = Dks.toFloat();
                        if (Pks == "q" || Pks.length() == 0 || Dks == "q" || Pks.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }
                        if (Dk == -1 || Pk == -1) {
                        Serial.println("Quit");
                        break;
                        } else {
                        Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
                        verticalStage.continuousServo.setPDTuning(Pk, Dk);
                        }
                    }

                    arm.loop();
                    verticalStage.loop();
                    base.loop();

                    if(positionDelay.checkAndReset()) {
                        if (posVert == posVert1) {
                        posVert = posVert2;
                        } else {
                        posVert = posVert1;
                        }
                        Serial.printf("Vertical position: %.3f\n", verticalStage.getPosition());
                        verticalStage.setPosition(posVert);
                    }
                    }
                    break;
                }
                case 19:
                {
                    Serial.println("Command: PID Tuning base");
                    if (!armHomed || !verticalHomed || !baseHomed) {
                    Serial.println("Claw not fully homed. Can't PID Tune");
                    break;
                    }
                    verticalStage.setPosition(60);
                    arm.setPosition(100);
                    DelayManager positionDelay(2000);
                    positionDelay.reset();
                    const int posBase1 = -10;
                    const int posBase2 = 10;
                    int posBase = posBase1;
                    while (true) {
                    if (Serial.available()) {
                        String Pks = Serial.readStringUntil('\n');
                        float Pk = Pks.toFloat();
                        String Dks = Serial.readStringUntil('\n');
                        float Dk = Dks.toFloat();
                        if (Pks == "q" || Pks.length() == 0 || Dks == "q" || Pks.length() == 0) {
                        Serial.println("Quit");
                        break;
                        }

                        if (Dk == -1 || Pk == -1) {
                        Serial.println("Quit");
                        break;
                        } else {
                        Serial.printf("Set to Pk = %.3f and Dk = %.3f\n", Pk, Dk);
                        base.continuousServo.setPDTuning(Pk, Dk);
                        }
                    }

                    arm.loop();
                    verticalStage.loop();
                    base.loop();

                    if(positionDelay.checkAndReset()) {
                        if (posBase == posBase1) {
                        posBase = posBase2;
                        } else {
                        posBase = posBase1;
                        }
                        Serial.printf("Base position: %.3f\n", base.getPosition());
                        base.setPosition(posBase);
                    }
                    }
                break;
                }

                case 20:
                {
                arm.continuousServo.logPIDOutput = !arm.continuousServo.logPIDOutput;
                break;
                }

                case 21:
                {
                verticalStage.continuousServo.logPIDOutput = !verticalStage.continuousServo.logPIDOutput;
                break;
                }

                case 22:
                {
                    base.continuousServo.logPIDOutput = !base.continuousServo.logPIDOutput;
                    break;
                }

                case 23:
                {
                    sensePet();
                    Serial.println("out of sense");
                    break;
                }

                case 24:
                {
                    arm.homingSequence();
                    verticalStage.homingSequence();
                    verticalStage.setPosition(60);
                    while (!verticalStage.reachedTarget()) {
                        verticalStage.loop();
                    }
                    base.homingSequence();
                    verticalStage.setPosition(100);
                    while (!verticalStage.reachedTarget()) {
                        verticalStage.loop();
                    }
                    base.setPosition(0); 
                    while (!base.reachedTarget()) {
                        base.loop();
                    }
                    arm.setPosition(120); 
                    while (!arm.reachedTarget()) {
                        arm.loop();
                    }
                    armHomed = true;
                    verticalHomed = true;
                    baseHomed = true;
                    break;
                }

                case 25:
                {
                    calibrateMagnet();
                    break;
                }
                // case -1:
                // {
                // quit = true;
                // break;
                // }
            }
        }
        if (quit) {
            Serial.println("exit claw tester");
            break;
        }
    }
}

float getMagnetReadingMagSq() {
    // Remember to benchmark
    float sampleSize = 5;
    float x_tot = 0;
    float y_tot = 0;
    float z_tot = 0;
    sensors_event_t event;

    // float HARD_IRON_OFFSET_X = -20.3;
    // float HARD_IRON_OFFSET_Y = 87.5;
    // float HARD_IRON_OFFSET_Z = 19.3;

 

    for (int i = 0; i < sampleSize; i++) {
        lis3mdl.getEvent(&event);
        x_tot += event.magnetic.x - HARD_IRON_OFFSET_X;
        y_tot += event.magnetic.y - HARD_IRON_OFFSET_Y;
        z_tot += event.magnetic.z - HARD_IRON_OFFSET_Z;
    }
    x_tot /= sampleSize;
    y_tot /= sampleSize;
    z_tot /= sampleSize;

    // Serial.printf("X Reading: %.2f\n", x_tot);
    // Serial.printf("Y Reading: %.2f\n", y_tot);
    // Serial.printf("Z Reading: %.2f\n", z_tot);
    return x_tot*x_tot + y_tot*y_tot + z_tot*z_tot;
}

void sensePet() {
    int samples = 0;
    Serial.println("Sensing pet start");
    // Params:

    const int MAGNETIC_THRESHOLD = 1000;

    // while (!arm.reachedTarget() || !base.reachedTarget() || !verticalStage.reachedTarget()) {
    //     Serial.println("Going to initial");
    //     // Serial.println(getMagnetReadingMagSq());
    //     arm.loop();
    //     // verticalStage.loop();
    //     base.loop();
    // }
    Serial.println("Initial position reached");

    int baseInit = base.getPosition();
    int verticalInit = verticalStage.getPosition();
    int armInit = arm.getPosition();


    // Try find best angle

    int sweepAngle = 30; // one side from initial position (so movement 2x sweepangle)
    int sweepAngle2 = 20;
    int sweepAngle3 = 10;

    int maxMagnetReading = 0;
    int maxMagnetReadingPos = 0;

    // First sweep
    base.setPosition(baseInit + sweepAngle);
    while (!base.reachedTarget()) {
        // Serial.println("Moving correct base to position");
        base.loop();
        arm.loop();
        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);
        if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }
    base.setPosition(baseInit - sweepAngle);
    while (!base.reachedTarget()) {
        // Serial.println("Moving correct base to position");
        base.loop();
        arm.loop();
        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);
        if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }

    // Second sweep
    base.setPosition(maxMagnetReadingPos+sweepAngle2);
    while (!base.reachedTarget()) {
        Serial.println("Moving to other base position");
        base.loop();
        arm.loop();

        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);

        if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }
    base.setPosition(maxMagnetReadingPos-sweepAngle2);
    while (!base.reachedTarget()) {
        Serial.println("Moving to other base position");
        base.loop();
        arm.loop();

        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);

        if (currentReading > maxMagnetReading && currentReading > MAGNETIC_THRESHOLD) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }

    // Third sweep
    base.setPosition(maxMagnetReadingPos+sweepAngle3);
    while (!base.reachedTarget()) {
        Serial.println("Moving correct base to position");
        base.loop();
        arm.loop();
        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);
        if (maxMagnetReading < currentReading) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }
    base.setPosition(maxMagnetReadingPos-sweepAngle3);
    while (!base.reachedTarget()) {
        Serial.println("Moving correct base to position");
        base.loop();
        arm.loop();
        float currentReading = getMagnetReadingMagSq();
        Serial.println(currentReading);
        if (maxMagnetReading < currentReading) {
        maxMagnetReading = currentReading;
        maxMagnetReadingPos = base.getPosition();
        }
        samples += 1;
    }

    base.setPosition(maxMagnetReadingPos);
    while (!base.reachedTarget()) {
        Serial.println("moving to max reading position");
        base.loop();
        arm.loop();

    }

    // Serial.printf("Max magnet reading position: %d\n", maxMagnetReadingPos);
    // Serial.printf("Done, samples: %d\n", samples);

    // // Try to find best position
    // int armExtension = 50;
    // float maxMagnetReadingArm = 0;
    // float maxMagnetReadingArmPos = 0;
    // arm.setPosition(armInit + armExtension);
    // while (!arm.reachedTarget()) {
    //   base.loop();
    //   arm.loop();
    //   float currentReading = getMagnetReadingMagSq();
    //   if (maxMagnetReadingArm < currentReading) {
    //     maxMagnetReadingArm = currentReading;
    //     maxMagnetReadingArmPos = arm.getPosition();
    //   }
    // }

    // arm.setPosition(maxMagnetReadingArmPos);
    // while (!arm.reachedTarget()) {
    //   arm.loop();
    //   base.loop();
    // }
}



void calibrateMagnet() {
    float min_x, max_x, mid_x;
    float min_y, max_y, mid_y;
    float min_z, max_z, mid_z;
    
    sensors_event_t event;
    lis3mdl.getEvent(&event);

    min_x = max_x = event.magnetic.x;
    min_y = max_y = event.magnetic.y;
    min_z = max_z = event.magnetic.z;
    delay(10);

    while (true) {
        lis3mdl.getEvent(&event);
        float x = event.magnetic.x;
        float y = event.magnetic.y;
        float z = event.magnetic.z;
        
        Serial.print("Mag: (");
        Serial.print(x); Serial.print(", ");
        Serial.print(y); Serial.print(", ");
        Serial.print(z); Serial.print(")");

        min_x = min(min_x, x);
        min_y = min(min_y, y);
        min_z = min(min_z, z);

        max_x = max(max_x, x);
        max_y = max(max_y, y);
        max_z = max(max_z, z);

        mid_x = (max_x + min_x) / 2;
        mid_y = (max_y + min_y) / 2;
        mid_z = (max_z + min_z) / 2;
        Serial.print(" Hard offset: (");
        Serial.print(mid_x); Serial.print(", ");
        Serial.print(mid_y); Serial.print(", ");
        Serial.print(mid_z); Serial.print(")");  
        HARD_IRON_OFFSET_X = mid_x;
        HARD_IRON_OFFSET_Y = mid_y;
        HARD_IRON_OFFSET_Z = mid_z;


        Serial.print(" Field: (");
        Serial.print((max_x - min_x)/2); Serial.print(", ");
        Serial.print((max_y - min_y)/2); Serial.print(", ");
        Serial.print((max_z - min_z)/2); Serial.println(")");    
        delay(10); 

        if (Serial.available()) {
            break;
        }
    }
}

void test() {
  //   last_x = x_tot/sampleSize;
  // last_y = y_tot/sampleSize;
  // last_z = z_tot/sampleSize;
  

  // float lastReading = getMagnetReadingMagSq();
  // int lastPosition = base.getPosition();
  // int bestPosition = lastPosition;
  // float bestReading = lastReading;
  // while (true) {
  //   base.loop();
  //   float currentReading = getMagnetReadingMagSq();
  //   int currentPosition = base.getPosition();
  //   // Serial.printf("Squared reading: %.2f\n", getMagnetReadingMagSq());
  //   Serial.printf("Change squared reading: %.2f\n", getMagnetReadingMagSq());

  //   float delta = currentReading - lastReading;
  //   Serial.printf("Change in reading: %.2f\n",delta);
  //   Serial.printf("Current position: %d\n", currentPosition);
  //   Serial.printf("Best position: %d\n", bestPosition);
  //   Serial.printf("Best reading: %.2f\n", bestReading);
  //   Serial.println("---");

  //   if (abs(delta) >= abs(bestReading)) {
  //     bestPosition = currentPosition;
  //     bestReading = currentReading;
  //   }

  //   lastReading = currentReading;
  //   delay(500);
  // }
}