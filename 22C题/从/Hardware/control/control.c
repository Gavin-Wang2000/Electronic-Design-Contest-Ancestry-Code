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
 * @brief �ٶȻ�(����ʽ)
 * @param  ��ǰ�ٶ� Ŀ���ٶ�
 * @retval  �ٶȻ����
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
 * @brief ѭ��PID
 * @param  ��ǰƫ���� Ŀ��ƫ����
 * @retval  PID���
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
 * @brief ѭ��PID
 * @param  ��ǰƫ���� Ŀ��ƫ����
 * @retval  PID���
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
 * @brief ʱ����(��ֹһ��·�ڶ��ʶ��
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
 * @brief �����ƺ���
 * @param  ��
 * @retval  ��
 **/
void mainControl(void)
{
	int pwmOut;
	int linePwmOut;
	static uint8_t stateFlag = 0;
	static uint32_t myTime;
	
	changeLockFlag(100);
	
	/*��Ȧ��*/
	if(stopFlag == 1 && lockFlag == 1)
	{
		turnNum ++;
		lockFlag = 0;
		myCountTime = 0;
	}
	/*������*/
#if question
	
	/*���뻷*/
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
		/*��һȦ����Ȧ*/
		case 0:
		{
			pwmOut = velocityPid(speedSum,targetSpeed);
			linePwmOut = linePid(sensorOffset(),goLine);
			Limit(&pwmOut,&pwmOut);
			Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			/*ͣ��*/
			if(turnNum == 2)
			{
				//stateFlag = 1;
			}
			
		}break;
		/*�ڶ�Ȧ����Ȧ*/
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
			/*���ٳ���*/
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
				/*ͣ��*/
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
	/*������*/
#else
	targetSpeed = 50;
	switch(stateFlag)
	{
		/*��һȦ����Ȧ*/
		case 0:
		{
			pwmOut = velocityPid(speedSum,targetSpeed);
			linePwmOut = linePid(sensorOffset(),goLine);
			Limit(&pwmOut,&pwmOut);
			Load(pwmOut + linePwmOut,pwmOut - linePwmOut);
			/*ͣ��*/
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
			/*ͣ��*/
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
