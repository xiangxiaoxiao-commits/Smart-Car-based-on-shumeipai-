/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         tracking.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        巡线实验
* @details
* @par History  见如下说明
*
*/
#include <wiringPi.h>
#include <softPwm.h>

//定义引脚
int Left_motor_go = 28;       //左电机前进AIN2连接Raspberry的wiringPi编码28口
int Left_motor_back = 29;     //左电机后退AIN1连接Raspberry的wiringPi编码29口

int Right_motor_go = 24;      //右电机前进BIN2连接Raspberry的wiringPi编码24口
int Right_motor_back = 25;    //右电机后退BIN1连接Raspberry的wiringPi编码25口

int Left_motor_pwm = 27;      //左电机控速PWMA连接Raspberry的wiringPi编码27口
int Right_motor_pwm = 23;     //右电机控速PWMB连接Raspberry的wiringPi编码23口

int key = 10;                 //定义按键为Raspberry的wiringPi编码10口

//循迹红外引脚定义
//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      9                 21                  7                   1
const int TrackSensorLeftPin1  =  9;   //定义左边第一个循迹红外传感器引脚为wiringPi编码9口
const int TrackSensorLeftPin2  =  21;  //定义左边第二个循迹红外传感器引脚为wiringPi编码21口
const int TrackSensorRightPin1 =  7;   //定义右边第一个循迹红外传感器引脚为wiringPi编码7口
const int TrackSensorRightPin2 =  1;   //定义右边第二个循迹红外传感器引脚为wiringPi编码1口

//定义各个循迹红外引脚采集的数据的变量
int TrackSensorLeftValue1;
int TrackSensorLeftValue2;
int TrackSensorRightValue1;
int TrackSensorRightValue2;

int EchoPin = 30;         //定义回声脚为连接Raspberry的wiringPi编码30口
int TrigPin = 31;         //定义触发脚为连接Raspberry的wiringPi编码31口

//定义引脚
int LED_R = 3;           //LED_R接在Raspberry上的wiringPi编码3口
int LED_G = 2;           //LED_G接在Raspberry上的wiringPi编码2口
int LED_B = 5;           //LED_B接在Raspberry上的wiringPi编码5口

//定义舵机引脚
int ServoPin = 4;

//初始化舵机位置向前
int ServoPos = 90;

const int AvoidSensorLeft =  26; //定义左边避障的红外传感器引脚为wiringPi编码26口
const int AvoidSensorRight = 0;  //定义右边避障的红外传感器引脚为wiringPi编码0口

int LeftSensorValue ;            //定义变量来保存红外传感器采集的数据大小
int RightSensorValue ;

//函数声明
void brake();
void spin_left(int);
void spin_right(int);
void back(int);
float Distance_test();
void bubble(unsigned long *, int);
void corlor_led(int, int, int);

/**
* Function       servo_pulse
* @author        Danny
* @date          2017.08.16
* @brief         定义一个脉冲函数，用来模拟方式产生PWM值
*                时基脉冲为20ms,该脉冲高电平部分在0.5-2.5ms
*                控制0-180度
* @param[in1]    ServoPin:舵机引脚
* @param[in2]    myangle:舵机转动指定的角度
* @param[out]    void
* @retval        void
* @par History   无
*/
void servo_pulse(int ServoPin, int myangle)
{
  int PulseWidth;                    //定义脉宽变量
  PulseWidth = (myangle * 11) + 500; //将角度转化为500-2480 的脉宽值
  digitalWrite(ServoPin, HIGH);      //将舵机接口电平置高
  delayMicroseconds(PulseWidth);     //延时脉宽值的微秒数
  digitalWrite(ServoPin, LOW);       //将舵机接口电平置低
  delay(20 - PulseWidth / 1000);     //延时周期内剩余时间
  return;
}

/**
* Function       servo_appointed_detection
* @author        Danny
* @date          2017.08.16
* @brief         舵机旋转到指定角度
* @param[in]     pos：指定的角度
* @param[out]    void
* @retval        void
* @par History   无
*/
void servo_appointed_detection(int pos)
{
  int i = 0;
  for (i = 0; i <= 15; i++)    //产生PWM个数，等效延时以保证能转到响应角度
  {
    servo_pulse(ServoPin, pos);//模拟产生PWM
  }
}

/**
* Function       servo_color_carstate
* @author        Danny
* @date          2017.08.16
* @brief         舵机转向超声波测距避障行驶,led根据车的状态显示相应的颜色
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void servo_color_carstate()
{ 
  if(flag==0){
  	float distance;
  //定义舵机位置变量和小车前方,左侧,右侧距离
  int iServoPos = 0;
  int LeftDistance = 0;    //左方距离值变量LeftDistance
  int RightDistance = 0;   //右方距离值变量RightDistance
  int FrontDistance = 0;   //前方距离值变量FrontDistance
  corlor_led(ON, OFF, OFF);//开红灯
  back(80);                //避免突然停止,刹不住车
  brake();

  //舵机旋转到0度,即右侧,测距
  servo_appointed_detection(0);
  delay(500);
  distance = Distance_test();  //测距
  RightDistance = distance;    //所测的右侧距离赋给变量RightDistance
  //printf("rightdistance :%d\n",RightDistance);
 
  //舵机旋转到180度,即左侧,测距
  servo_appointed_detection(180);
  delay(500);
  distance = Distance_test();  //测距
  LeftDistance = distance;//所测的左侧距离赋给变量LeftDistance
  // printf("leftdistance :%d\n",LeftDistance);

  //舵机旋转到90度,即左侧,测距
  servo_appointed_detection(90);
  delay(500);
  distance = Distance_test();
  FrontDistance = distance;//所测前侧距离付给变量FrontDistance
  //printf("FrontDistance:%d\n",FrontDistance);
 
  if (LeftDistance < 30 && RightDistance < 30 && FrontDistance < 30  )
  {
    //亮品红色,掉头
    corlor_led(ON, OFF, ON);
    brake;delay(1000000);
  }
  else if ( LeftDistance >= RightDistance) //当发现左侧距离大于右侧，原地左转
  {
    //亮蓝色
    corlor_led(OFF, OFF, ON);
    spin_left(350);
  }
  else if (LeftDistance < RightDistance ) //当发现右侧距离大于左侧，原地右转
  {
    //亮品红色,向右转
    corlor_led(ON, OFF, ON);
    spin_right(350);
  }
  }
  else if(flag==1){
  	  	float distance;
  //定义舵机位置变量和小车前方,左侧,右侧距离
  int iServoPos = 0;
  int LeftDistance = 0;    //左方距离值变量LeftDistance
  int RightDistance = 0;   //右方距离值变量RightDistance
  int FrontDistance = 0;   //前方距离值变量FrontDistance
  corlor_led(ON, OFF, OFF);//开红灯
  back(80);                //避免突然停止,刹不住车
  brake();

  //舵机旋转到0度,即右侧,测距
  servo_appointed_detection(0);
  delay(500);
  distance = Distance_test();  //测距
  RightDistance = distance;    //所测的右侧距离赋给变量RightDistance
  //printf("rightdistance :%d\n",RightDistance);
 
  //舵机旋转到180度,即左侧,测距
  servo_appointed_detection(180);
  delay(500);
  distance = Distance_test();  //测距
  LeftDistance = distance;//所测的左侧距离赋给变量LeftDistance
  // printf("leftdistance :%d\n",LeftDistance);

  //舵机旋转到90度,即左侧,测距
  servo_appointed_detection(90);
  delay(500);
  distance = Distance_test();
  FrontDistance = distance;//所测前侧距离付给变量FrontDistance
  //printf("FrontDistance:%d\n",FrontDistance);
 
  if (LeftDistance < 30 && FrontDistance < 30  )
  {
    //亮品红色,掉头
    corlor_led(ON, OFF, ON);
    back(500);
    spin_left(330);
    run(60,60);
    delay(500);
    car();
  }
  else if ( LeftDistance >= 30) //当发现左侧距离大于右侧，原地左转
  {
    //亮蓝色
    corlor_led(OFF, OFF, ON);
    spin_left(330);
    car();
  }
  }else if(flag==(-1)){
  	float distance;
  //定义舵机位置变量和小车前方,左侧,右侧距离
  int iServoPos = 0;
  int LeftDistance = 0;    //左方距离值变量LeftDistance
  int RightDistance = 0;   //右方距离值变量RightDistance
  int FrontDistance = 0;   //前方距离值变量FrontDistance
  corlor_led(ON, OFF, OFF);//开红灯
  back(80);                //避免突然停止,刹不住车
  brake();

  //舵机旋转到0度,即右侧,测距
  servo_appointed_detection(0);
  delay(500);
  distance = Distance_test();  //测距
  RightDistance = distance;    //所测的右侧距离赋给变量RightDistance
  //printf("rightdistance :%d\n",RightDistance);
 
  //舵机旋转到180度,即左侧,测距
  servo_appointed_detection(180);
  delay(500);
  distance = Distance_test();  //测距
  LeftDistance = distance;//所测的左侧距离赋给变量LeftDistance
  // printf("leftdistance :%d\n",LeftDistance);

  //舵机旋转到90度,即左侧,测距
  servo_appointed_detection(90);
  delay(500);
  distance = Distance_test();
  FrontDistance = distance;//所测前侧距离付给变量FrontDistance
  //printf("FrontDistance:%d\n",FrontDistance);
 
  if (RightDistance < 30 && FrontDistance < 30  )
  {
    //亮品红色,掉头
    corlor_led(ON, OFF, ON);
    back(500);
    spin_right(330);
    run(60,60);
    delay(500);
    car();
  }
  else if ( RightDistance >= 30) //当发现左侧距离大于右侧，原地左转
  {
    //亮蓝色
    corlor_led(OFF, OFF, ON);
    spin_left(330);
    car();
  }
  }
}

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         小车前进
* @param[in1]    LeftSpeed:左侧速度
* @param[in2]    RightSpeed:右侧速度
* @param[out]    void
* @retval        void
* @par History   无
*/
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
void run(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);   //左电机前进使能
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm, left_speed );

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, right_speed);
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
void brake(int time)
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);

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
void left(int left_speed, int right_speed)
{
  //左电机停止
  digitalWrite(Left_motor_go, LOW);    //左电机前进禁止
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm, left_speed);

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, right_speed);
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
void right(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);   //左电机前进使能
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm, left_speed);

  //右电机停止
  digitalWrite(Right_motor_go, LOW);   //右电机前进禁止
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, right_speed);
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
void spin_left(int left_speed, int right_speed)
{
  //左电机后退
  digitalWrite(Left_motor_go, LOW);     //左电机前进禁止
  digitalWrite(Left_motor_back, HIGH);  //左电机后退使能
  softPwmWrite(Left_motor_pwm, left_speed);

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm, right_speed);
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
void spin_right(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);    //左电机前进使能
  digitalWrite(Left_motor_back, LOW);   //左电机后退禁止
  softPwmWrite(Left_motor_pwm, left_speed);

  //右电机后退
  digitalWrite(Right_motor_go, LOW);    //右电机前进禁止
  digitalWrite(Right_motor_back, HIGH); //右电机后退使能
  softPwmWrite(Right_motor_pwm, right_speed);
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

  delay(time );
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
void car(){
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
   if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW)
   {
     spin_right(150, 150);
     delay(80);
   }
   //四路循迹引脚电平状态
   // 0 X 0 0       
   // 0 X 0 1 
   // 0 X 1 0       
   //处理左锐角和左直角的转动
   else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW))
   {
     spin_left(150, 150);
     delay(80);
   }
   // 0 X X X
   //最左边检测到
   else if ( TrackSensorLeftValue1 == LOW)
   {
     spin_left(150, 150);
    // delay(10);
   }
   // X X X 0
   //最右边检测到
   else if ( TrackSensorRightValue2 == LOW )
   {
     spin_right(150, 150);
    // delay(10);
   }
   //四路循迹引脚电平状态
   // X 0 1 X
   //处理左小弯
   else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
   {
     left(0, 150);
   }
   //四路循迹引脚电平状态
   // X 1 0 X  
   //处理右小弯
   else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
   {
     right(150, 0);
   }
   //四路循迹引脚电平状态
   // X 0 0 X
   //处理直线
   else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
   {
     run(150, 150);
   }
   //当为1 1 1 1时小车保持上一个小车运行状态
   else run(60,60);
}
/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         先调用setup初始化配置里面的按键扫描函数，
*                循迹模式开启
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

  float distance;
  //定义四路循迹红外传感器为输入接口
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);

  //初始化RGB三色LED的IO口为输出方式
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  //定义按键接口为输出接口
  pinMode(key, INPUT);

  //初始化超声波引脚
  pinMode(EchoPin, INPUT);    //定义超声波输入脚
  pinMode(TrigPin, OUTPUT);   //定义超声波输出脚

  //舵机初始化为向前
  servo_appointed_detection(ServoPos);
  //舵机初始化为输出模式
  pinMode(ServoPin, OUTPUT);

  //定义左右传感器为输入接口
  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);


  //调用按键扫描函数
  key_scan();
 
 while(1)
  {
   distance = Distance_test();        //测量前方距离
   if (distance > 50  )    //障碍物距离大于50时，开启左右红外辅助避障
   {
    //遇到障碍物,红外避障模块的指示灯亮,端口电平为LOW
    //未遇到障碍物,红外避障模块的指示灯灭,端口电平为HIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(120); //右边探测到有障碍物，有信号返回，原地向左转
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(120);//左边探测到有障碍物，有信号返回，原地向右转
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(120);//当两侧均检测到障碍物时调用固定方向的避障(原地右转)
    }
    //距离大于50时前进,亮绿灯
	car();
    corlor_led(OFF, ON, OFF);
  }
  else if ((distance >= 30 && distance <= 50))
  {
    //遇到障碍物,红外避障模块的指示灯亮,端口电平为LOW
    //未遇到障碍物,红外避障模块的指示灯灭,端口电平为HIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(120); //右边探测到有障碍物，有信号返回，原地向左转
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(120);//左边探测到有障碍物，有信号返回，原地向右转
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(120);//当两侧均检测到障碍物时调用固定方向的避障(原地右转)
    }
    //距离在30-50之间时慢速前进
    car();
  }
  else if (  distance < 30  )//当距离小于30时调用舵机颜色控制程序
  {
    servo_color_carstate();
  }
 }
 return;
}

