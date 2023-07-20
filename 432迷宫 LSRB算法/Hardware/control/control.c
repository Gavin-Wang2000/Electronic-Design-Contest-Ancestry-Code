#include "control.h"

int speedOut;		  //�ٶȻ����
int speedSum;		  //�����ٶ��ܺ�
int pathLenth;		  //·��
int targtSpeed = 5;	  //Ĭ���ٶ���5
float startYaw;		  //ת��ǰ�Ƕ�
float targetYaw;	  //Ŀ��ת��Ƕ�
int yawPidOut;		  //ת�����
int myTime;			  //��ʱ��2.5ms����һ��
uint8_t moveMode = 0; //�����˶�ģʽ Ĭ��ֱ��
uint8_t startCaculate = 0;//���·���Ƿ�������
uint8_t turnRightGoLineTime = 45;//��תǰֱ�о���
uint8_t turnLeftGoLineTime = 50;//��תǰֱ�о���
uint8_t turnGolineBios = 0;
char mazeArray[100] = {'\0'}; //��ȥʱ��ס��·��
int roadIndex = 0;		   //·������

/**
 * @brief ����ֵ
 * @param  Ŀ����
 * @retval  Ŀ����ȡ����ֵ
 **/
int MyAbs(int p)
{
	return (p > 0 ? p : (-p));
}

/**
 * @brief �ٶȻ�(λ��ʽ)
 * @param  ��ǰ�ٶ� Ŀ���ٶ�
 * @retval  �ٶȻ����
 **/
int velocityPid(int nowSpeed, int targetSpeed)
{
	static int PWM_out, error;
	static int last_error = 0, error_int = 0;
	static float filtError;
	float a = 0.3;

	error = nowSpeed - targetSpeed;
	filtError = a * error + (1 - a) * last_error;

	error_int += error;
	error_int = error_int > 3000 ? 3000 : (error_int < (-3000) ? (-3000) : error_int);
	last_error = error;

	PWM_out = VeloctityKp * filtError + VeloctityKi * error_int + VeloctityKd * (error - last_error);

	return PWM_out;
}

/**
 * @brief ת��
 * @param  ��ǰ�Ƕ� Ŀ��Ƕ�
 * @retval  �ٶȻ����
 **/
int yawPid(float nowYaw, float targetYaw)
{
	static float PWM_out, error;
	static float last_error = 0, error_int = 0;

	error = nowYaw - targetYaw;

	if (error > 180)
		error -= 360;
	if (error < -180)
		error += 360;

	PWM_out = yawKp * error + yawKi * error_int + yawKd * (error - last_error);

	last_error = error;
	error_int += error;

	return PWM_out;
}

/**
 * @brief ֱ��
 * @param  None
 * @retval  None
 **/
void goLine(void)
{
	/*ƫ����*/
	uint8_t offset = 4;

	speedOut = velocityPid(speedSum, targtSpeed);
	Limit(&speedOut, &speedOut);

	/*��ƫ��*/
	if(sensor[5] == 1)
	{
		Load(speedOut + offset, speedOut - offset);
	}
	else if(sensor[1] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	else if(sensor[0] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	/*Сƫ��*/
	else if (sensor[2] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	else if (sensor[4] == 1)
	{
		Load(speedOut + offset, speedOut - offset);
	}
	/*ֱ��*/
	else
	{
		Load(speedOut, speedOut);
	}
	
}

/**
 * @brief �жϲ�·��
 * @param  None
 * @retval 0��ֱ��
 * @retval 1����
 * @retval 2����
 * @retval 8: ��T
 * @retval 5:����ͬ
 * @retval 6:�յ�
 **/
uint8_t judgeCrossing(void)
{
	/*�յ�*/
	if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 1))
	{
		return 6;
	}
	/*��*/
	else if(sensor[6] == 1)
	{
		return 1;
	}
	/*��*/
	else if (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1)
	{
		return 2;
	}
	/*��ͷ*/
	else if (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 0 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0 && sensor[7] == 0)
	{
		return 5;
	}
	/*��T*/
	else if((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0)
		  ||(sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0))
	{
		return 8;
	}
	/*ֱ*/
	else
	{
		return 0;
	}
}

#if isYaw
/**
 * @brief ������ת��
 * @param  ����
 * @retval  None
 **/
void carTurn(uint8_t turnMode)
{
	static uint8_t flag = 0;
	if (turnMode == 1)
	{
		switch (flag)
		{
		/*�ȶ�ȡ��ʼyaw��*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*ת��*/
		case 1:
		{
			targetYaw = startYaw + 90;
			yawPidOut = yawPid(yaw, targetYaw);
			Load(yawPidOut, -yawPidOut);
			flag = 2;
		}
		break;
		}
	}
	if (turnMode == 0)
	{
		switch (flag)
		{
		/*�ȶ�ȡ��ʼyaw��*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*ת��*/
		case 1:
		{
			targetYaw = startYaw - 90;
			yawPidOut = yawPid(yaw, targetYaw);
			Load(yawPidOut, -yawPidOut);
		}
		}
	}
	if (turnMode == 2)
	{
		switch (flag)
		{
		/*�ȶ�ȡ��ʼyaw��*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*ת��*/
		case 1:
		{
			targetYaw = startYaw - 180;
			yawPidOut = yawPid(yaw, targetYaw) * 2 / 3;
			Load(yawPidOut, -yawPidOut);
		}
		}
	}
}
#else

/**
 * @brief ����PID
 * @param  ��ǰ���� Ŀ������
 * @retval  ����PID���
 **/
int turnPID(int currentPulse, int targetPulse)
{
	static float PWM_out, error;
	static float last_error = 0, error_int = 0;

	error = currentPulse - targetPulse;

	PWM_out = turnKp * error + turnKd * (error - last_error);

	last_error = error;
	error_int += error;

	return PWM_out;
}

#endif

/**
 * @brief ������������
 * @param  ��ͼ���� ���鳤�� �±�
 * @retval  None
 **/
void reArrange(char* array, int length, int index) 
{
    char* newArray = (char*)malloc(length * sizeof(char));
	// ��ԭ���鸴�Ƶ�������
	memcpy(newArray, array, length * sizeof(char));
    for(int k = index;k < length - 2;k++)
	{
		newArray[k] = newArray[k+2];
	}
	newArray[length - 1] = '\0';
	newArray[length - 2] = '\0';
    // �������鸴�ƻ�ԭ����
    memcpy(array, newArray, length * sizeof(char));
    free(newArray);
}

/**
 * @brief �������·��
 * @param  ��ͼ���� ���鳤��
 * @retval  None
 **/
void caculateShortPath(char MAZE_ARRAY[], int SIZE_OF_ARRAY)
{
	/*ONCE THE ROBOT COMPLETES THE MAZE THE FINAL SHORTEST PATH CALCULATED
  IS STORED IN THE ROBOT MEMORY.THIS SHORTEST PATH IS USED TO COMPLETE
  THE SAME MAZE IN SHORTEST AMOUNT OF TIME.(L :LEFT, R:RIGHT, B:BACK,S:STRAIGHT)
  BELOW ARE THE FEW SUBSTITUTIONS TO CONVERT FULL MAZE PATH TO ITS 
  SHORTEST PATH:
  LBL = S
  LBR = B
  LBS = R
  RBL = B
  SBL = R
  SBS = B
  LBL = S */
	char ACTION;
//	isCaculateOk = 0;

	for (int i = 1; i <= SIZE_OF_ARRAY - 2; i++)
	{
		ACTION = MAZE_ARRAY[i];

		if (ACTION == 'B')
		{
			if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'R')
			{
				MAZE_ARRAY[i] = 'B';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}
			if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'S')
			{
				MAZE_ARRAY[i] = 'R';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}
			if (MAZE_ARRAY[i - 1] == 'R' && MAZE_ARRAY[i + 1] == 'L')
			{
				MAZE_ARRAY[i] = 'B';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}

			if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'L')
			{
				MAZE_ARRAY[i] = 'R';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}

			if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'S')
			{
				MAZE_ARRAY[i] = 'B';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}

			if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'L')
			{
				MAZE_ARRAY[i] = 'S';
				MAZE_ARRAY[i - 1] = MAZE_ARRAY[i];
				reArrange(MAZE_ARRAY, SIZE_OF_ARRAY, i);
			}
			i = 1;
		}
	}
//	isCaculateOk = 1;
}

int my01sTime;	  // 0.1s����
int lockFlag = 1; //����

/**
 * @brief ʱ����(��ֹһ��·�ڶ��ʶ��
 * @param  None
 * @retval  None
 **/
void changeLockFlag(uint8_t lockTime)
{
	my01sTime++;
	if (my01sTime >= lockTime)
	{
		lockFlag = 1;
	}
}

/**
 * @brief �����ƺ���
 * @param  None
 * @retval  None
 **/
void mainControl(void)
{
	int pwmout;
	/*ֱ��(Ĭ��)*/
	if (moveMode == 0 || moveMode == 8)
	{
		myTime = 0;
		pathLenth = 0;
		goLine();
		moveMode = judgeCrossing();
		changeLockFlag(15);
		/*��*/
		if (moveMode == 1 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'L';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*��*/
		else if (moveMode == 2 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'R';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*��ͷ*/
		else if (moveMode == 5 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'B';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*·��ֱ�У����������T*/
		else if (moveMode == 8 && lockFlag == 1)
		{
			LED_RED_On();
			mazeArray[roadIndex++] = 'S';
			my01sTime = 0;
			lockFlag = 0;
		}
	}

	/*��ת(����)*/
	if (moveMode == 1)
	{
		/*��ǰ��һ��*/
		if (myTime < turnLeftGoLineTime)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(30, -30);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(25, -25);
		}
		else
		{
			moveMode = 0;
		}
	}
	/*��ת*/
	if (moveMode == 2)
	{
		/*��ǰ��һ��*/
		if (myTime < turnRightGoLineTime)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(-30, 30);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(-25, 25);
		}
		else
		{
			moveMode = 0;
		}
	}
	/*��ͷ*/
	if (moveMode == 5)
	{
		/*��ǰ��һ��*/
		if (myTime < 60)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*��ͷ*/
		else if (pathLenth < 800)
		{
			Load(30,-30);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(25,-25);
		}
		else
		{
			moveMode = 0;
		}
	}
	/*�����յ�*/
	if (moveMode == 6)
	{
		if(myTime < 20)
		{
			myTime ++;
			Load(0,0);
			buzzer_on();
		}
		else
		{
			buzzer_off();
			lockFlag = 1;
			startCaculate = 1;
		}
	}
}

char tempMoveMode; //��ż���ֵ
uint8_t backMoveMode = 5;//Ĭ���ȵ�ͷ

void Back_mainControl(void)
{
	int pwmout;
	if(roadIndex > 15)
	{
		turnGolineBios = 20;
	}
	else if(roadIndex > 25)
	{
		turnGolineBios = 35;
	}
	/*ֱ��*/
	if (backMoveMode == 0)
	{
		myTime = 0;
		pathLenth = 0;
		goLine();
		moveMode = judgeCrossing();
		changeLockFlag(15);
		if(moveMode != 0 && moveMode != 6 && lockFlag == 1)
		{
			if(roadIndex > 0)
			{
				roadIndex --;
			}
			tempMoveMode = mazeArray[roadIndex];
			if (tempMoveMode == 'L')
			{
				backMoveMode = 2;
				my01sTime = 0;
				lockFlag = 0;
			}
			else if (tempMoveMode == 'R')
			{
				backMoveMode = 1;
				my01sTime = 0;
				lockFlag = 0;
			}
			else if (tempMoveMode == 'S')
			{
				backMoveMode = 0;
				my01sTime = 0;
				lockFlag = 0;
			}
	    }
		else if(moveMode == 6)
		{
			backMoveMode = 6;
		}
	}

	/*��ת(����)*/
	if (backMoveMode == 1)
	{
		/*��ǰ��һ��*/
		if (myTime < turnLeftGoLineTime - turnGolineBios)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(25, -25);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(20, -20);
		}
		else
		{
			backMoveMode = 0;
			turnGolineBios = 0;
		}
	}
	/*��ת*/
	if (backMoveMode == 2)
	{
		/*��ǰ��һ��*/
		if (myTime < turnRightGoLineTime - turnGolineBios)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(-25, 25);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(-20, 20);
		}
		else
		{
			backMoveMode = 0;
			turnGolineBios = 0;
		}
	}
	/*��ͷ*/
	if (backMoveMode == 5)
	{
		/*��ǰ��һ��*/
		if (myTime < 60)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*��ͷ*/
		else if (pathLenth < 800)
		{
			Load(20,-20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(20,-20);
		}
		else
		{
			backMoveMode = 0;
		}
	}
	/*�ص����*/
	if (backMoveMode == 6)
	{
		if(myTime < 20)
		{
			myTime ++;
			Load(0, 0);
			buzzer_on();
		}
		else
		{
			buzzer_off();
		}
		
	}
}