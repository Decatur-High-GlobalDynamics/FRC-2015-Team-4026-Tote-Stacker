#include "WPILib.h"
#include "stdio.h"
#include "math.h"
#define DRIVEGEARRATIO 1/16 //the # of times the wheels turn per motor turn
#define LIFTGEARRATIO 1/12.75 //the # of times the sprocket turns per motor turn
class Robot: public SampleRobot
{
	Talon rightDriveMotor;
	Talon leftDriveMotor;
	Talon rightIntakeWheel;
	Talon leftIntakeWheel;
	Talon rightLiftDrive;
	Talon leftLiftDrive;
	Encoder rightDriveEncoder;
	Encoder leftDriveEncoder;
	Encoder liftEncoder; //
	DoubleSolenoid intakeWheelsActuation;
	DoubleSolenoid canGrabber;
	DoubleSolenoid canLifter;
	DoubleSolenoid crateHolder;
	DoubleSolenoid cratePusher;
	Joystick mainDriveStick; // arcade drive joystick
	Joystick secondaryDriveStick; //joystick for left side tank drive
	Joystick manipulatorStick;
	bool twistDrive=false;
	bool intakeWheelsActuated=true;
	float intakeWheelSpeed=1.0;
public:
	Robot() :
			rightDriveMotor(0),
			leftDriveMotor(1),
			rightIntakeWheel(2),
			leftIntakeWheel(3),
			rightLiftDrive(4),
			leftLiftDrive(5),
			rightDriveEncoder(0, 1, true),
			leftDriveEncoder(2, 3),
			liftEncoder(4 ,5),
			intakeWheelsActuation(0, 1),
			canGrabber(2, 3),
			canLifter(4, 5),
			crateHolder(6, 7),
			cratePusher(8, 9),
			mainDriveStick(0),
			secondaryDriveStick(1),
			manipulatorStick(2)
	{}
	float smoothJoyStick(float joyInput)
	{
		return powf(joyInput,3);
	}
	float getAdjustedThrottle(Joystick requestedStick)
	{
		if (mainDriveStick.GetThrottle()!=NULL)
		{
			return 1-((requestedStick.GetThrottle()+1)/2);
		}
		else
		{
			return 1;
		}
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
		float adjustedThrottle=getAdjustedThrottle(mainDriveStick); //changes throttle from: -1 is throttle up and 1 is throttle down
		x=mainDriveStick.GetX();
		y=smoothJoyStick(mainDriveStick.GetY())*adjustedThrottle;
		setDrive((y-x), (y+x)); //the right side runs where -1 is full speed forward
	}
	void twistArcadeDrive()
	{
		float z;
		float y;
		float adjustedThrottle=getAdjustedThrottle(mainDriveStick); //changes throttle from: -1 is throttle up and 1 is throttle down
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
	void liftControl()
	{
		float adjustedThrottle=getAdjustedThrottle(manipulatorStick);
		rightLiftDrive.Set();
		leftLiftDrive.Set();
	}
	void canControl() //control trash can lifter and grabber
	{
		//needs work
	}
	void crateControl() //control crate pusher and holder
	{
		//needs work
	}
	void intakeWheelControl(int intakeMotorButton, int intakeActuationToggleButton)
	{
		if (manipulatorStick.GetRawButton(intakeMotorButton))
		{
			leftIntakeWheel.Set(intakeWheelSpeed);
			rightIntakeWheel.Set(intakeWheelSpeed);
		}
		else
		{
			leftIntakeWheel.Set(intakeWheelSpeed);
			rightIntakeWheel.Set(intakeWheelSpeed);
		}
		if (manipulatorStick.GetRawButton(intakeActuationToggleButton))
		{
			if (intakeWheelsActuated)
			{
				intakeWheelsActuation.Set(intakeWheelsActuation.kForward);
			}
			else
			{
				intakeWheelsActuation.Set(intakeWheelsActuation.kReverse);
			}
		}
		else
		{

		}
	}
	void drive(int twistToggleButton) //automaticly decides which drive setup to use based on available joysticks, eventually remove arcade type toggle
	{
		if (secondaryDriveStick.GetButtonCount()!=NULL)
		{
			tankDrive();
		}
		else if (twistDrive && mainDriveStick.GetAxisCount() > 2)
		{
			twistArcadeDrive();
		}
		else
		{
			noTwistArcadeDrive();
		}
		if (mainDriveStick.GetRawButton(twistToggleButton))
		{
			twistDrive = oppositeBoolean(twistDrive);
		}
	}
	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive(2);
			intakeWheelControl(7,8);
			liftControl();
			Wait(0.005);				// wait for a motor update time
		}
	}

};

START_ROBOT_CLASS(Robot);
