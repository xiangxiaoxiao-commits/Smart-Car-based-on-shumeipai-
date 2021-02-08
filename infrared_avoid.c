/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         infrared_avoid.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        �������ʵ��
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

int key = 10;                 //���尴��ΪRaspberry��wiringPi����10��

const int AvoidSensorLeft =  26; //������߱��ϵĺ��⴫��������ΪwiringPi����26��
const int AvoidSensorRight = 0;  //�����ұ߱��ϵĺ��⴫��������ΪwiringPi����0��

int LeftSensorValue ;            //���������������⴫�����ɼ������ݴ�С
int RightSensorValue ;

/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         С��ǰ��
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void run()
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, 250);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 250);
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
  softPwmWrite(Left_motor_pwm, 150);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, 150);

  delay(time * 100);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in]     time����ʱʱ��
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

  delay(time * 100);
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
  digitalWrite(Left_motor_go, LOW);       //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);    //��������ʹ��
  softPwmWrite(Left_motor_pwm, 150);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);     //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH);  //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, 150);

  delay(time * 100);
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
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         �ȵ��ð���ɨ�躯�������ź������ģʽ����
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
  softPwmCreate(Left_motor_pwm,0,255); 
  softPwmCreate(Right_motor_pwm,0,255);

  //���尴���ӿ�Ϊ����ӿ�
  pinMode(key, INPUT);

  //�������Ҵ�����Ϊ����ӿ�
  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);
  
  //���ð���ɨ�躯��
  key_scan(); 
  
  while(1)
  {
    //�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪLOW
    //δ�����ϰ���,�������ģ���ָʾ����,�˿ڵ�ƽΪHIGH
    LeftSensorValue  = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == HIGH)
    {
      run();        //�������δ��⵽�ϰ���ʱ����ǰ������
    }
    else if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(2); //�ұ�̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(2);//���̽�⵽���ϰ�����źŷ��أ�ԭ������ת
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(2);//���������⵽�ϰ���ʱ���ù̶�����ı���(ԭ����ת)
    }
  }
  return;
}
