
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <ctime>
#include <string>




//// CONFIG



int l1 = -20;
int l2 = -18;
int l3 = -17;
int r1 = 10;
int r2 = 8;
int r3 = 7;

signed char a = char(l1);
signed char b = char(l2);
signed char c = char(l3);
signed char d = char(r1);
signed char e = char(r2);
signed char f = char(r3); 

pros::MotorGroup left_motors({a, b, c}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({d, e, f}, pros::MotorGearset::blue);

pros::Motor flexWheelIntake(5, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::Imu imu(12);

pros::Motor armMotor(3, pros::MotorGearset::rpm_600, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut mogo('A');
pros::adi::DigitalOut doinker('B');

pros::Rotation verticalTracking(15);

pros::Rotation horizontalTracking(15);

pros::Controller controller(pros::E_CONTROLLER_MASTER);






// Controller LCD screen:


/*
Line 1: Bat: X% |
Line 2: Errors: (e.g. dc IMU)              
Line 3: Log
*/








lemlib::Drivetrain drivetrain(&left_motors,
                              &right_motors,
                              11.9,
                              lemlib::Omniwheel::NEW_275,
                              400,
                            6
);




lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                                                      3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);




// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0.05, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              4.420714285714281, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);






pros::Rotation vs(15);


// lemlib::TrackingWheel vertical_tracking_wheel(&vs, lemlib::Omniwheel::NEW_275, -6);




lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu//&imu // inertial sensor
);




// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.02 // expo curve gain
);




// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  5, // minimum output where drivetrain will move out of 127
                                  1.018 // expo curve gain
);




// create the  uwu Jacob Kim is gay
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors,
                        &throttle_curve,
                        &steer_curve
);


//Bond Liu is an opp and a loser
int team = 2;
int auton = 0;


std::string autonDisplay[2] = {"left", "right"};
std::string teamDisplay[3] = {"Red", "Blue", "None"};


void on_right_button() {
    pros::delay(1000);
    autonomous();


}


void on_center_button() {
    // 1 == blue        
    team++;


    if (team == 3) {
        team = 0;
    }      
    pros::lcd::print(0, "Team: %s          ", teamDisplay[team]);


}


void on_left_button() {
    auton = !auton;
    pros::lcd::print(1, "You are %s side auton, on %s          ", autonDisplay[auton], teamDisplay[team]);
}
bool imudc = false;


void test_drive_single(int port) {
    pros::Motor left_mtr(port);
    left_mtr.move(127);
    pros::delay(500);
    left_mtr.move(0);
    pros::delay(500);

}
void test_drive() {
	test_drive_single(l1);
    test_drive_single(l2);
    test_drive_single(l3);
    test_drive_single(r1);
    test_drive_single(r2);
    test_drive_single(r3);
}


void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors


    armMotor.tare_position();

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms


    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs


    pros::lcd::print(0, "SELECT THE TEAM");
    pros::lcd::print(1, "SELECT THE AUTON");


   
    pros::lcd::register_btn1_cb(on_center_button);
    pros::lcd::register_btn0_cb(on_left_button);
    pros::lcd::register_btn2_cb(on_right_button);
    controller.clear();
    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        int bruh = 0;
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(2, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(3, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(4, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
           
           


           
            if (!imu.is_installed()) {
                imudc = true;
            }
            if (imudc) {
                           
                controller.print(2, 0, "IMU Disconnected");
            }




            // delay to save resources
            pros::delay(100);


            // bruh++;
            // if (bruh == 30) {
            //     controller.clear();
            // }
            // if (bruh % 5 == 0) {
            //     int theta = static_cast<int>(round(chassis.getPose().theta));
            //     controller.print(0, 0, "%.1f, %.1f, %i          ", chassis.getPose().x, chassis.getPose().y, (theta % 360));
            // } else if (bruh % 5 == 4) {
            //     controller.print(1, 0, "%.0f%% | %.0f", pros::battery::get_capacity(), controller.get_battery_capacity() );
            // }
        }
    });
}

int sign(int num){
    if (num == 0) return 0;
    return (num >= 0) ? 1 : -1;
}

float lateralInputRemap(float input, float scale) {
	if (scale != 0) {
		auto eq1 = [](float x, int a) {
            return powf(a, std::abs(x) - 127) * (std::abs(x)) *  sign(x);
        };


        return (127.0-10.0)/127.0 * (eq1(input, scale) * 127.0/(eq1(127, scale)));
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

    angular = angularInputRemap(angular, 7.5);

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






void rightRED() {
    // Doinker the mogo
    chassis.moveToPose(2, 38, 10, 1000, {.lead=0.05}, false);
    doinker.set_value(1);
    chassis.moveToPoint(0, 15, 1000, {.forwards=false}, false);
    doinker.set_value(0);
    pros::delay(400);




    // Move to get the stake
    chassis.turnToHeading(130, 800);
    chassis.moveToPoint(-8.1, 28.1, 1000, {.forwards=false, .maxSpeed=70}, false);
    mogo.set_value(1);
    pros::delay(200);


    // Move to get the 4 rings
    chassis.moveToPoint(-3, 5.1, 1000);
    chassis.moveToPoint(20.4, -5, 1000);


    doinker.set_value(1);


    flexWheelIntake.move(-127);


    // pros::delay(200);


    chassis.turnToHeading(0, 1000, {.maxSpeed=70}, false);


    doinker.set_value(0);


    chassis.turnToHeading(120, 500);
    // chassis.resetLocalPosition();


    chassis.moveToPoint(30, -4, 3000, {}, false);


    chassis.resetLocalPosition();
    chassis.moveToPoint(-4, 0, 1000);


    // flexWheelIntake.move(0);


}


void leftRED() {
    mogo.set_value(0);


    // Score alliance goal
    chassis.moveToPoint(0, -11, 700, {.forwards=false}, true);
    chassis.turnToHeading(90, 500, {}, true);
    chassis.moveToPoint(-3, -11, 500);
    flexWheelIntake.move(-127);
    pros::delay(600);


    flexWheelIntake.move(0);


    // // try to score the ring
    // chassis.moveToPose(-18, -15, -120, 2000, {.lead=0.7}, false);


    // // Reset for next
    chassis.moveToPoint(0, -11, 500);
    chassis.turnToHeading(0, 400, {}, false);


    // Move to get the stacked ring
    flexWheelIntake.move(-80);
    chassis.moveToPose(26, 36, 65, 2000, {.lead=0.3, .minSpeed=0}, false);


    // // Wait a bit to get it in but dont score
    pros::delay(200);
    flexWheelIntake.move_relative(410, -100);


    flexWheelIntake.move(-10);


    // pros::delay(10000000);




    // // move to the stake and get it
    chassis.moveToPoint(25, 15, 1700, {.forwards=false, .maxSpeed=100}, false);
    mogo.set_value(1);
    pros::delay(200);


    // // score and turn to the middle rings
    flexWheelIntake.move(-127);
    chassis.turnToHeading(45, 300, {}, false);


    chassis.resetLocalPosition();
    // chassis.moveToPose(19, 26, 0, 3000, {.lead=0.4}, false);
   
    // try to score the blue ring in the middle of the field




    chassis.moveToPoint(-4, -16, 3000, {.forwards=false, .maxSpeed=50});
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
}


void leftBLUEmogorush() {
    chassis.moveToPose(2, 37, 10, 1100, {.lead=0.05}, false);
    doinker.set_value(1);
    chassis.moveToPoint(0, 6, 1000, {.forwards=false}, false);
    doinker.set_value(0);
    pros::delay(100);
    chassis.turnToHeading(-120, 800);


    chassis.moveToPoint(32, 26, 1000, {.forwards=false, .maxSpeed=70}, false);
    mogo.set_value(1);
    pros::delay(500);


    chassis.moveToPoint(-4, -7.7, 2000);


    doinker.set_value(1);


    flexWheelIntake.move(-127);


    // chassis.turnToHeading(-300, 1000);


    chassis.moveToPose(16, -3, -270, 2000, {}, false);
    // chassis.moveToPoint(19, 2, 1200, {.maxSpeed=80}, false);
    // doinker.set_value(0);


    chassis.resetLocalPosition();
    flexWheelIntake.move(0);


    chassis.moveToPoint(-8, 0, 1000, {.forwards=false}, false);






}


void leftBLUEnormal() {
    chassis.moveToPoint(0, -30, 1900, {.forwards=false}, true);
    chassis.turnToHeading(35, 250);
    chassis.moveToPoint(-8, -45.5, 1000, {.forwards=false}, false);


    mogo.set_value(1);


    flexWheelIntake.move(-127);
    pros::delay(500);


    chassis.moveToPoint(-12, -22, 2000, {.maxSpeed=100}, false);


    pros::delay(200);


    chassis.moveToPoint(-8, -24, 1000, {.forwards=false}, false);


    pros::delay(300);


    mogo.set_value(0);


    flexWheelIntake.move(127);
    chassis.moveToPoint(-14, -20, 1000, {.maxSpeed=100});
    chassis.turnToHeading(65, 1000, {}, false);


    chassis.resetLocalPosition();
    flexWheelIntake.move(0);


    chassis.moveToPoint(-24, -12, 1000, {.forwards=false}, false);


    mogo.set_value(1);
    // flexWheelIntake.move(0);
    // pros::delay(100);


    chassis.moveToPoint(5, 5, 1000);
    doinker.set_value(1);


    chassis.moveToPoint(20, 24, 1000);


    // pros::delay(400);


    chassis.turnToHeading(-80, 600, {.maxSpeed=100}, false);
    // doinker.set_value(0);
    // pros::delay(200);
    // chassis.turnToHeading(0, 400);
    // pros::delay(300);
    // // chassis.moveToPoint(12, 30, 2000);


    // flexWheelIntake.move(-127);


    // chassis.moveToPoint(12, 22, 2000);
}


void rightBLUE() {


    mogo.set_value(0);


    // Score alliance goal
    chassis.moveToPoint(0, -10, 700, {.forwards=false}, true);
    chassis.turnToHeading(-90, 500, {}, false);
    flexWheelIntake.move(-127);
    pros::delay(500);


    flexWheelIntake.move(0);


    // // try to score the ring
    // chassis.moveToPose(-18, -15, -120, 2000, {.lead=0.7}, false);


    // // Reset for next
    chassis.turnToHeading(0, 400, {}, false);


    // Move to get the stacked ring
    flexWheelIntake.move(-127);
    chassis.moveToPose(-42, 30, -65, 2000, {.lead=0.3, .minSpeed=0}, false);


    // Wait a bit to get it in but dont score
    pros::delay(350);
    flexWheelIntake.move_relative(410, -100);


    flexWheelIntake.move(-20);




    // move to the stake and get it
    chassis.moveToPoint(-37, 9.2, 1300, {.forwards=false, .maxSpeed=100}, false);
    mogo.set_value(1);
    pros::delay(200);


    // score and turn to the middle rings
    flexWheelIntake.move(-127);
    chassis.turnToHeading(-45, 300, {}, false);


    chassis.resetLocalPosition();
    // chassis.moveToPose(-16, 24, 0, 3300, {.lead=0.4}, false);
   
    // try to score the blue ring in the middle of the field
    chassis.moveToPoint(3.2, -21, 3000, {.forwards=false, .maxSpeed=50});
    // flexWheelIntake.move(0);




    // chassis.turnToHeading(-135, 600);


    // chassis.moveToPose(, float y, float theta, int timeout)








    // pros::delay(10);
    // chassis.turnToHeading(0, 500);
    // flexWheelIntake.move(-50);
    // chassis.moveToPoint(-35, 16, 900, {.forwards=false}, false);
    // mogo.set_value(1);
    // pros::delay(20);
    // flexWheelIntake.move(-127);


    // pros::delay(50);
    // chassis.moveToPoint(-58, 30, 2000);
    // chassis.turnToHeading(-180, 1000);
    // chassis.moveToPoint(-51, 10, 1000);
    // chassis.turnToHeading(-180, 100000);
    // flexWheelIntake.move(0);
}


void getMogo() {
    mogo.set_value(1);
    pros::delay(500);
}


void stop() {
    pros::delay(2000);flexWheelIntake.move(0);pros::delay(1000000);
}
void autonomous() {
    // this is the skills auton  codew
    mogo.set_value(0);


    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);


    chassis.setPose(0, 0, 0);






    // SCORE ALLIANCE GOAL
    flexWheelIntake.move(-127);
    pros::delay(1000);
    flexWheelIntake.move(0); // not necessary


    // SCORE LEFT SIDE


    // get stake
    chassis.moveToPoint(0, 12, 500);
    chassis.turnToHeading(90, 500);    
    chassis.moveToPoint(-20, 12, 900, {.forwards=false}, false);
    getMogo();


    // get first ring
    chassis.turnToHeading(0, 500);    


    flexWheelIntake.move(-127);


    chassis.moveToPoint(-24, 28.8, 2000, {.forwards=true});
    chassis.moveToPoint(-48, 32, 3000, {.forwards=true});
    chassis.moveToPoint(-43, 32, 500, {.forwards=false});
    chassis.moveToPoint(-44, 0, 3000, {.forwards=true, .maxSpeed=90});
    chassis.moveToPoint(-44, 4, 2000, {});


    chassis.moveToPoint(-44, 14, 1000, {});
    chassis.moveToPoint(-60, 14, 1000);


    // chassis.turnToHeading(0, 500);


    chassis.moveToPoint(-70, -10, 3000, {.forwards=false}, false);


    mogo.set_value(0);


    pros::delay(1000);
    chassis.resetLocalPosition();
    flexWheelIntake.move(0);
    //reset pos by ramming the wall
    chassis.moveToPoint(0, 20, 1000);
    chassis.turnToHeading(-90, 500);
    chassis.moveToPoint(-50, 20, 1000);
    chassis.resetLocalPosition();




    // SCORE RIGHT SIDE


    chassis.moveToPose(48, -3.5, -60, 4000, {.forwards=false, .lead=0.2});








    // SCORE WALL STAKE x 1


    // SCORE 2 RINGS


    // HANG


}






int conveyTurnAmt = 0;




pros::c::optical_rgb_s_t rgb_value;


// Arm motor


int armMotorCounter = 0;


bool nomoveflex = false;
bool nomovearm = false;

void autoMoveArm() {
    // Wall stake mech code
    // Should be ran inside a thread because it uses delay commands which can interrupt the main while true loop

    nomovearm = true;
    // Arm motor has a gear ratio of 5 therefore we need to multiply every angle by 5

    
    if (armMotorCounter == 0) {
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        pros::delay(10);
        // Moves the arm to catch the ring
        armMotor.move_absolute(42 * 5, 200);

    } else if (armMotorCounter == 1) {
        // Complicated steps to push it down for the next step
        nomoveflex = true;
        armMotor.move(-20);
        flexWheelIntake.move(50);
        pros::delay(20);
        armMotor.move(0);
        pros::delay(100);
        flexWheelIntake.move(5);
        nomoveflex = false;
    } else if (armMotorCounter == 2) {        
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        pros::delay(10);
        armMotor.move_absolute(170*5, 10000);
        pros::delay(550);
        armMotor.move_absolute(0, 500);
        armMotorCounter = -1;


    }
    armMotorCounter++;
    nomovearm = false;

}

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
        armMotor.move_absolute(120 * 5, 127);
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    } else {
        armMotor.move_absolute(0, 127);
    }
 
}


// Driver code
void opcontrol() {


   
    // Before the while true loop, set the arm motor to brake mode instead of coast to prevent slipping
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
   


    // Main while true loop
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


        int downArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
        int leftArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);
        int upArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);


        // Should manual arm be on, arm motors can be moved using the joystick
        if (manualarm && !nomovearm) {armMotor.move(-rightY);}


        // Mogo mech code
        if (r1) {
            mogoValue = !mogoValue;
            mogo.set_value(mogoValue);
            controller.print(2, 0, "Mogo Piston %s          ", mogoValue ? "ON" : "OFF");
        }
        if (l1) {
            pros::Task armmotortask(autoMoveArm);
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
            test_drive(); // Hangs code
        }

        if (y) {
            usebrake = !usebrake;
            // // controller.clear_line(2);
            controller.print(2, 0, "Brake Mode: %s          ", usebrake ? "Hold" : "None");
 
        }
        if (x) {
            manualarm = !manualarm;
            armMotor.tare_position();
            controller.print(2, 0, "Manual Arm %s          ", doinkerValue ? "ON" : "OFF");


        }
        if (upArrow) {
            pros::Task armmotorhangtask(&hang);


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
        arcade(leftY, rightX);




        // A code that does not work but it is intended to detect if the flex wheel intake/chain is "stuck" and therefore should be moved backwards a bit
        int fwmv = flexWheelIntake.get_actual_velocity();
        int fwmv2 = flexWheelIntake.get_target_velocity();
        if (!nomoveflex) {


       
            if (abs(fwmv) < 15 && fwmv2 > 0) {
                flexwheelstuckamt++;
                if (flexwheelstuckamt > 100) {
                    flexwheelstuckamt = 0;
                }
                else if (flexwheelstuckamt > 30) {
                    flexWheelIntake.move(127);
                }
                else {
                    flexWheelIntake.move(  
                        r2 * -127 // Intake
                        + l2 * 127  // Outake


                        + downArrow * -30
                    );
                }
            } else {
                flexwheelstuckamt = 0;
           
                flexWheelIntake.move(  
                    r2 * -127 // Intake
                    + l2 * 127  // Outake


                    + downArrow * -30
                );
            }
        }


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



