#include "control.h"

int leftSpeed, rightSpeed; //�������ٶ�

int Target_Speed = 0;  //Ŀ���ٶ�
float Med_Angle = -10; //Ŀ��Ƕ�
int Target_Turn = 0  ; //ת��Ƕ�

/**
 * @brief ����ֵ����
 * @param  ����ֵ
 * @retval  |p|
 **/
int myAbs(int p)
{
	int q;
	q = p > 0 ? p : (-p);
	return q;
}

/**
 * @brief �������˲�
 * @param  ����ֵ
 * @retval  �˲�ֵ
 **/
float Kalman_filtering(float AOA_angle)
{
	static float Kalman_KA,Kalman_XA,Kalman_ZA,Kalman_ESTA=5,Kalman_MEAA=3;
	
	Kalman_MEAA=myAbs(Kalman_ZA-AOA_angle);
	Kalman_KA=Kalman_ESTA/(Kalman_ESTA+Kalman_MEAA);
	Kalman_XA=Kalman_XA+Kalman_KA*(AOA_angle-Kalman_XA);
	
	Kalman_ZA=AOA_angle;//����,Ϊ��һ���˲���׼��
	
	return Kalman_XA;
}

/**
 * @brief ֱ������λ��ʽ��
 * @param  �����Ƕ� ��ʵ�Ƕ� ��ʵ���ٶ�
 * @retval  ֱ�������
 **/
int verticalPid(float Med_Angle, float Now_Angle, float gyro_y)
{
	int PWM_OUT;
	PWM_OUT = Vertical_Kp * (Now_Angle - Med_Angle) + Vertical_Kd * (gyro_y);
	return PWM_OUT;
}

/**
 * @brief �ٶȻ���λ��ʽ��
 * @param  �����ٶ� ��ʵ�ٶ�
 * @retval  �ٶȻ����
 **/
int velocityPid(int Target_Speed, int Now_Speed)
{
	static int Speed_Sum,		  //�ٶȻ���
		Speed_Error,			  //�ٶ�ƫ��
		Speed_Error_Lowpass,	  //�ٶȵ�ͨ�˲�ֵ
		Speed_Error_Lowpass_Last, //�ϴ��ٶȵ�ͨ�˲�ֵ
		Speed_Pwm_Out;			  //�ٶȻ����

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
 * @brief ת��
 * @param  Ŀ��Ƕ� ��ǰ�Ƕ�
 * @retval  PID���
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

	Velocity_out = velocityPid(Target_Speed, leftSpeed - rightSpeed);	//�ٶȻ�
	Vertical_out = verticalPid(Velocity_out + Med_Angle, Pitch, gyroy); //ֱ����
	Turn_out = turnPid(Target_Turn,gyroz);								//ת��

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
