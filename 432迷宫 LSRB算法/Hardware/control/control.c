#include "control.h"

int speedOut;		  //速度环输出
int speedSum;		  //两边速度总和
int pathLenth;		  //路程
int targtSpeed = 5;	  //默认速度是5
float startYaw;		  //转向前角度
float targetYaw;	  //目标转向角度
int yawPidOut;		  //转向环输出
int myTime;			  //定时器2.5ms计数一次
uint8_t moveMode = 0; //定义运动模式 默认直行
uint8_t startCaculate = 0;//最短路径是否计算完成
uint8_t turnRightGoLineTime = 45;//右转前直行距离
uint8_t turnLeftGoLineTime = 50;//左转前直行距离
uint8_t turnGolineBios = 0;
char mazeArray[100] = {'\0'}; //过去时记住的路线
int roadIndex = 0;		   //路线索引

/**
 * @brief 绝对值
 * @param  目标数
 * @retval  目标数取绝对值
 **/
int MyAbs(int p)
{
	return (p > 0 ? p : (-p));
}

/**
 * @brief 速度环(位置式)
 * @param  当前速度 目标速度
 * @retval  速度环输出
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
 * @brief 转向环
 * @param  当前角度 目标角度
 * @retval  速度环输出
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
 * @brief 直行
 * @param  None
 * @retval  None
 **/
void goLine(void)
{
	/*偏移量*/
	uint8_t offset = 4;

	speedOut = velocityPid(speedSum, targtSpeed);
	Limit(&speedOut, &speedOut);

	/*大偏差*/
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
	/*小偏差*/
	else if (sensor[2] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	else if (sensor[4] == 1)
	{
		Load(speedOut + offset, speedOut - offset);
	}
	/*直行*/
	else
	{
		Load(speedOut, speedOut);
	}
	
}

/**
 * @brief 判断岔路口
 * @param  None
 * @retval 0：直线
 * @retval 1：左
 * @retval 2：右
 * @retval 8: 右T
 * @retval 5:死胡同
 * @retval 6:终点
 **/
uint8_t judgeCrossing(void)
{
	/*终点*/
	if ((sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 0 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 1) || (sensor[6] == 1 && sensor[5] == 1 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 0 && sensor[7] == 1))
	{
		return 6;
	}
	/*左*/
	else if(sensor[6] == 1)
	{
		return 1;
	}
	/*右*/
	else if (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 0 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1)
	{
		return 2;
	}
	/*掉头*/
	else if (sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 0 && sensor[2] == 0 && sensor[1] == 0 && sensor[0] == 0 && sensor[7] == 0)
	{
		return 5;
	}
	/*右T*/
	else if((sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 0 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0)
		  ||(sensor[6] == 0 && sensor[5] == 0 && sensor[4] == 1 && sensor[3] == 1 && sensor[2] == 1 && sensor[1] == 1 && sensor[0] == 1 && sensor[7] == 0))
	{
		return 8;
	}
	/*直*/
	else
	{
		return 0;
	}
}

#if isYaw
/**
 * @brief 陀螺仪转向
 * @param  方向
 * @retval  None
 **/
void carTurn(uint8_t turnMode)
{
	static uint8_t flag = 0;
	if (turnMode == 1)
	{
		switch (flag)
		{
		/*先读取起始yaw角*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*转向*/
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
		/*先读取起始yaw角*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*转向*/
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
		/*先读取起始yaw角*/
		case 0:
		{
			startYaw = yaw;
			flag = 1;
		}
		break;
		/*转向*/
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
 * @brief 脉冲PID
 * @param  当前脉冲 目标脉冲
 * @retval  脉冲PID输出
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
 * @brief 重新排列数组
 * @param  地图数组 数组长度 下标
 * @retval  None
 **/
void reArrange(char* array, int length, int index) 
{
    char* newArray = (char*)malloc(length * sizeof(char));
	// 将原数组复制到新数组
	memcpy(newArray, array, length * sizeof(char));
    for(int k = index;k < length - 2;k++)
	{
		newArray[k] = newArray[k+2];
	}
	newArray[length - 1] = '\0';
	newArray[length - 2] = '\0';
    // 将新数组复制回原数组
    memcpy(array, newArray, length * sizeof(char));
    free(newArray);
}

/**
 * @brief 计数最短路径
 * @param  地图数组 数组长度
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

int my01sTime;	  // 0.1s计数
int lockFlag = 1; //自锁

/**
 * @brief 时间锁(防止一个路口多次识别）
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
 * @brief 主控制函数
 * @param  None
 * @retval  None
 **/
void mainControl(void)
{
	int pwmout;
	/*直行(默认)*/
	if (moveMode == 0 || moveMode == 8)
	{
		myTime = 0;
		pathLenth = 0;
		goLine();
		moveMode = judgeCrossing();
		changeLockFlag(15);
		/*左*/
		if (moveMode == 1 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'L';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*右*/
		else if (moveMode == 2 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'R';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*掉头*/
		else if (moveMode == 5 && lockFlag == 1)
		{
			mazeArray[roadIndex++] = 'B';
			my01sTime = 0;
			lockFlag = 0;
		}
		/*路口直行，这里针对右T*/
		else if (moveMode == 8 && lockFlag == 1)
		{
			LED_RED_On();
			mazeArray[roadIndex++] = 'S';
			my01sTime = 0;
			lockFlag = 0;
		}
	}

	/*左转(优先)*/
	if (moveMode == 1)
	{
		/*往前走一点*/
		if (myTime < turnLeftGoLineTime)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*右转*/
	if (moveMode == 2)
	{
		/*往前走一点*/
		if (myTime < turnRightGoLineTime)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*掉头*/
	if (moveMode == 5)
	{
		/*往前走一点*/
		if (myTime < 60)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*掉头*/
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
	/*到达终点*/
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

char tempMoveMode; //存放记忆值
uint8_t backMoveMode = 5;//默认先掉头

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
	/*直行*/
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

	/*左转(优先)*/
	if (backMoveMode == 1)
	{
		/*往前走一点*/
		if (myTime < turnLeftGoLineTime - turnGolineBios)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*右转*/
	if (backMoveMode == 2)
	{
		/*往前走一点*/
		if (myTime < turnRightGoLineTime - turnGolineBios)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*掉头*/
	if (backMoveMode == 5)
	{
		/*往前走一点*/
		if (myTime < 60)
		{
			myTime++;
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*掉头*/
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
	/*回到起点*/
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