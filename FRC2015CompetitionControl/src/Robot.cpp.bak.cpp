#include "WPILib.h"
#include "stdio.h"
#include "math.h"
//#include "time.h"
#define DRIVEGEARRATIO 1/16 //the # of times the wheels turn per motor turn
#define LIFTGEARRATIO 1/12.75 //the # of times the sprocket turns per motor turn
class Robot: public SampleRobot
{
	Talon rightDriveMotor;
	Talon leftDriveMotor;
	Talon intakeWheel;
	Talon leftLiftDrive;
	Talon rightLiftDrive;
	Encoder rightDriveEncoder;
	Encoder leftDriveEncoder;
	Encoder leftLiftEncoder;
	Encoder rightLiftEncoder;
	DoubleSolenoid *intakeWheelsActuation;
	DoubleSolenoid *canGrabber;
	DoubleSolenoid *stackStabilizer;
	DoubleSolenoid *cratePincer;
	DoubleSolenoid *cratePusher;
	Joystick driveGamepad; // arcade drive logitech gamepad
	Joystick manipulatorStick;
	Timer cratePunchTimer;
	bool twistDrive=false;
	bool twoSticks=false;
	bool intakeWheelsActuated=true;
	float intakeWheelSpeed=1.0;
	int punchOutTimeSecs=0.5;
public:
	Robot() :
			rightDriveMotor(0),
			leftDriveMotor(1),
			intakeWheel(2),
			leftLiftDrive(3),
			rightLiftDrive(4),
			rightDriveEncoder(0, 1, true),
			leftDriveEncoder(2, 3),
			leftLiftEncoder(4 ,5),
			rightLiftEncoder(6, 7),
			driveGamepad(0),
			manipulatorStick(1),
			cratePunchTimer()
	{
		intakeWheelsActuation= new DoubleSolenoid(0, 1);
		canGrabber= new DoubleSolenoid(2, 3);
		stackStabilizer= new DoubleSolenoid(4, 5);
		cratePincer= new DoubleSolenoid(6, 7);
		cratePusher= new DoubleSolenoid(8, 9);
	}
	float smoothJoyStick(float joyInput)
	{
		return powf(joyInput,3);
	}
	float getAdjustedThrottle(float defualtThrottleValue)
	{
		if (defualtThrottleValue!=NULL)
		{
			return 1-((defualtThrottleValue+1)/2);
		}
		else
		{
			return 1;
		}
	}

	void setDrive(float left, float right)
	{
		leftDriveMotor.Set(-left);
		rightDriveMotor.Set(right);
	}
	bool oppositeBoolean(bool input)
	{
		return 1-input;
	}
	void togglePiston(DoubleSolenoid *piston)
	{
		int pistonPos=piston->Get();
		if (pistonPos==DoubleSolenoid::kForward || pistonPos==DoubleSolenoid::kOff)
		{
			piston->Set(DoubleSolenoid::kReverse);
		}
		else
		{
			piston->Set(DoubleSolenoid::kForward);
		}
	}
	void cratePunch()
	{
		cratePusher->Set(DoubleSolenoid::kForward);
		cratePunchTimer.Start();
	}
/*
	void noTwistArcadeDrive() //the motor's speed is the joystick input multiplied by speedCoef, this way the crazy drivetrain can be slowed down
	{
		float x;
		float y;
		float adjustedThrottle=getAdjustedThrottle(driveGamepad); //changes throttle from: -1 is throttle up and 1 is throttle down
		x=driveGamepad.GetX();
		y=smoothJoyStick(driveGamepad.GetY())*adjustedThrottle;
		setDrive((y-x), (y+x)); //the right side runs where -1 is full speed forward
	}
	void twistArcadeDrive()
	{
		float z;
		float y;
		float adjustedThrottle=getAdjustedThrottle(driveGamepad); //changes throttle from: -1 is throttle up and 1 is throttle down
		z=driveGamepad.GetZ();
		y=smoothJoyStick(driveGamepad.GetY())*adjustedThrottle;
		setDrive((y-z), (y+z)); //the right side runs where -1 is full speed forward
	}
*/
	void tankDrive()
	{
		float right=smoothJoyStick(driveGamepad.GetRawAxis(4));
		float left=smoothJoyStick(driveGamepad.GetRawAxis(2));
		setDrive(left, right);
	}
	void liftControl()
	{
		float adjustedThrottle=getAdjustedThrottle(manipulatorStick.GetThrottle());
		float speed=smoothJoyStick(manipulatorStick.GetY())*adjustedThrottle;
		rightLiftDrive.Set(speed);
		leftLiftDrive.Set(speed);
	}
	void canControl(int toggleButton) //control trash can grabber
	{
		if (manipulatorStick.GetRawButton(toggleButton))
		{
			togglePiston(canGrabber);
		}
	}
	void crateControl(int pincerToggleButton, int cratePunchButton) //control crate pusher and holder
	{
		if (manipulatorStick.GetRawButton(pincerToggleButton))
		{
			togglePiston(cratePincer);
		}
		if (manipulatorStick.GetRawButton(cratePunchButton))
		{
			cratePunch();
		}

	}
	void intakeWheelControl(int intakeForwardButton, int intakeReverseButton, int intakeActuationToggleButton)
	{
		if (manipulatorStick.GetRawButton(intakeForwardButton))
		{
			intakeWheel.Set(intakeWheelSpeed);
		}
		else if (manipulatorStick.GetRawButton(intakeReverseButton))
		{
			intakeWheel.Set(-intakeWheelSpeed);
		}
		else
		{
			intakeWheel.Set(0);
		}
		if (manipulatorStick.GetRawButton(intakeActuationToggleButton))
		{
			togglePiston(intakeWheelsActuation);
		}
	}
	void stackManagement(int stackStabilizerToggleButton)
	{
		if (abs(driveGamepad.GetRawAxis(4)>0.05) || abs(driveGamepad.GetRawAxis(2)>0.05))
		{
			stackStabilizer->Set(DoubleSolenoid::kForward);
		}
		else if (abs(manipulatorStick.GetY())>0.05)
		{
			stackStabilizer->Set(DoubleSolenoid::kReverse);
		}
		else if (driveGamepad.GetRawButton(stackStabilizerToggleButton))
		{
			togglePiston(stackStabilizer);
		}
	}
	void cratePunchTimeout()
	{
		if (cratePusher->Get()==DoubleSolenoid::kForward && cratePunchTimer.Get()>punchOutTimeSecs)
		{
			cratePunchTimer.Stop();
			cratePunchTimer.Reset();
			cratePusher->Set(DoubleSolenoid::kReverse);
		}
	}
	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			tankDrive();
			intakeWheelControl(1, 5, 3);
			liftControl();
			stackManagement(2);
			canControl(4);
			crateControl(2, 10);
			cratePunchTimeout();
			Wait(0.005);				// wait for a motor update time
		}
	}

};

START_ROBOT_CLASS(Robot);
