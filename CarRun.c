/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         CarRun.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief       С��ǰ�����������ۺ�ʵ��
* @details
* @par History  ������˵��
*
*/
#include <wiringPi.h>
#include <softPwm.h>

//��������
int Left_motor_go = 28;       //����ǰ��AIN2����Raspberry��wiringPi����28��
int Left_motor_back = 29;     //��������AIN1����Raspberry��wiringPi����29��

int Right_motor_go = 24;      //�ҵ��ǰ��BIN2����Raspberry��wiringPi����24��
int Right_motor_back = 25;    //�ҵ������BIN1����Raspberry��wiringPi����25��

int Left_motor_pwm = 27;      //��������PWMA����Raspberry��wiringPi����27��
int Right_motor_pwm = 23;     //�ҵ������PWMB����Raspberry��wiringPi����23��

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         С��ǰ��
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void run(int time)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 200);   //PWM����0-255���٣������ֲ���������

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 200);  //PWM����0-255���٣������ֲ���������

  //��ʱ
  delay(time * 100);
}

/**
* Function       brake
* @author        Danny
* @date          2017.08.16
* @brief         С��ɲ��
* @param[in]     time
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

  //��ʱ
  delay(time * 100);
}

/**
* Function       left
* @author        Danny
* @date          2017.08.16
* @brief         С����ת ��ת(���ֲ���,����ǰ��)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void left(int time)
{
  //����ֹͣ
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 0);      //��ߵ���ٶ���Ϊ0(0-255)

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 200);  //�ұߵ���ٶ���200(0-255)

  //��ʱ
  delay(time * 100);
}

/**
* Function       right
* @author        Danny
* @date          2017.08.16
* @brief         С����ת ��ת(����ǰ��,���ֲ���)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void right(int time)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 200);    //��ߵ���ٶ���200(0-255)

  //�ҵ��ֹͣ
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, LOW);  //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 0);     //�ұߵ���ٶ���0(0-255)

  //��ʱ
  delay(time * 100);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_left(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);    //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH); //��������ʹ��
  softPwmWrite(Left_motor_pwm, 200);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 200);

  //��ʱ
  delay(time * 100);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_right(int time)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 200);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 200);

  //��ʱ
  delay(time * 100);
}

/**
* Function       back
* @author        Danny
* @date          2017.08.16
* @brief         С������ 
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   ��
*/
void back(int time)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, 200);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 200);

  //��ʱ
  delay(time * 100);
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         ����ʱ2����ǰ��1������1s,��ת2s,��ת2s,
*                ԭ����ת3s,ԭ����ת3s,ֹͣ0.5s
* @param[in]     void
* @retval        void
* @par History   ��
*/
void main()
{
   //wiringPi��ʼ��
   wiringPiSetup();
  
  //��ʼ���������IO��Ϊ�����ʽ
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  
  //��������������Ƶ�PWM��
  //int softPwmCreate(int pin,int initialValue,int pwmRange);
  softPwmCreate(Left_motor_pwm,0,255); 
  softPwmCreate(Right_motor_pwm,0,255);
  
  while(1)
  {
   delay(2000);      //��ʱ2s  
   run(10);          //ǰ��1s(10 * 100ms)
   //back(10);         //����1s
   //left(20);         //��ת2s
   //right(20);        //��ת2s
   spin_left(3.75);    //ԭ����ת3s
   run(3);
   spin_left(15);
   run(5);
   spin_left(5);
   run(3);
   back(3);
   spin_right(2.5); 
   run(3);  
   back(3);
   spin_right(2.5);
   run(5);
   back(3);
   spin_right(3.75);
   run(2);
   spin_right(3.75);
   run(3); 
   back(3);
   spin_left(3.75);
   run(2);
   spin_right(3.75);
   run(3);
   brake(5);         //ֹͣ0.5s
  }
  return;
}
