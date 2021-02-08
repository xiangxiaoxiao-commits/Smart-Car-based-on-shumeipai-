/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         servo_ultrasonic_avoid.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        �������̨����������
* @details
* @par History  ������˵��
*
*/
#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define ON  1       //ʹ��LED
#define OFF 0       //��ֹLED
#define flag 0		//�ܵ�ָʾ

//��������
int Left_motor_go = 28;   //����ǰ��AIN2����Raspberry��wiringPi����28��
int Left_motor_back = 29; //��������AIN1����Raspberry��wiringPi����29��

int Right_motor_go = 24;  //�ҵ��ǰ��BIN2����Raspberry��wiringPi����24��
int Right_motor_back = 25;//�ҵ������BIN1����Raspberry��wiringPi����25��

int Left_motor_pwm = 27;  //��������PWMA����Raspberry��wiringPi����27��
int Right_motor_pwm = 23; //�ҵ������PWMB����Raspberry��wiringPi����23��

//����
int key = 10;             //���尴��ΪRaspberry��wiringPi����10��

//������
int EchoPin = 30;         //���������Ϊ����Raspberry��wiringPi����30��
int TrigPin = 31;         //���崥����Ϊ����Raspberry��wiringPi����31��

//��������
int LED_R = 3;           //LED_R����Raspberry�ϵ�wiringPi����3��
int LED_G = 2;           //LED_G����Raspberry�ϵ�wiringPi����2��
int LED_B = 5;           //LED_B����Raspberry�ϵ�wiringPi����5��

//����������
int ServoPin = 4;

//��ʼ�����λ����ǰ
int ServoPos = 90;

const int AvoidSensorLeft =  26; //������߱��ϵĺ��⴫��������ΪwiringPi����26��
const int AvoidSensorRight = 0;  //�����ұ߱��ϵĺ��⴫��������ΪwiringPi����0��

int LeftSensorValue ;            //���������������⴫�����ɼ������ݴ�С
int RightSensorValue ;

//��������
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
* @brief         ����һ�����庯��������ģ�ⷽʽ����PWMֵ
*                ʱ������Ϊ20ms,������ߵ�ƽ������0.5-2.5ms
*                ����0-180��
* @param[in1]    ServoPin:�������
* @param[in2]    myangle:���ת��ָ���ĽǶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void servo_pulse(int ServoPin, int myangle)
{
  int PulseWidth;                    //�����������
  PulseWidth = (myangle * 11) + 500; //���Ƕ�ת��Ϊ500-2480 ������ֵ
  digitalWrite(ServoPin, HIGH);      //������ӿڵ�ƽ�ø�
  delayMicroseconds(PulseWidth);     //��ʱ����ֵ��΢����
  digitalWrite(ServoPin, LOW);       //������ӿڵ�ƽ�õ�
  delay(20 - PulseWidth / 1000);     //��ʱ������ʣ��ʱ��
  return;
}

/**
* Function       servo_appointed_detection
* @author        Danny
* @date          2017.08.16
* @brief         �����ת��ָ���Ƕ�
* @param[in]     pos��ָ���ĽǶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void servo_appointed_detection(int pos)
{
  int i = 0;
  for (i = 0; i <= 15; i++)    //����PWM��������Ч��ʱ�Ա�֤��ת����Ӧ�Ƕ�
  {
    servo_pulse(ServoPin, pos);//ģ�����PWM
  }
}

/**
* Function       servo_color_carstate
* @author        Danny
* @date          2017.08.16
* @brief         ���ת��������������ʻ,led���ݳ���״̬��ʾ��Ӧ����ɫ
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void servo_color_carstate()
{
  float distance;
  //������λ�ñ�����С��ǰ��,���,�Ҳ����
  int iServoPos = 0;
  int LeftDistance = 0;    //�󷽾���ֵ����LeftDistance
  int RightDistance = 0;   //�ҷ�����ֵ����RightDistance
  int FrontDistance = 0;   //ǰ������ֵ����FrontDistance
  corlor_led(ON, OFF, OFF);//�����
  back(80);                //����ͻȻֹͣ,ɲ��ס��
  brake();

  //�����ת��0��,���Ҳ�,���
  servo_appointed_detection(0);
  delay(500);
  distance = Distance_test();  //���
  RightDistance = distance;    //������Ҳ���븳������RightDistance
  //printf("rightdistance :%d\n",RightDistance);
 
  //�����ת��180��,�����,���
  servo_appointed_detection(180);
  delay(500);
  distance = Distance_test();  //���
  LeftDistance = distance;//����������븳������LeftDistance
  // printf("leftdistance :%d\n",LeftDistance);

  //�����ת��90��,�����,���
  servo_appointed_detection(90);
  delay(500);
  distance = Distance_test();
  FrontDistance = distance;//����ǰ����븶������FrontDistance
  //printf("FrontDistance:%d\n",FrontDistance);
 
  switch(flag)
	{
		case -1:	//���ܵ�
			if (RightDistance < 30 && FrontDistance < 30  )		//�ҷ���ǰ�����ϰ���
			{
				//��Ʒ��ɫ,����
				corlor_led(ON, OFF, ON);
				back(100);	//����
				spin_right(250);
				run(100,100);	//��ת��ǰ��
				delay(100);		//�ƶ������ܵ���Ⱦ���
				flag=1;	
			}
			else if (RightDistance > 30 && FrontDistance < 30  )	//�ҷ����ϰ��ǰ����
			{
				//��Ʒ��ɫ,����ת
				corlor_led(ON, OFF, ON);
				spin_right(250);
				flag=0;	
			}
			break;

		case 0:
			if (LeftDistance < 30 && RightDistance < 30 && FrontDistance < 30  )	//����ǰ�������ϰ���
			{
				brake();
				flag=0;
			}
			else if (LeftDistance > 30 && FrontDistance < 30  )	//�����ϰ��ǰ����
			{
				//����ɫ
				corlor_led(OFF, OFF, ON);
				spin_left(250);
				flag=-1;	
			}
			else if (RightDistance > 30 && LeftDistance < 30 && FrontDistance < 30  )	//�ҷ����ϰ��ǰ����
			{
				//��Ʒ��ɫ,����
				corlor_led(ON, OFF, ON);
				back(100);	//����
				spin_left(250);
				run(100,100);	//��ת��ǰ��
				delay(50);		//�ƶ������ܵ���Ⱦ���
				flag=1;	
			}
			break;

		case 1:		//���ܵ�
			if (LeftDistance < 30 && FrontDistance < 30  )		//�󷽺�ǰ�����ϰ���
			{
				//��Ʒ��ɫ,����
				corlor_led(ON, OFF, ON);
				back(100);	//����
				spin_left(250);
				run(100,100);	//��ת��ǰ��
				delay(100);		//�ƶ������ܵ���Ⱦ���
				flag=-1;	
			}
			else if (LeftDistance > 30 && FrontDistance < 30  )	//�����ϰ��ǰ����
			{
				//����ɫ
				corlor_led(OFF, OFF, ON);
				spin_left(250);
				flag=0;	
			}
			break;
	}
}

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         С��ǰ��
* @param[in1]    LeftSpeed:����ٶ�
* @param[in2]    RightSpeed:�Ҳ��ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void run(int LeftSpeed, int RightSpeed)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, LeftSpeed);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, RightSpeed);
}

/**
* Function       brake
* @author        Danny
* @date          2017.08.16
* @brief         С��ɲ��
* @param[in]
* @param[out]    void
* @retval        void
* @par History   ��
*/
void brake()
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
}

/**
* Function       left
* @author        Danny
* @date          2017.08.16
* @brief         С����ת(���ֲ���������ǰ��)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void left()
{
  //����ֹͣ
  digitalWrite(Left_motor_go, LOW);    //����ǰ����ֹ
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 0);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 80);
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
  softPwmWrite(Left_motor_pwm, 80);

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
* @param[in]     time:��ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_left(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, 100);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 100);

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
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 100);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 100);

  delay(time);
}

/**
* Function       back
* @author        Danny
* @date          2017.08.16
* @brief         С������
* @param[in]     time:��ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void back(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, 80);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 80);

  delay(time);
}

/**
* Function       corlor_led
* @author        Danny
* @date          2017.08.16
* @brief         ��R,G,B��ɫ�Ĳ�ͬ����γ�7�ֲ�ͬ��ɫ��
* @param[in1]    v_iRed:Red����
* @param[in2]    v_iGreen:Green����
* @param[in3]    v_iBlue:Blue����
* @retval        void
* @par History   ��
*/
void corlor_led(int v_iRed, int v_iGreen, int v_iBlue)
{
  //��ɫLED
  v_iRed == ON ? digitalWrite(LED_R, HIGH): digitalWrite(LED_R, LOW);
 
  //��ɫLED
  v_iGreen == ON ? digitalWrite(LED_G, HIGH) : digitalWrite(LED_G, LOW);
  
  //��ɫLED
  v_iBlue == ON ? digitalWrite(LED_B, HIGH) : digitalWrite(LED_B, LOW);
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
	struct timeval tv4;
	long start, stop;
	
	digitalWrite(TrigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(TrigPin, HIGH);      //��Trig����������10US�ĸߵ�ƽ
	delayMicroseconds(15);
	digitalWrite(TrigPin, LOW);
    
	//��ֹ����δ��⵽��ƽ�仯��������ѭ��������һ����ʱ�ز����
    gettimeofday(&tv3, NULL);        //��ʱ�ز���ƿ�ʼ��ʱ
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
    while ( distance >= 500 || (int)distance == 0)
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
  return;
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         �ȵ��ð���ɨ�躯����
*                �������̨�ĳ���������ģʽ����,
*                �������ģ�鸨������(������������ä��)
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

  //��ʼ��RGB��ɫLED��IO��Ϊ�����ʽ
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  //���尴���ӿ�Ϊ����ӿ�
  pinMode(key, INPUT);

  //��ʼ������������
  pinMode(EchoPin, INPUT);    //���峬���������
  pinMode(TrigPin, OUTPUT);   //���峬���������

  //�����ʼ��Ϊ��ǰ
  servo_appointed_detection(ServoPos);
  //�����ʼ��Ϊ���ģʽ
  pinMode(ServoPin, OUTPUT);

  //�������Ҵ�����Ϊ����ӿ�
  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);

  //���ð���ɨ�躯��
  key_scan();
  
  while(1)
  {
   distance = Distance_test();        //����ǰ������
   if (distance > 50  )    //�ϰ���������50ʱ���������Һ��⸨������
   {
    //�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪLOW
    //δ�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪHIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(120); //�ұ�̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(120);//���̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(120);//���������⵽�ϰ���ʱ���ù̶�����ı���(ԭ����ת)
    }
    //�������50ʱǰ��,���̵�
	run(150, 150);
    corlor_led(OFF, ON, OFF);
  }
  else if ((distance >= 30 && distance <= 50))
  {
    //�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪLOW
    //δ�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪHIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(120); //�ұ�̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(120);//���̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(120);//���������⵽�ϰ���ʱ���ù̶�����ı���(ԭ����ת)
    }
    //������30-50֮��ʱ����ǰ��
    run(120, 120);
  }
  else if (  distance < 30  )//������С��30ʱ���ö����ɫ���Ƴ���
  {
    servo_color_carstate();
  }
 }
	return;
}



