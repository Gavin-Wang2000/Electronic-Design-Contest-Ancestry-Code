#include "control.h"

/*稳定标志位*/
uint8_t stableFlag = 0;
/*稳定计数值*/
int count = 0;
/*采用周期 在一个运动周期内每隔 5ms 取点*/
uint32_t TimeCnt = 0;
/*振幅，周期，相位差，振幅比例系数*/
float Basic_Amplitude = 150,Data_Amplitude,Data_Period,Data_Phase,Data_Gama;
/*振幅X，振幅Y，相位差，时间变量*/
float Amplitude_x=0,Amplitude_y=0,Phase=PI/2,Alpha;  
/*X方向目标值，Y方向目标值*/
float Target_X,Target_Y;
/*X方向最终输出值，Y方向最终输出值*/
int Motor_X,Motor_Y;
/*激光点距离中心的距离 对应地板上的环数*/
float Measure_X,Measure_Y;


/**
 * @brief 绝对值函数
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
 * @brief 判断陀螺仪是否稳定
 * @param  误差值
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
 * @brief X方向PID
 * @param  目标距离 测量距离
 * @retval  PID输出
 **/
int Position_PID_X (float value,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=value-Target;                                    //计算偏差
	 Integral_bias+=Bias;	                               //求出偏差的积分
	 Pwm=xKp*Bias+                                 //PID控制器比例项
	     xKi*Integral_bias+                        //PID控制器积分项
	     xKd*(Bias-Last_Bias);                     //PID控制器微分项 
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}

/**
 * @brief Y方向PID
 * @param  目标距离 测量距离
 * @retval  PID输出
 **/
int Position_PID_Y (float value,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=value-Target;                                    //计算偏差
	 Integral_bias+=Bias;	                               //求出偏差的积分
	 Pwm=yKp*Bias+                                 //PID控制器比例项
	     yKi*Integral_bias+                        //PID控制器积分项
	     yKd*(Bias-Last_Bias);                     //PID控制器微分项 
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}

/**
 * @brief 获得参数值
 * @param  None
 * @retval  None
 **/
void getParameter(void)
{
	/*单摆模式*/
	if(modeFlag == 0)
	{
		Data_Amplitude = Basic_Amplitude;//初始振幅
		Data_Phase= 0;      //直线运动相位差为0
		Data_Gama = 0;
	}
	/*稳定模式*/
	else if(modeFlag == 1)
	{
		Data_Amplitude = 0;
		Data_Phase= 0;      //直线运动相位差为0
		Data_Gama = 0;
	}
	/*画圆模式*/
	else if(modeFlag == 2)
	{
		
	}
	/*自由模式*/
	else if(modeFlag == 3)
	{
		
	}
	
	Amplitude_x=Data_Amplitude * sin(Data_Gama) * COEFFICIENT;//振幅X
	Amplitude_y=Data_Amplitude * cos(Data_Gama) * COEFFICIENT;//振幅Y
	Phase=Data_Phase;  //相位差
	
}

/**
 * @brief 边界限幅
 * @param None 
 * @retval 越界返回0
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
 * @brief 主控制函数
 * @param  None
 * @retval  None
 **/
void mainControl(void)
{
	TimeCnt += 5;
	/*获取参数*/
	getParameter();
	/*单摆周期处理为三角函数2Π周期*/
	Alpha=(float)TimeCnt / PERIOD * 2 * PI;  
	/*X方向目标距离*/
	Target_X=Amplitude_x * sin(Alpha);      
	/*Y方向目标距离*/
	Target_Y=Amplitude_y * sin(Alpha+Phase);
	/*通过角度换算出激光点距中心的距离*/
	Measure_X=(float)tan((pitch- START_ROLL)/180 * 2 * PI) * HEIGHT;
	Measure_Y=(float)tan((roll- START_PITCH)/180 * 2 * PI) * HEIGHT;
	/*PID换算*/
	Motor_X=Position_PID_X(Measure_X,Target_X);
	Motor_Y=Position_PID_Y(Measure_Y,Target_Y);
	/*限幅*/
	Limit(&Motor_X,&Motor_Y);
	/*输出*/
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