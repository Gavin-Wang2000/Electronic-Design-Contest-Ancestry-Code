#include "control.h"
/*
Sp_Flag ���涡�ֻ���ʮ��or ��OR�Ҷ� 1 2 3 4
Num ����ڼ��ν��������
*/
struct Main_date Road_date[50];
int Road_indexs;   //·��������ֻ������ȷ��·��
int Back_Flag = 0; //��ͷ��־λ�����ж�Ϊ��·ʱ����

int speedOut;		  //�ٶȻ����
int speedSum;		  //�����ٶ��ܺ�
int pathLenth;		  //·��
int targtSpeed = 5;	  //Ĭ���ٶ���5
float startYaw;		  //ת��ǰ�Ƕ�
float targetYaw;	  //Ŀ��ת��Ƕ�
int yawPidOut;		  //ת�����
int myTime;			  //��ʱ��2.5ms����һ��
uint8_t moveMode = 0; //�����˶�ģʽ Ĭ��ֱ��

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
	uint8_t offset = 3;

	speedOut = velocityPid(speedSum, targtSpeed);
	Limit(&speedOut, &speedOut);

	/*Сƫ��*/
	if (sensor[2] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	else if (sensor[4] == 1)
	{
		Load(speedOut + offset, speedOut - offset);
	}

	/*��ƫ��*/
	else if (sensor[1] == 1)
	{
		Load(speedOut - 1.5 * offset, speedOut + 1.5 * offset);
	}
	else if (sensor[5] == 1)
	{
		Load(speedOut + 1.5 * offset, speedOut - 1.5 * offset);
	}

	/*��ƫ��*/
	else if (sensor[0] == 1)
	{
		Load(speedOut - 2 * offset, speedOut + 2 * offset);
	}
	else if (sensor[6] == 1)
	{
		Load(speedOut + 2 * offset, speedOut - 2 * offset);
	}

	/*ֱ��*/
	else
	{
		Load(speedOut, speedOut);
	}
}
/**
 * @brief �ж�T·��
 * @param  None
 * @retval  None
 **/
void judge_jun(void)
{
	/*�ڶ��ν���*/
	if (Back_Flag && Road_date[Road_indexs].Nums == 1)
	{
		Road_date[Road_indexs].dir = 1;
		Road_date[Road_indexs].Sp_Flag = 2;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	/*�����ν���*/
	if (Back_Flag && Road_date[Road_indexs].Nums == 2)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 0;
		Road_date[Road_indexs].Nums = 0;
		Back_Flag = 0;
		Road_indexs--;
	}
	/*��һ�ν���*/
	else
	{
		++Road_indexs;
		Road_date[Road_indexs].dir = 2;
		Road_date[Road_indexs].Sp_Flag = 2;
		Road_date[Road_indexs].Nums++;
	}
}
/**
 * @brief �ж���T·��
 * @param  None
 * @retval  None
 **/
void judge_Leftjun(void)
{
	if (Back_Flag && Road_date[Road_indexs].Nums == 1)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 3;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	else if (Back_Flag && Road_date[Road_indexs].Nums == 2)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 0;
		Road_date[Road_indexs].Nums = 0;
		Road_indexs--;
		Back_Flag = 0;
	}
	else
	{
		LED_B_On();
		++Road_indexs;
		Road_date[Road_indexs].dir = 2;
		Road_date[Road_indexs].Sp_Flag = 3;
		Road_date[Road_indexs].Nums++;
	}
}

/**
 * @brief �ж���T·��
 * @param  None
 * @retval  None
 **/
void judge_Rightjun(void)
{
	if (Back_Flag && Road_date[Road_indexs].Nums == 1)
	{
		Road_date[Road_indexs].dir = 1;
		Road_date[Road_indexs].Sp_Flag = 4;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	else if (Back_Flag && Road_date[Road_indexs].Nums == 2)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 0;
		Road_date[Road_indexs].Nums = 0;
		Road_indexs--;
		Back_Flag = 0;
	}
	else
	{
		++Road_indexs;
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 4;
		Road_date[Road_indexs].Nums++;
	}
}

/**
 * @brief �ж�ʮ��·��
 * @param  None
 * @retval  None
 **/
void judge_Cross(void)
{
	if (Back_Flag && Road_date[Road_indexs].Nums == 1 && Road_date[Road_indexs].Sp_Flag == 1)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 1;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	else if (Back_Flag && Road_date[Road_indexs].Nums == 2 && Road_date[Road_indexs].Sp_Flag == 1)
	{
		Road_date[Road_indexs].dir = 1;
		Road_date[Road_indexs].Sp_Flag = 1;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	else if (Back_Flag && Road_date[Road_indexs].Nums == 3 && Road_date[Road_indexs].Sp_Flag == 1)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 0;
		Road_date[Road_indexs].Nums = 0;
		Back_Flag = 0;
		Road_indexs--;
	}
	else
	{
		++Road_indexs;
		Road_date[Road_indexs].dir = 2;
		Road_date[Road_indexs].Sp_Flag = 1;
		Road_date[Road_indexs].Nums++;
	}
}

/**
 * @brief �ж��Ƿ��ͷ
 * @param  None
 * @retval  None
 **/
void judge_Back(void)
{
	Back_Flag = 1;
}

/**
 * @brief �жϲ�·��
 * @param  None
 * @retval 0��ֱ��
 * @retval 1����ֱ��
 * @retval 2����ֱ��
 * @retval 3: T��
 * @retval 4:ʮ��
 * @retval 5:����ͬ
 * @retval 6:�յ�
 * @retval 7:��T��
 * @retval 8:��T��
 **/
uint8_t judgeCrossing(void)
{
	if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 0 && sensor[0] == 0))
	{
		return 1;
	}
	else if ((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1) || (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1))
	{
		return 2;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 0))
	{

		return 3;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 0))
	{

		return 4;
	}
	else if (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 0 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0 && sensor[7] == 0)
	{

		return 5;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 1))
	{
		return 6;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 0 && sensor[0] == 0))
	{
		return 7;
	}
	else if ((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1) || (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1))
	{

		return 8;
	}
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

int my01sTime;	  // 0.1s����
int lockFlag = 1; //����

/**
 * @brief ÿ0.1�뿪��һ��
 * @param  None
 * @retval  None
 **/
void changeLockFlag(void)
{
	my01sTime++;
	if (my01sTime >= 60)
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
	/*ֱ��*/
	if (moveMode == 0 || moveMode == 8)
	{
		goLine();
		moveMode = judgeCrossing();
		myTime = 0;
		pathLenth = 0;
		changeLockFlag();
        if(moveMode == 3 && Road_date[Road_indexs].Sp_Flag != 3 && lockFlag != 0) //T
        {
            judge_jun();
             my01sTime = 0;
            lockFlag = 0;

        }
        else if((moveMode == 7 || (moveMode == 3 && Road_date[Road_indexs].Sp_Flag == 3))&& lockFlag != 0)//��T��
        {
            judge_Leftjun();
            my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 8 && Road_date[Road_indexs].Sp_Flag != 2 && lockFlag != 0)//��T��
        {
            judge_Rightjun();
             my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 4 && lockFlag != 0)//ʮ��
        {
            judge_Cross();
             my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 5 && lockFlag != 0)//��·
        {
            judge_Back();
            my01sTime = 0;
            lockFlag = 0;
        }
	}
	/*��ת(����)*/
	if (moveMode == 1 || moveMode == 3 || moveMode == 4 || moveMode == 7)
	{
		/*��ǰ��һ��*/
		if (myTime < 50)
		{
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(20, -20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(20, -20);
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
		if (myTime < 55)
		{
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(-20, 20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(-20, 20);
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
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*��ͷ*/
		else if (pathLenth < 800)
		{
			Load(-20, 20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(-15, 15);
		}
		else
		{
			moveMode = 0;
		}
	}
	/*�����յ�*/
	if (moveMode == 6)
	{
		Load(0, 0);
		brake();
	}
}

/**
 * @brief  ����ʱ�жϲ�·��
 * @param  None
 * @retval 0��ֱ��
 * @retval 1����ֱ��
 * @retval 2����ֱ��
 * @retval 6:�յ�
 **/
int Back_Lock_Flag = 0;
uint8_t Back_judgeCrossing(void)
{
	if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 0 && sensor[0] == 0))
	{
		return 1;
	}
	else if ((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1) || (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1))
	{
		return 2;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 0))
	{
		Back_Lock_Flag = 1;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 0))
	{
		Back_Lock_Flag = 1;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 1))
	{

		return 6;
	}
	else if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 0 && sensor[0] == 0))
	{
		Back_Lock_Flag = 1;
	}
	else if ((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1) || (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1))
	{
		Back_Lock_Flag = 1;
	}

	return 0;
}

uint8_t tempMoveMode; //��ż���ֵ
uint8_t backMoveMode;

void Back_mainControl(void)
{
	/*ֱ��*/
	if (backMoveMode == 0)
	{
		Back_Lock_Flag = 0;
		backMoveMode = Back_judgeCrossing();
		changeLockFlag();
		if (lockFlag != 0 && Back_Lock_Flag == 1 && Road_indexs >= 1)
		{
			backMoveMode = Road_date[Road_indexs].dir;
			Back_Lock_Flag = 0;
			lockFlag = 0;
			my01sTime = 0;
			LED_RED_On();
			Road_indexs--;
		}
		goLine();
		myTime = 0;
		pathLenth = 0;
	}
	/*��ת(����)*/
	else if (backMoveMode == 1)
	{
		/*��ǰ��һ��*/
		if (myTime < 70)
		{
			Load(15, 15);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			LED_RED_On();
			Load(20, -20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(20, -20);
		}
		else
		{
			backMoveMode = 0;
		}
	}
	/*��ת*/
	else if (backMoveMode == 2)
	{

		/*��ǰ��һ��*/
		if (myTime < 75)
		{
			Load(15, 15);
		}
		/*ת��*/
		else if (pathLenth < 400)
		{
			Load(-20, 20);
			pathLenth += (MyAbs(leftSpeed) + MyAbs(rightSpeed));
		}
		else if (sensor[3] == 0)
		{
			Load(-20, 20);
		}
		else
		{
			backMoveMode = 0;
		}
	}
	/*�����յ�*/
	else if (backMoveMode == 6)
	{
		Load(0, 0);
		brake();
	}
}