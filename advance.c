/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         advance.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        С��ǰ��ʵ��
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
* @param[in]     time��ʱʱ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void run(int time)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  //����ָ���ܽŵ�PWMֵ
  softPwmWrite(Left_motor_pwm, 150);    //pwm����0-255֮�����������в���

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);   //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW);  //�ҵ�����˽�ֹ
  //����ָ���ܽŵ�PWMֵ
  softPwmWrite(Right_motor_pwm, 150);   //pwm����0-255֮�����������в���

  //��ʱ
  delay(time * 100); //delay����Ĭ������ms��Ϊ��λ��
  return;
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         ����ʱ2s������ǰ��
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
  
  delay(2000);     //��ʱ2s
  
  while(1)
  {
   run(10);        //ǰ��
  }
  return;
}
