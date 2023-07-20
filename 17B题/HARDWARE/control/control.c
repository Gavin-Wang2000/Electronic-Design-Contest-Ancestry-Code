#include "control.h"

float xKp = XKP;
float xKi = XKI;
float xKd = XKD;

float yKp = YKP;
float yKi = YKI;
float yKd = YKD;

int zone1Tozone4Fory[200];
int zone4Tozone5Forx[200];
int zone1Tozone9Forx[400];
int zone1Tozone9Fory[400];

/**
 * @brief x方向pid
 * @param  当前x方向坐标 x目标坐标
 * @retval  pid输出
 **/
int xPid(int nowX,int targetX)
{
	static int PWM_out, error;
	static int last_error = 0,error_int = 0;
	static unsigned char index;

	error = nowX - targetX;
	
	if(abs(error) > Integraldead_zone)
	{
		index = 0;
	}
	else
	{
		index = 1;
		error_int += error;
	}

	PWM_out = xKp * error + xKi * error_int * index + xKd * (error - last_error);
	
	last_error = error;
	
	if(PWM_out > 300)
	{
		PWM_out = 300;
	}
	if(PWM_out < -300)
	{
		PWM_out = -300;
	}

	return PWM_out;
}

/**
 * @brief y方向pid
 * @param  当前y方向坐标 y目标坐标
 * @retval  pid输出
 **/
int yPid(int nowY,int targetY)
{
	static int PWM_out, error;
	static int last_error = 0,error_int = 0;
	static unsigned char index;

	error = nowY - targetY;
	
	if(abs(error) > Integraldead_zone)
	{
		index = 0;
	}
	else
	{
		index = 1;
		error_int += error;
	}

	PWM_out = yKp * error + yKi * error_int * index + yKd * (error - last_error);

	last_error = error;
	
	if(PWM_out > 300)
	{
		PWM_out = 300;
	}
	if(PWM_out < -300)
	{
		PWM_out = -300;
	}

	return PWM_out;
}

/**
 * @brief 主控制函数
 * @param  无
 * @retval  无
 **/
void mainControl(void)
{
	switch(ballMove.mode)
	{
		case 1:
		{
			questionOne();
		}break;
		case 2:
		{
			questionTwo();
		}break;
		case 3:
		{
			questionThree();
		}break;
		case 4:
		{
			questionFour();
		}
	}
	
}

/**
 * @brief 第一问
 * @param  无
 * @retval  无
 **/

void questionOne(void)
{
	float xOut;
	float yOut;
	
	xOut = servoxRight + xPid(ball.cx,zone2X);
	yOut = servoyRight + yPid(ball.cy,zone2Y);
	
	TIM_SetCompare1(TIM8,xOut);
	TIM_SetCompare2(TIM8,yOut);
}

/**
 * @brief 第二问
 * @param  无
 * @retval  无
 **/

void questionTwo(void)
{
	float xOut;
	float yOut;
	
	xOut = servoxRight + xPid(ball.cx,zone5X);
	yOut = servoyRight + yPid(ball.cy,zone5Y);
	
	TIM_SetCompare1(TIM8,xOut);
	TIM_SetCompare2(TIM8,yOut);
}


/**
 * @brief 第三问
 * @param  无
 * @retval  无
 **/

void questionThree(void)
{
	float xOut;
	float yOut;
	
	int targetX;
	int targetY;
	
	static uint8_t mode = 0;
	static uint16_t index = 0;
	
	/*按键执行下一步*/
	if(key[1].keyShortDown == 1)
	{
		mode++;
		key[1].keyShortDown = 0;
	}
	
	switch(mode)
	{
		/*停留在区域1*/
		case 0:
		{
			targetX = zone1X;
			targetY = zone1Y;
		}break;
		/*移动到区域4*/
		case 1:
		{
			targetX = zone1X;
			targetY = zone1Tozone4Fory[index++];
			if(index >= 200)
			{
				mode = 2;
				index = 0;
			}
		}break;
		/*停留在区域4*/
		case 2:
		{
			targetX = zone4X;
			targetY = zone4Y;
		}break;
		/*移动到区域5*/
		case 3:
		{
			targetX = zone4Tozone5Forx[index++];
			targetY = zone5Y;
			if(index >= 200)
			{
				mode = 4;
				index = 0;
			}
		}break;
		case 4:
		{
			targetX = zone5X;
			targetY = zone5Y;
		}break;
	}
	xOut = servoxRight + xPid(ball.cx,targetX);
	yOut = servoyRight + yPid(ball.cy,targetY);
	
	TIM_SetCompare1(TIM8,xOut);
	TIM_SetCompare2(TIM8,yOut);
}

/**
 * @brief 第三问
 * @param  无
 * @retval  无
 **/

void questionFour(void)
{
	float xOut;
	float yOut;
	
	int targetX;
	int targetY;
	
	static uint8_t mode = 0;
	static uint16_t index = 0;
	
	/*按键执行下一步*/
	if(key[1].keyShortDown == 1)
	{
		mode++;
		key[1].keyShortDown = 0;
	}
	
	switch(mode)
	{
		/*停留在区域1*/
		case 0:
		{
			targetX = zone1X;
			targetY = zone1Y;
		}break;
		/*移动到区域9*/
		case 1:
		{
			targetX = zone1Tozone9Forx[index++];
			targetY = zone1Tozone9Fory[index++];
			if(index >= 400)
			{
				mode = 2;
				index = 0;
			}
		}break;
		/*停留在区域9*/
		case 2:
		{
			targetX = zone9X;
			targetY = zone9Y;
		}break;
	}
	xOut = servoxRight + xPid(ball.cx,targetX);
	yOut = servoyRight + yPid(ball.cy,targetY);
	
	TIM_SetCompare1(TIM8,xOut);
	TIM_SetCompare2(TIM8,yOut);
}


int zone1Tozone4Fory[200] = 
{
	14,14,14,14,14,14,14,14,
	13,13,13,13,13,13,13,13,
	12,12,12,12,12,12,12,12,
	11,11,11,11,11,11,11,11,
	10,10,10,10,10,10,10,10,
	 9, 9, 9, 9, 9, 9, 9, 9,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 7, 7, 7, 7, 7, 7, 7, 7,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 5, 5, 5, 5, 5, 5, 5, 5,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 3, 3, 3, 3, 3, 3, 3, 3,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 1, 1, 1, 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0,
	-1,-1,-1,-1,-1,-1,-1,-1,
	-2,-2,-2,-2,-2,-2,-2,-2,
	-3,-3,-3,-3,-3,-3,-3,-3,
	-4,-4,-4,-4,-4,-4,-4,-4,
	-5,-5,-5,-5,-5,-5,-5,-5,
	-6,-6,-6,-6,-6,-6,-6,-6,
	-7,-7,-7,-7,-7,-7,-7,-7,
	-8,-8,-8,-8,-8,-8,-8,-8,
	-9,-9,-9,-9,-9,-9,-9,-9,
	-10,-10,-10,-10,-10,-10,-10,-10,
};

int zone4Tozone5Forx[200] =
{
	30,30,30,30,30,30,30,30,30,30,30,30,30,
	29,29,29,29,29,29,29,29,29,29,29,29,29,
	28,28,28,28,28,28,28,28,28,28,28,28,28,
	27,27,27,27,27,27,27,27,27,27,27,27,27,
	26,26,26,26,26,26,26,26,26,26,26,26,26,
	25,25,25,25,25,25,25,25,25,25,25,25,25,
	24,24,24,24,24,24,24,24,24,24,24,24,24,
	23,23,23,23,23,23,23,23,23,23,23,23,23,
	22,22,22,22,22,22,22,22,22,22,22,22,22,
	21,21,21,21,21,21,21,21,21,21,21,21,21,
	20,20,20,20,20,20,20,20,20,20,20,20,20,
	19,19,19,19,19,19,19,19,19,19,19,19,19,
	18,18,18,18,18,18,18,18,18,18,18,18,18,
	17,17,17,17,17,17,17,17,17,17,17,17,17,
	16,16,16,16,16,16,16,16,16,16,16,16,16,
	15,15,15,15,15,
};

int zone1Tozone9Forx[400]=
{
	29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,
	28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
	27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,
	26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,
	25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,
	24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
	23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
	22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
	21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
	20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
	19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
	18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
	17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
	16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
	15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
	13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
	11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
	10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
	 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

int zone1Tozone9Fory[400]=
{
	14,14,14,14,14,14,14,14,
	13,13,13,13,13,13,13,13,
	12,12,12,12,12,12,12,12,
	11,11,11,11,11,11,11,11,
	10,10,10,10,10,10,10,10,
	 9, 9, 9, 9, 9, 9, 9, 9,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 7, 7, 7, 7, 7, 7, 7, 7,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 5, 5, 5, 5, 5, 5, 5, 5,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 3, 3, 3, 3, 3, 3, 3, 3,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	-1,-1,-1,-1,-1,-1,-1,-1,
	-2,-2,-2,-2,-2,-2,-2,-2,
	-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
	-4,-4,-4,-4,-4,-4,-4,-4,
	-5,-5,-5,-5,-5,-5,-5,-5,
	-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,
	-7,-7,-7,-7,-7,-7,-7,-7,
	-8,-8,-8,-8,-8,-8,-8,-8,
	-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,
	-10,-10,-10,-10,-10,-10,-10,-10,
	-11,-11,-11,-11,-11,-11,-11,-11,
	-12,-12,-12,-12,-12,-12,-12,-12,
	-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,
	-14,-14,-14,-14,-14,-14,-14,-14,
	-15,-15,-15,-15,-15,-15,-15,-15,
	-16,-16,-16,-16,-16,-16,-16,-16,
	-17,-17,-17,-17,-17,-17,-17,-17,
	-18,-18,-18,-18,-18,-18,-18,-18,
	-19,-19,-19,-19,-19,-19,-19,-19,
	-20,-20,-20,-20,-20,-20,-20,-20,
	-21,-21,-21,-21,-21,-21,-21,-21,
	-22,-22,-22,-22,-22,-22,-22,-22,-22,-22,-22,-22,
	-23,-23,-23,-23,-23,-23,-23,-23,
	-24,-24,-24,-24,-24,-24,-24,-24,
	-25,-25,-25,-25,-25,-25,-25,-25,
	-26,-26,-26,-26,-26,-26,-26,-26,
	-27,-27,-27,-27,-27,-27,-27,-27,
	-28,-28,-28,-28,-28,-28,-28,-28,
	-29,-29,-29,-29,-29,-29,-29,-29,
	-30,-30,-30,-30,-30,-30,-30,-30,
	-30,-30,-30,-30,-30,-30,-30,-30,
};