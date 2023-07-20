#include "control.h"

/*�ȶ���־λ*/
uint8_t stableFlag = 0;
/*�ȶ�����ֵ*/
int count = 0;
/*�������� ��һ���˶�������ÿ�� 5ms ȡ��*/
uint32_t TimeCnt = 0;
/*��������ڣ���λ��������ϵ��*/
float Basic_Amplitude = 150,Data_Amplitude,Data_Period,Data_Phase,Data_Gama;
/*���X�����Y����λ�ʱ�����*/
float Amplitude_x=0,Amplitude_y=0,Phase=PI/2,Alpha;  
/*X����Ŀ��ֵ��Y����Ŀ��ֵ*/
float Target_X,Target_Y;
/*X�����������ֵ��Y�����������ֵ*/
int Motor_X,Motor_Y;
/*�����������ĵľ��� ��Ӧ�ذ��ϵĻ���*/
float Measure_X,Measure_Y;


/**
 * @brief ����ֵ����
 * @param  X
 * @retval |X|
 **/
int myAbs(int x)
{
	if (x >= 0)
	{
		return x;
	}
	else
	{
		return -x;
	}
}

/**
 * @brief �ж��������Ƿ��ȶ�
 * @param  ���ֵ
 * @retval  None
 **/
void is_stable(uint8_t standard)
{
	if ((myAbs(pitch) < standard)  && (myAbs(roll) < standard))
	{
		if(count > 10)
		{
			stableFlag = 1;
		}
		count++;
	}
	else
	{
		count = 0;
	}
	
}

/**
 * @brief X����PID
 * @param  Ŀ����� ��������
 * @retval  PID���
 **/
int Position_PID_X (float value,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=value-Target;                                    //����ƫ��
	 Integral_bias+=Bias;	                               //���ƫ��Ļ���
	 Pwm=xKp*Bias+                                 //PID������������
	     xKi*Integral_bias+                        //PID������������
	     xKd*(Bias-Last_Bias);                     //PID������΢���� 
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}

/**
 * @brief Y����PID
 * @param  Ŀ����� ��������
 * @retval  PID���
 **/
int Position_PID_Y (float value,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=value-Target;                                    //����ƫ��
	 Integral_bias+=Bias;	                               //���ƫ��Ļ���
	 Pwm=yKp*Bias+                                 //PID������������
	     yKi*Integral_bias+                        //PID������������
	     yKd*(Bias-Last_Bias);                     //PID������΢���� 
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}

/**
 * @brief ��ò���ֵ
 * @param  None
 * @retval  None
 **/
void getParameter(void)
{
	/*����ģʽ*/
	if(modeFlag == 0)
	{
		Data_Amplitude = Basic_Amplitude;//��ʼ���
		Data_Phase= 0;      //ֱ���˶���λ��Ϊ0
		Data_Gama = 0;
	}
	/*�ȶ�ģʽ*/
	else if(modeFlag == 1)
	{
		Data_Amplitude = 0;
		Data_Phase= 0;      //ֱ���˶���λ��Ϊ0
		Data_Gama = 0;
	}
	/*��Բģʽ*/
	else if(modeFlag == 2)
	{
		
	}
	/*����ģʽ*/
	else if(modeFlag == 3)
	{
		
	}
	
	Amplitude_x=Data_Amplitude * sin(Data_Gama) * COEFFICIENT;//���X
	Amplitude_y=Data_Amplitude * cos(Data_Gama) * COEFFICIENT;//���Y
	Phase=Data_Phase;  //��λ��
	
}

/**
 * @brief �߽��޷�
 * @param None 
 * @retval Խ�緵��0
 **/
uint8_t isBounds(void)
{
	if ((myAbs(pitch) > 15)  || (myAbs(roll) > 15))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief �����ƺ���
 * @param  None
 * @retval  None
 **/
void mainControl(void)
{
	TimeCnt += 5;
	/*��ȡ����*/
	getParameter();
	/*�������ڴ���Ϊ���Ǻ���2������*/
	Alpha=(float)TimeCnt / PERIOD * 2 * PI;  
	/*X����Ŀ�����*/
	Target_X=Amplitude_x * sin(Alpha);      
	/*Y����Ŀ�����*/
	Target_Y=Amplitude_y * sin(Alpha+Phase);
	/*ͨ���ǶȻ�������������ĵľ���*/
	Measure_X=(float)tan((pitch- START_ROLL)/180 * 2 * PI) * HEIGHT;
	Measure_Y=(float)tan((roll- START_PITCH)/180 * 2 * PI) * HEIGHT;
	/*PID����*/
	Motor_X=Position_PID_X(Measure_X,Target_X);
	Motor_Y=Position_PID_Y(Measure_Y,Target_Y);
	/*�޷�*/
	Limit(&Motor_X,&Motor_Y);
	/*���*/
	if(isBounds() == 1)
	{
		Load(Motor_X,Motor_Y);
	}
	else
	{
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0);
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, 0);
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, 0);
		Timer_A_setCompareValue(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);
	}
}