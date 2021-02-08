/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         tracking.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        Ѳ��ʵ��
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

//ѭ���������Ŷ���
//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      9                 21                  7                   1
const int TrackSensorLeftPin1  =  9;   //������ߵ�һ��ѭ�����⴫��������ΪwiringPi����9��
const int TrackSensorLeftPin2  =  21;  //������ߵڶ���ѭ�����⴫��������ΪwiringPi����21��
const int TrackSensorRightPin1 =  7;   //�����ұߵ�һ��ѭ�����⴫��������ΪwiringPi����7��
const int TrackSensorRightPin2 =  1;   //�����ұߵڶ���ѭ�����⴫��������ΪwiringPi����1��

//�������ѭ���������Ųɼ������ݵı���
int TrackSensorLeftValue1;
int TrackSensorLeftValue2;
int TrackSensorRightValue1;
int TrackSensorRightValue2;

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
  if(flag==0){
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
 
  if (LeftDistance < 30 && RightDistance < 30 && FrontDistance < 30  )
  {
    //��Ʒ��ɫ,��ͷ
    corlor_led(ON, OFF, ON);
    brake;delay(1000000);
  }
  else if ( LeftDistance >= RightDistance) //����������������Ҳ࣬ԭ����ת
  {
    //����ɫ
    corlor_led(OFF, OFF, ON);
    spin_left(350);
  }
  else if (LeftDistance < RightDistance ) //�������Ҳ���������࣬ԭ����ת
  {
    //��Ʒ��ɫ,����ת
    corlor_led(ON, OFF, ON);
    spin_right(350);
  }
  }
  else if(flag==1){
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
 
  if (LeftDistance < 30 && FrontDistance < 30  )
  {
    //��Ʒ��ɫ,��ͷ
    corlor_led(ON, OFF, ON);
    back(500);
    spin_left(330);
    run(60,60);
    delay(500);
    car();
  }
  else if ( LeftDistance >= 30) //����������������Ҳ࣬ԭ����ת
  {
    //����ɫ
    corlor_led(OFF, OFF, ON);
    spin_left(330);
    car();
  }
  }else if(flag==(-1)){
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
 
  if (RightDistance < 30 && FrontDistance < 30  )
  {
    //��Ʒ��ɫ,��ͷ
    corlor_led(ON, OFF, ON);
    back(500);
    spin_right(330);
    run(60,60);
    delay(500);
    car();
  }
  else if ( RightDistance >= 30) //����������������Ҳ࣬ԭ����ת
  {
    //����ɫ
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
* @brief         С��ǰ��
* @param[in1]    LeftSpeed:����ٶ�
* @param[in2]    RightSpeed:�Ҳ��ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
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
* @param[in1]    left_speed:�����ٶ�
* @param[in2]    right_speed:�����ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void right(int left_speed, int right_speed)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, left_speed);

  //�ҵ��ֹͣ
  digitalWrite(Right_motor_go, LOW);   //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, right_speed);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in1]    left_speed:�����ٶ�
* @param[in2]    right_speed:�����ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_left(int left_speed, int right_speed)
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, left_speed);

  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, right_speed);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.08.16
* @brief         С��ԭ����ת(���ֺ��ˣ�����ǰ��)
* @param[in1]    left_speed:�����ٶ�
* @param[in2]    right_speed:�����ٶ�
* @param[out]    void
* @retval        void
* @par History   ��
*/
void spin_right(int left_speed, int right_speed)
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, left_speed);

  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, right_speed);
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
void car(){
	//��⵽����ʱѭ��ģ����Ӧ��ָʾ�������˿ڵ�ƽΪLOW
   //δ��⵽����ʱѭ��ģ����Ӧ��ָʾ���𣬶˿ڵ�ƽΪHIGH
   TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
   TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
   TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
   TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);

   //��·ѭ�����ŵ�ƽ״̬
   // 0 0 X 0
   // 1 0 X 0
   // 0 1 X 0
   //����6�ֵ�ƽ״̬ʱС��ԭ����ת���ٶ�Ϊ250,��ʱ80ms
   //��������Ǻ���ֱ�ǵ�ת��
   if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW)
   {
     spin_right(150, 150);
     delay(80);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // 0 X 0 0       
   // 0 X 0 1 
   // 0 X 1 0       
   //��������Ǻ���ֱ�ǵ�ת��
   else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW))
   {
     spin_left(150, 150);
     delay(80);
   }
   // 0 X X X
   //����߼�⵽
   else if ( TrackSensorLeftValue1 == LOW)
   {
     spin_left(150, 150);
    // delay(10);
   }
   // X X X 0
   //���ұ߼�⵽
   else if ( TrackSensorRightValue2 == LOW )
   {
     spin_right(150, 150);
    // delay(10);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 0 1 X
   //������С��
   else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
   {
     left(0, 150);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 1 0 X  
   //������С��
   else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
   {
     right(150, 0);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 0 0 X
   //����ֱ��
   else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
   {
     run(150, 150);
   }
   //��Ϊ1 1 1 1ʱС��������һ��С������״̬
   else run(60,60);
}
/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         �ȵ���setup��ʼ����������İ���ɨ�躯����
*                ѭ��ģʽ����
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

  float distance;
  //������·ѭ�����⴫����Ϊ����ӿ�
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);

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
	car();
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
    car();
  }
  else if (  distance < 30  )//������С��30ʱ���ö����ɫ���Ƴ���
  {
    servo_color_carstate();
  }
 }
 return;
}

