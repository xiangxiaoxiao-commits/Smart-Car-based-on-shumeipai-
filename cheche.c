#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//定义引脚
//(左) 
int Left_motor_go = 28;       //左电机前进AIN2连接Raspberry的wiringPi编码28口
int Left_motor_back = 29;     //左电机后退AIN1连接Raspberry的wiringPi编码29口
//(右) 
int Right_motor_go = 24;      //右电机前进BIN2连接Raspberry的wiringPi编码24口
int Right_motor_back = 25;    //右电机后退BIN1连接Raspberry的wiringPi编码25口
//(控速) 
int Left_motor_pwm = 27;      //左电机控速PWMA连接Raspberry的wiringPi编码27口
int Right_motor_pwm = 23;     //右电机控速PWMB连接Raspberry的wiringPi编码23口
//(按键) 
int key = 10;                 //定义按键为Raspberry的wiringPi编码10口
//(回声与触发) 
int EchoPin = 30;             //定义回声脚为连接Raspberry的wiringPi编码30口
int TrigPin = 31;             //定义触发脚为连接Raspberry的wiringPi编码31口

float distance;
void car();
void MIDDLE();
void LEFT();
void RIGHT();

//循迹红外引脚定义
//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      9                 21                  7                   1
const int TrackSensorLeftPin1  =  9;   //定义左边第一个循迹红外传感器引脚为wiringPi编码9口（左1） 
const int TrackSensorLeftPin2  =  21;  //定义左边第二个循迹红外传感器引脚为wiringPi编码21口（左2） 
const int TrackSensorRightPin1 =  7;   //定义右边第一个循迹红外传感器引脚为wiringPi编码7口（右1） 
const int TrackSensorRightPin2 =  1;   //定义右边第二个循迹红外传感器引脚为wiringPi编码1口（右2） 

//定义各个循迹红外引脚采集的数据的变量
int TrackSensorLeftValue1;     //（左一） 
int TrackSensorLeftValue2;     //（左二） 
int TrackSensorRightValue1;    //（右一） 
int TrackSensorRightValue2;    //（右二）

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         小车前进
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void run(int l,int r)    //向前移动 
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);      //左电机前进使能
  digitalWrite(Left_motor_back, LOW);     //左电机后退禁止
  softPwmWrite(Left_motor_pwm,  l );      //左转速
  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  	  //右电机前进使能
  digitalWrite(Right_motor_back, LOW);    //右电机后退禁止
  softPwmWrite(Right_motor_pwm, r );      //右转速 
}

/**
* Function       brake
* @author        Danny
* @date          2017.08.16
* @brief         小车刹车
* @param[in]     time:延时时间
* @param[out]    void
* @retval        void
* @par History   无
*/
void brake(int time)   //刹车                
{
  digitalWrite(Left_motor_go, LOW);//左前进停 
  digitalWrite(Left_motor_back, LOW);//左后退停
  digitalWrite(Right_motor_go, LOW);//右前进停
  digitalWrite(Right_motor_back, LOW);//右后退停
  //睡眠 
  delay(time * 100); 
} 

/**
* Function       left
* @author        Danny
* @date          2017.08.16
* @brief         小车左转(左轮不动，右轮前进)
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void left(int l, int r)  //前进左转 
{
  //左电机停止
  digitalWrite(Left_motor_go, LOW);    //左电机前进禁止
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm, l);//左转速
  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, r);//右转速
}

/**
* Function       right
* @author        Danny
* @date          2017.08.16
* @brief         小车右转(右轮不动，左轮前进)
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void right(int l, int r)		//前进右转 
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);   //左电机前进使能
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm, l);//左转速
  //右电机停止
  digitalWrite(Right_motor_go, LOW);   //右电机前进禁止
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, r);//右转速
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.08.16
* @brief         小车原地左转(左轮后退，右轮前进)
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void spin_left(int l,int r)		//原地左转 
{
  //左电机后退
  digitalWrite(Left_motor_go, LOW);     //左电机前进禁止
  digitalWrite(Left_motor_back, HIGH);  //左电机后退使能
  softPwmWrite(Left_motor_pwm, l);
  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, r);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.08.16
* @brief         小车原地右转(右轮后退，左轮前进)
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void spin_right(int l,int r)	//原地右转 
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);    //左电机前进使能
  digitalWrite(Left_motor_back, LOW);   //左电机后退禁止
  softPwmWrite(Left_motor_pwm, l);
  //右电机后退
  digitalWrite(Right_motor_go, LOW);    //右电机前进禁止
  digitalWrite(Right_motor_back, HIGH); //右电机后退使能
  softPwmWrite(Right_motor_pwm, r);
}

/**
* Function       back
* @author        Danny
* @date          2017.08.16
* @brief         小车后退
* @param[in]     time：延时时间
* @param[out]    void
* @retval        void
* @par History   无
*/
void back(int time) 
{
  //左电机后退
  digitalWrite(Left_motor_go, LOW);     //左电机前进禁止
  digitalWrite(Left_motor_back, HIGH);  //左电机后退使能
  softPwmWrite(Left_motor_pwm, 40);
  //右电机后退
  digitalWrite(Right_motor_go, LOW);    //右电机前进禁止
  digitalWrite(Right_motor_back, HIGH); //右电机后退使能
  softPwmWrite(Right_motor_pwm, 40);
  
  delay(time);          //延迟时间 
}

/**
* Function       key_scan
* @author        Danny
* @date          2017.08.16
* @brief         按键检测(包含软件按键去抖)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void key_scan()
{
  while (digitalRead(key));       //当按键没有被按下一直循环
  while (!digitalRead(key))       //当按键被按下时
  {
    delay(10);	                  //延时10ms
    if (digitalRead(key)  ==  LOW)//第二次判断按键是否被按下
    {
      delay(100);
      while (!digitalRead(key));  //判断按键是否被松开
    }
  }
}

/**
* Function       Distance
* @author        Danny
* @date          2017.08.16
* @brief         超声波测一次前方的距离
* @param[in]     void
* @param[out]    void
* @retval        float:distance返回距离值
* @par History   无
*/
float Distance()
{
	float distance1;    //距离 
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	struct timeval tv4;
	long start, stop;
	
	digitalWrite(TrigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(TrigPin, HIGH);      //向Trig脚输入至少10US的高电平
	delayMicroseconds(10);
	digitalWrite(TrigPin, LOW);
    
	//防止程序未检测到电平变化，陷入死循环，加入一个超时重测机制
    gettimeofday(&tv3, NULL);        //超时重测机制开始计时
	//int gettimeofday(struct timeval*tv, struct timezone *tz);
	//其参数tv是保存获取时间结果的结构体，参数tz用于保存时区结果
	start = tv3.tv_sec * 1000000 + tv3.tv_usec;
	while(!digitalRead(EchoPin) == 1)
	{
		gettimeofday(&tv4, NULL);    //超时重测机制结束计时
		stop = tv4.tv_sec * 1000000 + tv4.tv_usec;
		
		if ((stop - start) > 30000)  //最大测5米时的时间值：10/340=0.03s
		{
			return -1;               //超时返回-1
		}
	} 
	
	//防止程序未检测到电平变化，陷入死循环，加入一个超时重测机制
	gettimeofday(&tv1, NULL);      //当echo脚电平变高时开始计时
    start = tv1.tv_sec*1000000+tv1.tv_usec;
	while(!digitalRead(EchoPin) == 0)
	{
		gettimeofday(&tv3,NULL);   //超时重测机制开始计时
		stop = tv3.tv_sec*1000000+tv3.tv_usec;
		if ((stop - start) > 30000)
		{
			return -1;
		}
	}                              //超时重测机制结束计时
	gettimeofday(&tv2, NULL);      //当echo脚电平变低时结束计时

	start = tv1.tv_sec * 1000000 + tv1.tv_usec;
	stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

	distance1 = (float)(stop - start)/1000000 * 34000 / 2;
	return distance1;
}

/**
* Function       bubble
* @author        Danny
* @date          2017.08.16
* @brief         超声波测五次的数据进行冒泡排序
* @param[in1]    a:超声波数组首地址
* @param[in2]    n:超声波数组大小
* @param[out]    void
* @retval        void
* @par History   无
*/
void bubble(unsigned long *a, int n)     //对距离进行排序 
{
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = i + 1; j < n; j++)
    {
      if (a[i] > a[j])
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
}

/**
* Function       Distane_test
* @author        Danny
* @date          2017.08.16
* @brief         超声波测五次，去掉最大值,最小值,
*                取平均值,提高测试准确性
* @param[in]     void
* @param[out]    void
* @retval        float:distance返回距离值
* @par History   无
*/
float Distance_test()   		//距离取值求平均，提高检测性能 
{
  float distance2;
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
     distance2 = Distance();
	 //超时返回-1，重新测试
	 while((int)distance2 == -1)
	 {
		 distance2 = Distance();
	 }
    //过滤掉测试距离中出现的错误数据大于500
    while ( (int)distance2 >= 500 || (int)distance2 == 0)
    {
         distance2 = Distance();
    }
    ultrasonic[num] = distance2;
    num++;
	delay(10);
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance2 = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  
  printf("distance:%f\n",distance2);      //打印测试的距离
  return distance2;
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         先调用setup初始化配置里面的按键扫描函数，
*                循迹模式开启,综合系统 
* @param[in]     void
* @retval        void
* @par History   无
*/
void main()
{
  //wiringPi初始化
  wiringPiSetup();
  //初始化电机驱动IO口为输出方式
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  //创建两个软件控制的PWM脚
  softPwmCreate(Left_motor_pwm,0,255); 
  softPwmCreate(Right_motor_pwm,0,255);
  //定义按键接口为输入接口
  pinMode(key, INPUT);
  //定义四路循迹红外传感器为输入接口
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);
  //初始化超声波引脚
  pinMode(EchoPin, INPUT);    //定义超声波输入脚
  pinMode(TrigPin, OUTPUT);   //定义超声波输出脚
  //调用按键扫描函数
  key_scan();
  
  MIDDLE();
  return;
}

void car()
{
   //检测到黑线时循迹模块相应的指示灯亮，端口电平为LOW
   //未检测到黑线时循迹模块相应的指示灯灭，端口电平为HIGH
   TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
   TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
   TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
   TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);
   //四路循迹引脚电平状态
   // 0 0 X 0
   // 1 0 X 0
   // 0 1 X 0
   //以上6种电平状态时小车原地右转，速度为250,延时80ms
   //处理右锐角和右直角的转动
   if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW){
   		spin_right(150,150);
   		delay(20);
   }
   //四路循迹引脚电平状态
   // 0 X 0 0       
   // 0 X 0 1 
   // 0 X 1 0       
   //处理左锐角和左直角的转动
   else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW)){
   		spin_left(150,150);
   		delay(20);
   }
   // 0 X X X
   //最左边检测到
   else if ( TrackSensorLeftValue1 == LOW){
   		spin_left(150,150);
   		delay(20);	
   } 
   // X X X 0
   //最右边检测到
   else if ( TrackSensorRightValue2 == LOW ){
   		spin_right(150,150);
   		delay(10);
   }
   //四路循迹引脚电平状态
   // X 0 1 X
   //处理左小弯
   else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH){
   		left(100,100);
   }
   //四路循迹引脚电平状态
   // X 1 0 X  
   //处理右小弯
   else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW){
   		right(100,100);
   }
   //四路循迹引脚电平状态
   // X 0 0 X
   //处理直线
   else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW){
   		run(200,200);	
   }
   else run(60,60); 
   //当为1 1 1 1时小车保持上一个小车运行状态
}

void MIDDLE()
{
	while(1)
	{
   		distance = Distance_test();
   		if (distance > 30){      //前方无障碍物则循迹 
   			car();   
   		}
   		else if (distance <= 30){          //前方有障碍物
     		spin_left(100,100);    
	 		delay(250);		//当靠近障碍物时原地右转大约60度
     		brake(1);
     		distance = Distance_test();    //再次测试判断前方距离
     		if (distance > 30)
     		{
        		run(100,100);            //转弯后当前方距离大于25cm时前进
        		LEFT();
     		}
     		else if (distance < 30)
     		{ 
       			spin_right(100,100);
	   			delay(700);               //距离小于30cm时向右原地转弯120度
       			brake(1);
       			distance =  Distance_test();     //再次测试判断前方距离
       			if (distance > 30)
       			{
          		run(100,100);            //转弯后当前方距离大于25cm时前进
        		RIGHT();
       			}
       			else if (distance <= 30)delay(1000000);
     		}
   		}
 	}
}

void LEFT()
{
	while(1)
	{
   		distance = Distance_test();
   		if (distance > 30){      //前方无障碍物则循迹 
   			car();   
   		}
   		else if (distance <= 30){          //前方有障碍物
     		spin_right(100,100);    
	 		delay(250);		//当靠近障碍物时原地右转大约60度
     		brake(1);
     		distance = Distance_test();    //再次测试判断前方距离
     		if (distance > 30)
     		{
        		run(100,100);            //转弯后当前方距离大于30cm时前进
        		MIDDLE();
     		}
     		else if (distance <= 30)
     		{ 
       			spin_left(100,100);    
	 			delay(250);		//当靠近障碍物时原地左转大约60度
	 			
	 			back(20);
	 			
	 			spin_right(100,100);    
	 			delay(250);		//当靠近障碍物时原地右转大约60度
	 			
	 			run(100,100);
	 			delay(400);
	 			RIGHT();	
     		}
   		}
 	}
}

void RIGHT()
{
	while(1)
	{
   		distance = Distance_test();
   		if (distance > 30){      //前方无障碍物则循迹 
   			car();   
   		}
   		else if (distance <= 30){          //前方有障碍物
     		spin_left(100,100);    
	 		delay(250);		//当靠近障碍物时原地右转大约60度
     		brake(1);
     		distance = Distance_test();    //再次测试判断前方距离
     		if (distance > 30)
     		{
        		run(100,100);            //转弯后当前方距离大于30cm时前进
        		MIDDLE();
     		}
     		else if (distance <= 30)
     		{ 
       			spin_right(100,100);    
	 			delay(250);		//当靠近障碍物时原地右转大约60度
	 			
	 			back(20);
	 			
	 			spin_left(100,100);    
	 			delay(250);		//当靠近障碍物时原地右转大约60度
	 			
	 			run(100,100);
	 			delay(400);
	 			LEFT();	
     		}
   		}
 	}
}


































