#include "WPILib.h"
#include "stdio.h"
#include "math.h"
class Robot: public SampleRobot
{
	Talon rightDriveMotor;
	Talon leftDriveMotor;
	Talon rightIntakeWheel;
	Talon leftIntakeWheel;
	Talon rightBallScrew;
	Talon leftBallScrew;
	DoubleSolenoid intakeWheelsActuation;
	DoubleSolenoid canGrabber;
	DoubleSolenoid canLifter;
	DoubleSolenoid crateHolder;
	DoubleSolenoid cratePusher;
	DoubleSolenoid *SolenoidArray[] = new DoubleSolenoid[5];
	Joystick mainDriveStick; // arcade drive joystick
	Joystick secondaryDriveStick; //joystick for left side tank drive
	Joystick manipulatorStick;
	DoubleSolenoid.Value kOff;
	bool twistDrive=false;
public:
	Robot() :
			rightDriveMotor(0),
			leftDriveMotor(1),
			rightIntakeWheel(2),
			leftIntakeWheel(3),
			rightBallScrew(4),
			leftBallScrew(5),
			intakeWheelsActuation(0,1),
			canGrabber(2,3),
			canLifter(4,5),
			crateHolder(6,7),
			cratePusher(8,9),
			mainDriveStick(0),
			secondaryDriveStick(1),
			manipulatorStick(2)
	{}
	float smoothJoyStick(float joyInput)
	{
		return powf(joyInput,3);
	}
	float getAdjustedThrottle()
	{
		return 1-((mainDriveStick.GetThrottle()+1)/2);
	}

	float setDrive(float left, float right)
	{
		leftDriveMotor.Set(-left);
		rightDriveMotor.Set(right);
	}
	bool oppositeBoolean(bool input)
	{
		return 1-input;
	}
	void noTwistArcadeDrive() //the motor's speed is the joystick input multiplied by speedCoef, this way the crazy drivetrain can be slowed down
	{
		float x;
		float y;
		float adjustedThrottle=getAdjustedThrottle(); //changes throttle from: -1 is throttle up and 1 is throttle down
		x=mainDriveStick.GetX();
		y=smoothJoyStick(mainDriveStick.GetY())*adjustedThrottle;
		setDrive((y-x), (y+x)); //the right side runs where -1 is full speed forward
	}
	void twistArcadeDrive()
	{
		float z;
		float y;
		float adjustedThrottle=getAdjustedThrottle(); //changes throttle from: -1 is throttle up and 1 is throttle down
		z=mainDriveStick.GetZ();
		y=smoothJoyStick(mainDriveStick.GetY())*adjustedThrottle;
		setDrive((y-z), (y+z)); //the right side runs where -1 is full speed forward
	}
	void tankDrive()
	{
		float right=smoothJoyStick(mainDriveStick.GetY());
		float left=smoothJoyStick(secondaryDriveStick.GetY());
		setDrive(left, right);
	}
	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			if (secondaryDriveStick.GetButtonCount()!=NULL)
			{
				tankDrive();
			}
			else if (twistDrive)
			{
				twistArcadeDrive();
			}
			else
			{
				noTwistArcadeDrive();
			}
			if (mainDriveStick.GetRawButton(2))
			{
				twistDrive = oppositeBoolean(twistDrive);
			}
			Wait(0.005);				// wait for a motor update time
		}
	}

};

START_ROBOT_CLASS(Robot);
