
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <ctime>
#include <string>




//// CONFIG



int l1 = -16;
int l2 = -15;
int l3 = -14;
int r1 = 17;
int r2 = 19;
int r3 = 18;

signed char a = char(l1);
signed char b = char(l2);
signed char c = char(l3);
signed char d = char(r1);
signed char e = char(r2);
signed char f = char(r3); 

pros::MotorGroup left_motors({a, b, c}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({d, e, f}, pros::MotorGearset::blue);

pros::Motor flexWheelIntake(-12, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees);

pros::Imu imu(7);

pros::Motor armMotor(3, pros::MotorGearset::rpm_600, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut mogo('A');
pros::adi::DigitalOut doinker('G');

pros::Rotation verticalTracking(15);

pros::Rotation horizontalTracking(15);
pros::Rotation LBtracking(-2);


pros::Controller controller(pros::E_CONTROLLER_MASTER);






// Controller LCD screen:


/*
Line 1: Bat: X% |
Line 2: Errors: (e.g. dc IMU)              
Line 3: Log
*/


lemlib::Drivetrain drivetrain(
    &left_motors,
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
// Intake is a percentage, that means 100 is 127
int intake = 0; 

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

void flexWheelIntakeFunc() {
    int fwamt = 0;
    int yesfw = 0;
    while (true) {
        // Spin the intake if on
        int intakespd = intake * 1.27;
        flexWheelIntake.move(intakespd );

        // Montior for highest speed
        if (intakespd > 100) {
            fwamt++; 
            if (fwamt > 50 && flexWheelIntake.get_actual_velocity() < 25 && yesfw < 200) {
                pros::lcd::print(5, "Actual velocity: %i",flexWheelIntake.get_actual_velocity());
                // Move it back then fwd again
                flexWheelIntake.move(-127);
                pros::delay(150 + yesfw);
                fwamt = 20;
                yesfw += 60;
            }

        } else {
            fwamt = 0;
            if (yesfw > 0) {yesfw--;}
        }

        // Save resources
        pros::delay(10);
    }
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
    LBtracking.reset();
    LBtracking.reset_position();

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
    LBtracking.set_reversed(true);
    pros::Task intakeTask(flexWheelIntakeFunc);
    pros::Task screenTask([&]() {
        int bruh = 0;
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(2, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(3, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(4, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            pros::lcd::print(7, "Angle: %i, %i", LBtracking.get_angle()/100,LBtracking.get_position()/100);

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



void getMogo() {
    mogo.set_value(1);
    pros::delay(500);
}


void stop() {
    pros::delay(2000);flexWheelIntake.move(0);pros::delay(1000000);
}
void autonomous() {
   
}

int conveyTurnAmt = 0;


pros::c::optical_rgb_s_t rgb_value;


// Arm motor


int armMotorCounter = -1;


bool nomoveflex = false;
bool nomovearm = false;

/**
* Moves the arm motor to an angle
* Speed is a percentage value. 100 is 127 in voltage
* Precision is a delay to save resources. Lower is more accurate and higher is more efficient 
*/
void LBmoveToAngle(int angle, int speed = 100, int precision = 5) {
    // If the rotational sensor is connected, then use the rotational sensor to move the ladybrown
    // This is done by constantly checking the angle of the rotatoional sensor until it meets the angle specified
    // However, if the rotational sensor is not connected, for example, it disconnects, it should use the internal motor encoding units instead

    if (LBtracking.is_installed()) {
        // If move more
        if (angle * 100 > LBtracking.get_position() - 5) {
            armMotor.move(speed * 1.27);
            while (LBtracking.get_position() < angle * 100) {
                pros::delay(precision);
            }
        } else {
            armMotor.move(-(speed * 1.27));
            while (LBtracking.get_position() > angle * 100) {
                pros::delay(precision);

            }
        }
        armMotor.move(0);
    } else {
        // This function should only run if the rotational sensor is not installed
        pros::lcd::print(6, "LB tracking gone, using IME");
        armMotor.move_absolute(angle * 5, speed * 10);
    }
    
}

void autoMoveArm() {
    // Wall stake mech code
    // Should be ran inside a thread because it uses delay commands which can interrupt the main while true loop

    nomovearm = true;
    
    armMotorCounter++;
    
    if (armMotorCounter == 0) {
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
        pros::delay(10);
        // Moves the arm to catch the ring

        LBmoveToAngle(28, 50, 2);
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);


    } else  {
        // Complicated steps to push it down for the next step
        nomoveflex = true;
        intake = -20;
        pros::delay(100);
        nomoveflex = false;
        intake = -5;
        pros::delay(100);
        // Now score
        armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        LBmoveToAngle(135, 100, 5);
        pros::delay(200);
        LBmoveToAngle(0, 100);
        armMotorCounter = -1;
    }
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




        // Delay to save resources. This also makes sure that code runs properly (e.g. 10 ticks = 100 milliseconds)
        pros::delay(10);

        // intake
        intake = r2 * 100;





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



