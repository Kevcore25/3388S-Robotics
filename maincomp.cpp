#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include <cmath>
#include <ctime>
#include <string>
#include "autons.hpp"


//Bond Liu is an opp and a loser
int team = 2;
int auton = 0;
// Intake is a percentage, that means 100 is 127


std::string autonDisplay[3] = {"left", "right", "awp"};
std::string teamDisplay[3] = {"Red", "Blue", "None"};



// rd::Image image2("", "Cat img");
rd::Selector selector({    
    {"Red Ring Side", leftRed, "", 0},
    {"Red Mogo Rush", rightRed, "", 12},
    {"Blue Mogo Rush", leftBLUE, "", 240},
    {"Blue Ring Side", rightBLUE, "", 200},
    {"Solo AWP Red", soloAWPRed, "", 350},

});
rd::Image catIMG("/usd/cat.bin", "Cat PNG");
rd::Console console;

bool imudc = false;




void test_drive_single(int port) {
   pros::Motor left_mtr(port);
   left_mtr.move(127);
   pros::delay(500);
   left_mtr.move(0);
   pros::delay(500);
}

// void test_drive() {
//    test_drive_single(l1);
//    test_drive_single(l2);
//    test_drive_single(l3);
//    test_drive_single(r1);
//    test_drive_single(r2);
//    test_drive_single(r3);
// }


bool nomoveflex = false;
bool nomovearm = false;


bool onmatch = false;

/**
* Moves the arm motor to an angle
* Speed is a percentage value. 100 is 127 in voltage
* Precision is a delay to save resources. Lower is more accurate and higher is more efficient
*/

bool ejecting = false;
void ejectring() {
    ejecting = true;
    // pros::lcd::print(9, "INTAKE STOP");

    // Keep detecting until the hole is passed
    while (ringsort.get_proximity() >= 120) {
        pros::delay(2); // Add a short delay. 2 is the minimum polling rate of the sensor so this should be 3
    }

    flexWheelIntake.move(0);


    pros::delay(150);
    // LBmoveToAngle(0);


    // armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    ejecting = false;
}


int armMotorCounter = 0;
int armMotorCounterDouble = 0;
int alliancecounter = 0;



void flexWheelIntakeFunc() {
   int fwamt = 0;
   int yesfw = 0;
   while (true) {
       // Spin the intake if on
       int intakespd = intake * 1.27;
       flexWheelIntake.move(intakespd );


       // Montior for highest speed
    // pros::lcd::print(5, "Actual velocity: %i %i %i",flexWheelIntake.get_actual_velocity(), fwamt, yesfw);


       if (intakespd > 100 && (armMotorCounter == 0 && armMotorCounterDouble == 0 && alliancecounter == 0)) {
           fwamt++;
           int prox = ringsort.get_proximity();
           if (fwamt > 250 && flexWheelIntake.get_actual_velocity() < 15 && yesfw < 200) {
            //    pros::lcd::print(5, "Actual velocity: %i",flexWheelIntake.get_actual_velocity());
               // Move it back then fwd again
               flexWheelIntake.move(-127);
               pros::delay(150);
               fwamt = 0;
               yesfw += 30;
           } else {
               // Ring sort function
               if (!onmatch){
               int hue = ringsort.get_hue();
              
               if (!ejecting && (
                   (team == 0 && hue >= 200 && hue <= 270) ||
                   (team == 1 && hue <= 30)
               )) {
                   if (prox >= 100) {} // eject ring should be here
               }
               }
           }


       } else {
           fwamt = 0;
           if (yesfw > 0) {yesfw--;}
       }


       // Save resources
       pros::delay(5);
   }
}


void initialize() {
//    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    chassis.setPose(0.0, 0.0, 0.0);



   armMotor.tare_position();
   LBtracking.reset();
   LBtracking.set_data_rate(5);
   LBtracking.reset_position();


   // the default rate is 50. however, if you need to change the rate, you
   // can do the following.
   // lemlib::bufferedStdout().setRate(...);
   // If you use bluetooth or a wired connection, you will want to have a rate of 10ms




   // for more information on how the formatting for the loggers
   // works, refer to the fmtlib docs




//    pros::lcd::print(0, "SELECT THE TEAM");
//    pros::lcd::print(1, "SELECT THE AUTON");


   ringsort.disable_gesture();
   ringsort.set_integration_time(3);
   ringsort.set_led_pwm(30);


//    pros::lcd::register_btn1_cb(on_center_button);
//    pros::lcd::register_btn0_cb(on_left_button);
   pros::Task screenTask([&]() {
//    pros::lcd::register_btn2_cb(on_right_button);
   controller.clear();
   // thread to for brain screen and position logging
   LBtracking.set_reversed(true);
   pros::Task intakeTask(flexWheelIntakeFunc);
   selector.focus();

       while (true) {
           // print robot location to the brain screen
        //    pros::lcd::print(2, "X: %.4f | Y: %.4f | D: %.4f    ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
        //    pros::lcd::print(4, "T: %.0fC | %.0fC    ", armMotor.get_temperature(), flexWheelIntake.get_temperature()); // heading
        //    pros::lcd::print(6, "DISTANCE: %d | HUE: %f", ringsort.get_proximity(), ringsort.get_hue());
        //    pros::lcd::print(7, "Intake Speed: %i  ", flexWheelIntake.get_actual_velocity());
        //    // log position telemetry

            
           lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
        //    pros::lcd::print(3, "Arm Angle: %.2f   ", LBtracking.get_position()/100);


           if (!imu.is_installed()) {
               imudc = true;
           }
           if (imudc) {
               controller.print(2, 0, "IMU Disconnected");
           }


           // delay to save resources
           pros::delay(100);


       }
   });

}


int sign(int num){
   if (num == 0) return 0;
   return (num >= 0) ? 1 : -1;
}


float lateralInputRemap(float input, float scale) {
   if (scale != 0) {
       auto eq1 = [](float x, float a) {
           return powf(a, std::abs(x) - 127) * (std::abs(x)) *  sign(x);
       };




       return (eq1(input, scale) * 127.0/(eq1(127, scale)));
   }
   return input;
}


float angularInputRemap(float input, float scale) {
   if (scale != 0) {
       return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
   }
   return input;
}




void arcade(int throttle, int angular){
   int deadzone = 5;


   throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
   angular = (abs(angular) <= deadzone) ? 0 : angular;


   throttle =  lateralInputRemap(throttle, 1.021);


   angular = angularInputRemap(angular, 7.5) * 0.6;

   if (throttle + angular < 100){
    angular /= 0.6;
   }


   left_motors.move(throttle + angular);
   right_motors.move(throttle - angular);
}




void disabled() {}








/**
* Runs after initialize(), and before autonomous when connected to the Field
* Management System or the VEX Competition Switch. This is intended for
* competition-specific initialization routines, such as an autonomous selector
* on the LCD.
*
* This task will exit when the robot is enabled and autonomous or opcontrol
* starts.
*/
void competition_initialize() {
   // pros::lcd::register_btn1_cb(on_center_button);
   // pros::lcd::register_btn0_cb(on_left_button);
   // pros::lcd::register_btn2_cb(on_right_button);
}




/**
* Runs the user autonomous code. This function will be started in its own task
* with the default priority and stack size whenever the robot is enabled via
* the Field Management System or the VEX Competition Switch in the autonomous
* mode. Alternatively, this function may be called in initialize or opcontrol
* for non-competition testing purposes.
*
* If the robot is disabled or communications is lost, the autonomous task
* will be stopped. Re-enabling the robot will restart the task, not re-start it
* from where it left off.
*/




// void mtp(float x, float y, float theta, int timeout = 5000, int delay = 50, float curve = 0.5) {
//     // Move to Pose custom function to reduce amount of coding
//     chassis.moveToPose(x,y, theta,timeout, {.horizontalDrift=8, .lead = curve});
//     pros::delay(delay);
// }








void stop() {
   pros::delay(2000);flexWheelIntake.move(0);pros::delay(1000000);
}








/* 
Scores the alliance stake in the start of autonomous.
This is done by using the ladybrown to score the wall stake.
*/
void scoreLB() {

}


bool moreLB = false;



// Arm motor

void armStagesOneRing() {
    if (armMotorCounter == 0) {
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        LBmoveToAngle(18, 30, 1);

    } else if (armMotorCounter == 1) {
        // Complicated steps to push it down for the next step
        intake = -20;
        LBmoveToAngle(155 + moreLB * 35, 100, 5);
        intake = 0;
        moreLB = false;

    } else if (armMotorCounter == 2) {
        LBmoveToAngle(-10, 100, 2, 1500);
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        // resetLBPos();
        armMotorCounter = -1;
    }

    armMotorCounter++;
    nomovearm = false;
}

void armStagesTwoRing() {
    if (armMotorCounterDouble == 0) {
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        LBmoveToAngle(24, 30, 2);
    } else if (armMotorCounterDouble == 1) {
        // intake = 100;
        // pros::delay(20);
        // intake = 0;
        // pros::delay(10);
        intake = -30;
        pros::delay(20);        
        intake = 0;
        LBmoveToAngle(70, 40, 2);
    } else if (armMotorCounterDouble == 2)  {
        // Complicated steps to push it down for the next step
        LBmoveToAngle(160, 100, 5);
    } else if (armMotorCounterDouble == 3) {
        LBmoveToAngle(40, 50, 2);
    } else if (armMotorCounterDouble == 4){
        LBmoveToAngle(160, 100, 5);
    } else if (armMotorCounterDouble == 5){
        LBmoveToAngle(0, 100, 2, 200);
        armMotorCounterDouble = -1;
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        // resetLBPos();
    }

    armMotorCounterDouble++;
    nomovearm = false;
}

void allianceStakeCode() {
   // Wall stake mech code
   // Should be ran inside a thread because it uses delay commands which can interrupt the main while true loop
   move_forward(-6.6, 500, true, {.minSpeed=30});
   moreLB = true;
}


void lbLater() {
    pros::delay(750);
    armStagesOneRing();
}



void autonomous() { 
    // chassis.setPose(0.0, 0.0, 0.0);

    // skills();

    // team 0 = red, team 1 = blue
    // false is left, true is right
    // // Auton selector
    // if      (team == 0 && auton == 0) { leftRed(); }
    // else if (team == 0 && auton == 1)  { rightRed(); }
    // else if (team == 1 && auton == 0) { leftBLUE(); }
    // else if (team == 1 && auton == 1) { rightBLUE(); }
    // else if (team == 0 && auton == 2) { soloAWPRed(); };
    // else if (team == 1 && auton == 2) { soloAWPBlue(); };
    pros::delay(1000);
    selector.run_auton();

}

int conveyTurnAmt = 0;






// Set a bunch of values
bool fwSwitch = 0; // Flex Wheel intake SWITCH
bool mogoValue = false; // Mogo Value switch
bool doinkerValue = false;
bool intakeValue = false;
int debugStatCount = 0;
bool goDetectRing = false;
bool yesdr = false;
bool usebrake = false;
bool manualarm = false;
int flexwheelstuckamt = 0;




bool hangbool = false;
void hang() {
   hangbool = !hangbool;
   if (hangbool) {
       hangADI.set_value(1);




   } else {
       hangADI.set_value(0);
   }
}





// Driver code
void opcontrol() {
   // Before the while true loop, set the arm motor to brake mode instead of coast to prevent slipping
    // armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // onmatch = true;

    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    // Main while true loop

    catIMG.focus();
    while (true) {
        // Get Values of the controller
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);








        int r1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
        int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
        int l1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
        int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);




        int a = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
        int b = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
        int x = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
        int y = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);




        int downArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);
        int leftArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT);
        int upArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP);
        int rightArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);




        // Should manual arm be on, arm motors can be moved using the joystick
        if (manualarm && !nomovearm) {armMotor.move(-rightY);}




        // Mogo mech code
        if (r1) {
            mogoValue = !mogoValue;
            mogo.set_value(mogoValue);
            controller.print(2, 0, "Mogo Piston %s          ", mogoValue ? "ON" : "OFF");
        }
        if (l1 && !movingArmMotor) {
            pros::Task armmotortask(armStagesOneRing);
            controller.print(2, 0, "Arm moving...          ");
        }
        // Buttons
        if (a) {
            doinkerValue = !doinkerValue;
            doinker.set_value(doinkerValue);
            // controller.clear_line(2);
            controller.print(2, 0, "Doinker %s          ", doinkerValue ? "ON" : "OFF");




        }
        if (leftArrow) {
            controller.print(2, 0, "Testing drive...          ");
            // test_drive(); // Hangs code
        }


        if (upArrow) {
            usebrake = !usebrake;
            // // controller.clear_line(2);
            controller.print(2, 0, "Brake Mode: %s          ", usebrake ? "Hold" : "None");
        }
        if (x) {
            manualarm = !manualarm;
            armMotor.tare_position();
            controller.print(2, 0, "Manual Arm %s          ", doinkerValue ? "ON" : "OFF");
        }


        if (y) {
            pros::Task armmotorhangtask(&hang);
        }


        if (downArrow && !movingArmMotor) {
            pros::Task armmotortask2(armStagesTwoRing);
        }


        if (rightArrow) {
            // pros::Task armmotortask3(allianceStakeCode);
            allianceStakeCode();
        }
        // If disconnected, set the bot to be hold mode just in case
        if (controller.is_connected()) {
            if (usebrake) {
                chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
            } else {
                chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            }




        } else {
            chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
        }




        // Function to move the robot
        // arcade(leftY, rightX);
        chassis.arcade(leftY, rightX, false, 0.4);




        // intake
        intake = r2 * 100 + l2 * -100;












        // Delay to save resources. This also makes sure that code runs properly (e.g. 10 ticks = 100 milliseconds)
        pros::delay(10);




        // // Debug
        // debugStatCount++;
        // if (debugStatCount >= 100) { // Runs every 1s
        //     // Print Batery
        //    controller.print(0, 0, "%i | %i | %i", fwmv, fwmv2, flexwheelstuckamt);




        //     // Reset
        //     debugStatCount = 0;




        // }
    }
    }











