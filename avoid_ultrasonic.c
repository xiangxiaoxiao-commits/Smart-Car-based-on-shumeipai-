/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         avoid_ultrasonic.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        ����������ʵ��
* @details
* @par History  ������˵��
*
*/
#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//��������
int Left_motor_go = 28;       //����ǰ��AIN2����Raspberry��wiringPi����28��
int Left_motor_back = 29;     //��������AIN1����Raspberry��wiringPi����29��

int Right_motor_go = 24;      //�ҵ��ǰ��BIN2����Raspberry��wiringPi����24��
int Right_motor_back = 25;    //�ҵ������BIN1����Raspberry��wiringPi����25��

int Left_motor_pwm = 27;      //��������PWMA����Raspberry��wiringPi����27��
int Right_motor_pwm = 23;     //�ҵ������PWMB����Raspberry��wiringPi����23��

int key = 10;                 //���尴��ΪRaspberry��wiringPi����10��

int EchoPin = 30;             //���������Ϊ����Raspberry��wiringPi����30��
int TrigPin = 31;             //���崥����Ϊ����Raspberry��wiringPi����31��

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         С��ǰ��
* @param[in1]    left_speed:�����ٶ�
* @param[in2]    right_speed:�����ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void run(int left_speed, int right_speed)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, left_speed );

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, right_speed);
}

/**
* Function       brake
* @author        Danny
* @date          2017.08.16
* @brief         С��ɲ��
* @param[in]     time:��ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
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
* @brief         С����ת(���ֲ���������ǰ��)
* @param[in1]    left_speed:�����ٶ�
* @param[in2]    right_speed:�����ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void left(int left_speed, int right_speed)
{
  //����ֹͣ
  digitalWrite(Left_motor_go, LOW);    //����ǰ����ֹ
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, left_speed);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, right_speed);
}

/**
* Function       right
* @author        Danny
* @date          2017.08.16
* @brief         С����ת(���ֲ���������ǰ��)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void right()
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 150);

  //�ҵ��ֹͣ
  digitalWrite(Right_motor_go, LOW);   //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 0);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in]     time����ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_left(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, 200);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 200);
  
  delay(time);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in]     time:��ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_right(int time)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm,200);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 200);
  
  delay(time);
}

/**
* Function       back
* @author        Danny
* @date          2017.08.16
* @brief         С������
* @param[in]     time����ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void back(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, 40);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 40);

  delay(time );
}

/**
* Function       key_scan
* @author        Danny
* @date          2017.08.16
* @brief         �������(�����������ȥ��)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void key_scan()
{
  while (digitalRead(key));       //������û�б�����һֱѭ��
  while (!digitalRead(key))       //������������ʱ
  {
    delay(10);	                  //��ʱ10ms
    if (digitalRead(key)  ==  LOW)//�ڶ����жϰ����Ƿ񱻰���
    {
      delay(100);
      while (!digitalRead(key));  //�жϰ����Ƿ��ɿ�
    }
  }
}

/**
* Function       Distance
* @author        Danny
* @date          2017.08.16
* @brief         ��������һ��ǰ���ľ���
* @param[in]     void
* @param[out]    void
* @retval        float:distance���ؾ���ֵ
* @par History   ��
*/
float Distance()
{
	float distance;
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	long start, stop;
	
	digitalWrite(TrigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(TrigPin, HIGH);      //��Trig����������10US�ĸߵ�ƽ
	delayMicroseconds(10);
	digitalWrite(TrigPin, LOW);
    
	//��ֹ����δ��⵽��ƽ�仯��������ѭ��������һ����ʱ�ز����
    gettimeofday(&tv3, NULL);        //��ʱ�ز���ƿ�ʼ��ʱ
	//int gettimeofday(struct timeval*tv, struct timezone *tz);
	//�����tv�Ǳ����ȡʱ�����Ľṹ�壬����tz���ڱ���ʱ�����
	start = tv3.tv_sec * 1000000 + tv3.tv_usec;
	while(!digitalRead(EchoPin) == 1)
	{
		gettimeofday(&tv4, NULL);    //��ʱ�ز���ƽ�����ʱ
		stop = tv4.tv_sec * 1000000 + tv4.tv_usec;
		
		if ((stop - start) > 30000)  //����5��ʱ��ʱ��ֵ��10/340=0.03s
		{
			return -1;               //��ʱ����-1
		}
	} 
	
	//��ֹ����δ��⵽��ƽ�仯��������ѭ��������һ����ʱ�ز����
	gettimeofday(&tv1, NULL);      //��echo�ŵ�ƽ���ʱ��ʼ��ʱ
    start = tv1.tv_sec*1000000+tv1.tv_usec;
	while(!digitalRead(EchoPin) == 0)
	{
		gettimeofday(&tv3,NULL);   //��ʱ�ز���ƿ�ʼ��ʱ
		stop = tv3.tv_sec*1000000+tv3.tv_usec;
		if ((stop - start) > 30000)
		{
			return -1;
		}
	}                              //��ʱ�ز���ƽ�����ʱ
	gettimeofday(&tv2, NULL);      //��echo�ŵ�ƽ���ʱ������ʱ

	start = tv1.tv_sec * 1000000 + tv1.tv_usec;
	stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

	distance = (float)(stop - start)/1000000 * 34000 / 2;
	return distance;
}

/**
* Function       bubble
* @author        Danny
* @date          2017.08.16
* @brief         ����������ε����ݽ���ð������
* @param[in1]    a:�����������׵�ַ
* @param[in2]    n:�����������С
* @param[out]    void
* @retval        void
* @par History   ��
*/
void bubble(unsigned long *a, int n)

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
* @brief         ����������Σ�ȥ�����ֵ,��Сֵ,
*                ȡƽ��ֵ,��߲���׼ȷ��
* @param[in]     void
* @param[out]    void
* @retval        float:distance���ؾ���ֵ
* @par History   ��
*/
float Distance_test()
{
  float distance;
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
     distance = Distance();
	 //��ʱ����-1�����²���
	 while((int)distance == -1)
	 {
		 distance = Distance();
	 }
    //���˵����Ծ����г��ֵĴ������ݴ���500
    while ( (int)distance >= 500 || (int)distance == 0)
    {
         distance = Distance();
    }
    ultrasonic[num] = distance;
    num++;
	delay(10);
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  
  printf("distance:%f\n",distance);      //��ӡ���Եľ���
  return distance;
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         �ȵ��ð���ɨ�躯����
*                ����������ģʽ����
* @param[in]     void
* @retval        void
* @par History   ��
*/
void main()
{
  float distance;
  
  //wiringPi��ʼ��
  wiringPiSetup();
  
  //��ʼ���������IO��Ϊ�����ʽ
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  
  //��������������Ƶ�PWM��
  softPwmCreate(Left_motor_pwm,0,255); 
  softPwmCreate(Right_motor_pwm,0,255);

  //���尴���ӿ�Ϊ����ӿ�
   pinMode(key, INPUT);

  //��ʼ������������
  pinMode(EchoPin, INPUT);    //���峬���������
  pinMode(TrigPin, OUTPUT);   //���峬���������

  //���ð���ɨ�躯��
  key_scan();

  while(1)
  {
   distance = Distance_test();

   if (distance > 55)
   {
     run(255, 255);      //�������ϰ����Զʱȫ��ǰ��
   }
   else if (distance >= 30 && distance <= 55)
   {
     run(200, 200);      //���쿿���ϰ���ʱ����ǰ��
   }
   else if (distance < 30)
   {
     spin_right(350);    //�������ϰ���ʱԭ����ת��Լ90��
     brake(1);
     distance = Distance_test();    //�ٴβ����ж�ǰ������
     if (distance >= 30)
     {
       run(200, 200);    //ת���ǰ���������25cmʱǰ��
     }
     else if (distance < 30)
     { 
       spin_left(800);     //ת���ǰ������С��25cmʱ����ԭ��ת��180��
       brake(1);
      distance =  Distance_test();  //�ٴβ����ж�ǰ������
       if (distance >= 30)
       {
         run(200, 200);  //ת���ǰ���������25cmʱǰ��
       }
       else if (distance < 30)
       {
         spin_left(400); //ת���ǰ������С��25cmʱ����ԭ��ת��90��
         brake(1);
       }
     }
   }
 }
 return;
}

