#include "control.h"

int leftSpeed,rightSpeed;
int speedSum;
int targetSpeed = 25;//50
uint8_t stopFlag = 0;
uint8_t turnFlag = 0;
uint8_t lockFlag = 1;
int myCountTime;
int turnNum = 0;


/**
 * @brief 速度环(增量式)
 * @param  当前速度 目标速度
 * @retval  速度环输出
 **/
int velocityPid(int nowSpeed, int targetSpeed)
{
	static float last_err,prev_err;
	static float last_out;
	float d_out;
	float err;
	
	err = targetSpeed - nowSpeed;
	
	d_out = VeloctityKp*(err - last_err) + VeloctityKi*err + VeloctityKd*(err - 2*last_err + prev_err);
	last_out += d_out;
	
	prev_err = last_err;
	last_err = err;
	
	return last_out;
}

/**
 * @brief 循迹PID
 * @param  当前偏移量 目标偏移量
 * @retval  PID输出
 **/
int linePid(int nowOffset, int targetOffset)
{
	static float PWM_out, error;
	static float last_error = 0, error_int = 0;

	error = nowOffset - targetOffset;

	PWM_out = lineKp * error + lineKi * error_int + lineKd * (error - last_error);

	last_error = error;
	error_int += error;

	return PWM_out;
}

/**
 * @brief 循迹PID
 * @param  当前偏移量 目标偏移量
 * @retval  PID输出
 **/
int distancePid(int nowDistance, int targetDistance)
{
	static float PWM_out, error;
	static float last_error = 0, error_int = 0;

	error = nowDistance - targetDistance;

	PWM_out = distanceKp * error + distanceKi * error_int + distanceKd * (error - last_error);

	last_error = error;
	error_int += error;

	return PWM_out;
}

/**
 * @brief 时间锁(防止一个路口多次识别）
 * @param  None
 * @retval  None
 **/
void changeLockFlag(int lockTime)
{
	myCountTime++;
	if (myCountTime >= lockTime)
	{
		lockFlag = 1;
	}
}

/**
 * @brief 主控制函数
 * @param  无
 * @retval  无
 **/
void mainControl(void)
{
	int pwmOut;
	int linePwmOut;
	static uint8_t stateFlag = 0;
	static uint32_t myTime;
	
	changeLockFlag(100);
	
	/*记圈数*/
	if(stopFlag == 1 && lockFlag == 1)
	{
		turnNum ++;
		lockFlag = 0;
		myCountTime = 0;
	}
	/*第三问*/
#if question
	
	/*距离环*/
	if(distance < TargetDistance)
	{
		targetSpeed = 22;
	}
	else
	{
		targetSpeed = 25;
	}
	
	
	switch(stateFlag)
	{
		/*第一圈走外圈*/
		case 0:
		{
			pwmOut = velocityPid(speedSum,targetSpeed);
			linePwmOut = linePid(sensorOffset(),goLine);
			Limit(&pwmOut,&pwmOut);
			Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			/*停车*/
			if(turnNum == 2)
			{
				//stateFlag = 1;
			}
			
		}break;
		/*第二圈走内圈*/
		case 1:
		{
			if(myTime < 400)
			{
				myTime ++;
				pwmOut = velocityPid(speedSum,targetSpeed);
				linePwmOut = linePid(sensorOffset(),goInner);
				Limit(&pwmOut,&pwmOut);
				Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			}
			/*加速超车*/
			else if(myTime < 400)
			{
				pwmOut = velocityPid(speedSum,targetSpeed + 5);
				linePwmOut = linePid(sensorOffset(),goLine);
				Limit(&pwmOut,&pwmOut);
				Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			}
			else
			{
				pwmOut = velocityPid(speedSum,targetSpeed);
				linePwmOut = linePid(sensorOffset(),goLine);
				Limit(&pwmOut,&pwmOut);
				Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
				if(turnNum == 3)
				{
					stateFlag = 2;
					myTime = 0;
				}
			}
			
		}break;
		case 2:
		{
			if(myTime < 900)
			{
				myTime++;
				pwmOut = velocityPid(speedSum,targetSpeed - 8);
				linePwmOut = linePid(sensorOffset(),goLine);
				Limit(&pwmOut,&pwmOut);
				Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			}
			else
			{
				pwmOut = velocityPid(speedSum,targetSpeed);
				linePwmOut = linePid(sensorOffset(),goLine);
				Limit(&pwmOut,&pwmOut);
				Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
				/*停车*/
				if(zigbeeFlag != 0)
				{
					stateFlag = 3;
					myTime = 0;
				}
			}
		}break;
		case 3:
		{
			Load(0,0);
			if(myTime < 100)
			{
				myTime ++;
				buzzer_on();
			}
			else
			{
				buzzer_off();
				startFlag = 2;
			}
		}break;
	}
	/*第四问*/
#else
	targetSpeed = 50;
	switch(stateFlag)
	{
		/*第一圈走外圈*/
		case 0:
		{
			pwmOut = velocityPid(speedSum,targetSpeed);
			linePwmOut = linePid(sensorOffset(),goLine);
			Limit(&pwmOut,&pwmOut);
			Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			/*停车*/
			if(turnNum == 1)
			{
				stateFlag = 1;
			}
		}break;
		case 1:
		{
			Load(0,0);
			if(myTime < 300)
			{
				myTime ++;
			}
			else
			{
				myTime = 0;
				stateFlag = 2;
			}
		}break;
		case 2:
		{
			pwmOut = velocityPid(speedSum,targetSpeed);
			linePwmOut = linePid(sensorOffset(),goLine);
			Limit(&pwmOut,&pwmOut);
			Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			/*停车*/
			if(turnNum == 2)
			{
				stateFlag = 3;
			}
		}break;
		case 3:
		{
			Load(0,0);
			if(myTime < 100)
			{
				myTime ++;
				buzzer_on();
			}
			else
			{
				buzzer_off();
			}
		}break;
	}
	
#endif
	
}
