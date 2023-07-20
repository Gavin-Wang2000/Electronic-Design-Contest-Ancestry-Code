#include "control.h"
/*
Sp_Flag 保存丁字还是十字or 左丁OR右丁 1 2 3 4
Num 保存第几次进入该数组
*/
struct Main_date Road_date[50];
int Road_indexs;   //路线索引，只保存正确的路径
int Back_Flag = 0; //掉头标志位，当判断为死路时触发

int speedOut;		  //速度环输出
int speedSum;		  //两边速度总和
int pathLenth;		  //路程
int targtSpeed = 5;	  //默认速度是5
float startYaw;		  //转向前角度
float targetYaw;	  //目标转向角度
int yawPidOut;		  //转向环输出
int myTime;			  //定时器2.5ms计数一次
uint8_t moveMode = 0; //定义运动模式 默认直行

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
	uint8_t offset = 3;

	speedOut = velocityPid(speedSum, targtSpeed);
	Limit(&speedOut, &speedOut);

	/*小偏差*/
	if (sensor[2] == 1)
	{
		Load(speedOut - offset, speedOut + offset);
	}
	else if (sensor[4] == 1)
	{
		Load(speedOut + offset, speedOut - offset);
	}

	/*中偏差*/
	else if (sensor[1] == 1)
	{
		Load(speedOut - 1.5 * offset, speedOut + 1.5 * offset);
	}
	else if (sensor[5] == 1)
	{
		Load(speedOut + 1.5 * offset, speedOut - 1.5 * offset);
	}

	/*大偏差*/
	else if (sensor[0] == 1)
	{
		Load(speedOut - 2 * offset, speedOut + 2 * offset);
	}
	else if (sensor[6] == 1)
	{
		Load(speedOut + 2 * offset, speedOut - 2 * offset);
	}

	/*直行*/
	else
	{
		Load(speedOut, speedOut);
	}
}
/**
 * @brief 判断T路口
 * @param  None
 * @retval  None
 **/
void judge_jun(void)
{
	/*第二次进入*/
	if (Back_Flag && Road_date[Road_indexs].Nums == 1)
	{
		Road_date[Road_indexs].dir = 1;
		Road_date[Road_indexs].Sp_Flag = 2;
		Road_date[Road_indexs].Nums++;
		Back_Flag = 0;
	}
	/*第三次进入*/
	if (Back_Flag && Road_date[Road_indexs].Nums == 2)
	{
		Road_date[Road_indexs].dir = 0;
		Road_date[Road_indexs].Sp_Flag = 0;
		Road_date[Road_indexs].Nums = 0;
		Back_Flag = 0;
		Road_indexs--;
	}
	/*第一次进入*/
	else
	{
		++Road_indexs;
		Road_date[Road_indexs].dir = 2;
		Road_date[Road_indexs].Sp_Flag = 2;
		Road_date[Road_indexs].Nums++;
	}
}
/**
 * @brief 判断左T路口
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
 * @brief 判断右T路口
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
 * @brief 判断十字路口
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
 * @brief 判断是否掉头
 * @param  None
 * @retval  None
 **/
void judge_Back(void)
{
	Back_Flag = 1;
}

/**
 * @brief 判断岔路口
 * @param  None
 * @retval 0：直线
 * @retval 1：左直角
 * @retval 2：右直角
 * @retval 3: T型
 * @retval 4:十字
 * @retval 5:死胡同
 * @retval 6:终点
 * @retval 7:左T字
 * @retval 8:右T字
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

int my01sTime;	  // 0.1s计数
int lockFlag = 1; //自锁

/**
 * @brief 每0.1秒开锁一次
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
 * @brief 主控制函数
 * @param  None
 * @retval  None
 **/
void mainControl(void)
{
	int pwmout;
	/*直行*/
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
        else if((moveMode == 7 || (moveMode == 3 && Road_date[Road_indexs].Sp_Flag == 3))&& lockFlag != 0)//左T字
        {
            judge_Leftjun();
            my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 8 && Road_date[Road_indexs].Sp_Flag != 2 && lockFlag != 0)//右T字
        {
            judge_Rightjun();
             my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 4 && lockFlag != 0)//十字
        {
            judge_Cross();
             my01sTime = 0;
            lockFlag = 0;
        }
        else if(moveMode == 5 && lockFlag != 0)//死路
        {
            judge_Back();
            my01sTime = 0;
            lockFlag = 0;
        }
	}
	/*左转(优先)*/
	if (moveMode == 1 || moveMode == 3 || moveMode == 4 || moveMode == 7)
	{
		/*往前走一点*/
		if (myTime < 50)
		{
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*右转*/
	if (moveMode == 2)
	{
		/*往前走一点*/
		if (myTime < 55)
		{
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*转弯*/
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
	/*掉头*/
	if (moveMode == 5)
	{
		/*往前走一点*/
		if (myTime < 60)
		{
			pwmout = velocityPid(speedSum, 6);
			Limit(&speedOut, &speedOut);
			Load(pwmout, pwmout);
		}
		/*掉头*/
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
	/*到达终点*/
	if (moveMode == 6)
	{
		Load(0, 0);
		brake();
	}
}

/**
 * @brief  返回时判断岔路口
 * @param  None
 * @retval 0：直线
 * @retval 1：左直角
 * @retval 2：右直角
 * @retval 6:终点
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

uint8_t tempMoveMode; //存放记忆值
uint8_t backMoveMode;

void Back_mainControl(void)
{
	/*直行*/
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
	/*左转(优先)*/
	else if (backMoveMode == 1)
	{
		/*往前走一点*/
		if (myTime < 70)
		{
			Load(15, 15);
		}
		/*转弯*/
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
	/*右转*/
	else if (backMoveMode == 2)
	{

		/*往前走一点*/
		if (myTime < 75)
		{
			Load(15, 15);
		}
		/*转弯*/
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
	/*到达终点*/
	else if (backMoveMode == 6)
	{
		Load(0, 0);
		brake();
	}
}