#include "control.h"

int leftSpeed, rightSpeed; //左右轮速度

int Target_Speed = 0;  //目标速度
float Med_Angle = -10; //目标角度
int Target_Turn = 0  ; //转向角度

/**
 * @brief 绝对值函数
 * @param  处理值
 * @retval  |p|
 **/
int myAbs(int p)
{
	int q;
	q = p > 0 ? p : (-p);
	return q;
}

/**
 * @brief 卡尔曼滤波
 * @param  处理值
 * @retval  滤波值
 **/
float Kalman_filtering(float AOA_angle)
{
	static float Kalman_KA,Kalman_XA,Kalman_ZA,Kalman_ESTA=5,Kalman_MEAA=3;
	
	Kalman_MEAA=myAbs(Kalman_ZA-AOA_angle);
	Kalman_KA=Kalman_ESTA/(Kalman_ESTA+Kalman_MEAA);
	Kalman_XA=Kalman_XA+Kalman_KA*(AOA_angle-Kalman_XA);
	
	Kalman_ZA=AOA_angle;//更新,为下一次滤波做准备
	
	return Kalman_XA;
}

/**
 * @brief 直立环（位置式）
 * @param  期望角度 真实角度 真实角速度
 * @retval  直立环输出
 **/
int verticalPid(float Med_Angle, float Now_Angle, float gyro_y)
{
	int PWM_OUT;
	PWM_OUT = Vertical_Kp * (Now_Angle - Med_Angle) + Vertical_Kd * (gyro_y);
	return PWM_OUT;
}

/**
 * @brief 速度环（位置式）
 * @param  期望速度 真实速度
 * @retval  速度环输出
 **/
int velocityPid(int Target_Speed, int Now_Speed)
{
	static int Speed_Sum,		  //速度积分
		Speed_Error,			  //速度偏差
		Speed_Error_Lowpass,	  //速度低通滤波值
		Speed_Error_Lowpass_Last, //上次速度低通滤波值
		Speed_Pwm_Out;			  //速度环输出

	float a = 0.7;

	Speed_Error = Now_Speed - Target_Speed;

	Speed_Error_Lowpass = (1 - a) * Speed_Error + a * Speed_Error_Lowpass_Last;
	Speed_Error_Lowpass_Last = Speed_Error_Lowpass;

	Speed_Sum += Speed_Error_Lowpass;
	Speed_Sum = Speed_Sum > 10000 ? 10000 : (Speed_Sum < (-10000) ? (-10000) : Speed_Sum);

	Speed_Pwm_Out = Velocity_Kp * Speed_Error_Lowpass + Velocity_Ki * Speed_Sum;

	return Speed_Pwm_Out;
}

/**
 * @brief 转向环
 * @param  目标角度 当前角度
 * @retval  PID输出
 **/
int turnPid(int Target_gyro, int Now_gyro)
{
	int turn, bios;
	bios = Now_gyro - Target_gyro;
	turn = turnKp * bios;
	return turn;
}

void mainControl(void)
{
	int Vertical_out, Velocity_out, Turn_out;
	int leftPwmOut,rightPwmOut;

	Velocity_out = velocityPid(Target_Speed, leftSpeed - rightSpeed);	//速度环
	Vertical_out = verticalPid(Velocity_out + Med_Angle, Pitch, gyroy); //直立环
	Turn_out = turnPid(Target_Turn,gyroz);								//转向环

	leftPwmOut = Vertical_out + Turn_out;
	rightPwmOut = Vertical_out - Turn_out;

	Limit(&leftPwmOut, &rightPwmOut);

	if(myAbs(Pitch) < 70)
	{
		Load(leftPwmOut, rightPwmOut);
	}
	else
	{
		brake();
	}
}
