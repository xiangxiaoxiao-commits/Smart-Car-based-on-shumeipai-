#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//��������
//(��) 
int Left_motor_go = 28;       //����ǰ��AIN2����Raspberry��wiringPi����28��
int Left_motor_back = 29;     //��������AIN1����Raspberry��wiringPi����29��
//(��) 
int Right_motor_go = 24;      //�ҵ��ǰ��BIN2����Raspberry��wiringPi����24��
int Right_motor_back = 25;    //�ҵ������BIN1����Raspberry��wiringPi����25��
//(����) 
int Left_motor_pwm = 27;      //��������PWMA����Raspberry��wiringPi����27��
int Right_motor_pwm = 23;     //�ҵ������PWMB����Raspberry��wiringPi����23��
//(����) 
int key = 10;                 //���尴��ΪRaspberry��wiringPi����10��
//(�����봥��) 
int EchoPin = 30;             //���������Ϊ����Raspberry��wiringPi����30��
int TrigPin = 31;             //���崥����Ϊ����Raspberry��wiringPi����31��

float distance;
void car();
void MIDDLE();
void LEFT();
void RIGHT();

//ѭ���������Ŷ���
//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      9                 21                  7                   1
const int TrackSensorLeftPin1  =  9;   //������ߵ�һ��ѭ�����⴫��������ΪwiringPi����9�ڣ���1�� 
const int TrackSensorLeftPin2  =  21;  //������ߵڶ���ѭ�����⴫��������ΪwiringPi����21�ڣ���2�� 
const int TrackSensorRightPin1 =  7;   //�����ұߵ�һ��ѭ�����⴫��������ΪwiringPi����7�ڣ���1�� 
const int TrackSensorRightPin2 =  1;   //�����ұߵڶ���ѭ�����⴫��������ΪwiringPi����1�ڣ���2�� 

//�������ѭ���������Ųɼ������ݵı���
int TrackSensorLeftValue1;     //����һ�� 
int TrackSensorLeftValue2;     //������� 
int TrackSensorRightValue1;    //����һ�� 
int TrackSensorRightValue2;    //���Ҷ���

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
void run(int l,int r)    //��ǰ�ƶ� 
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);      //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);     //�������˽�ֹ
  softPwmWrite(Left_motor_pwm,  l );      //��ת��
  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  	  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW);    //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, r );      //��ת�� 
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
void brake(int time)   //ɲ��                
{
  digitalWrite(Left_motor_go, LOW);//��ǰ��ͣ 
  digitalWrite(Left_motor_back, LOW);//�����ͣ
  digitalWrite(Right_motor_go, LOW);//��ǰ��ͣ
  digitalWrite(Right_motor_back, LOW);//�Һ���ͣ
  //˯�� 
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
void left(int l, int r)  //ǰ����ת 
{
  //����ֹͣ
  digitalWrite(Left_motor_go, LOW);    //����ǰ����ֹ
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, l);//��ת��
  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, r);//��ת��
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
void right(int l, int r)		//ǰ����ת 
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);   //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);  //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, l);//��ת��
  //�ҵ��ֹͣ
  digitalWrite(Right_motor_go, LOW);   //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, r);//��ת��
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
void spin_left(int l,int r)		//ԭ����ת 
{
  //��������
  digitalWrite(Left_motor_go, LOW);     //����ǰ����ֹ
  digitalWrite(Left_motor_back, HIGH);  //��������ʹ��
  softPwmWrite(Left_motor_pwm, l);
  //�ҵ��ǰ��
  digitalWrite(Right_motor_go, HIGH);  //�ҵ��ǰ��ʹ��
  digitalWrite(Right_motor_back, LOW); //�ҵ�����˽�ֹ
  softPwmWrite(Right_motor_pwm, r);
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
void spin_right(int l,int r)	//ԭ����ת 
{
  //����ǰ��
  digitalWrite(Left_motor_go, HIGH);    //����ǰ��ʹ��
  digitalWrite(Left_motor_back, LOW);   //�������˽�ֹ
  softPwmWrite(Left_motor_pwm, l);
  //�ҵ������
  digitalWrite(Right_motor_go, LOW);    //�ҵ��ǰ����ֹ
  digitalWrite(Right_motor_back, HIGH); //�ҵ������ʹ��
  softPwmWrite(Right_motor_pwm, r);
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
  
  delay(time);          //�ӳ�ʱ�� 
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
	float distance1;    //���� 
	struct timeval tv1;
	struct timeval tv2;
	struct timeval tv3;
	struct timeval tv4;
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

	distance1 = (float)(stop - start)/1000000 * 34000 / 2;
	return distance1;
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
void bubble(unsigned long *a, int n)     //�Ծ���������� 
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
float Distance_test()   		//����ȡֵ��ƽ������߼������ 
{
  float distance2;
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
     distance2 = Distance();
	 //��ʱ����-1�����²���
	 while((int)distance2 == -1)
	 {
		 distance2 = Distance();
	 }
    //���˵����Ծ����г��ֵĴ������ݴ���500
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
  
  printf("distance:%f\n",distance2);      //��ӡ���Եľ���
  return distance2;
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         �ȵ���setup��ʼ����������İ���ɨ�躯����
*                ѭ��ģʽ����,�ۺ�ϵͳ 
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
  //������·ѭ�����⴫����Ϊ����ӿ�
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);
  //��ʼ������������
  pinMode(EchoPin, INPUT);    //���峬���������
  pinMode(TrigPin, OUTPUT);   //���峬���������
  //���ð���ɨ�躯��
  key_scan();
  
  MIDDLE();
  return;
}

void car()
{
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
   if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW){
   		spin_right(150,150);
   		delay(20);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // 0 X 0 0       
   // 0 X 0 1 
   // 0 X 1 0       
   //��������Ǻ���ֱ�ǵ�ת��
   else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW)){
   		spin_left(150,150);
   		delay(20);
   }
   // 0 X X X
   //����߼�⵽
   else if ( TrackSensorLeftValue1 == LOW){
   		spin_left(150,150);
   		delay(20);	
   } 
   // X X X 0
   //���ұ߼�⵽
   else if ( TrackSensorRightValue2 == LOW ){
   		spin_right(150,150);
   		delay(10);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 0 1 X
   //������С��
   else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH){
   		left(100,100);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 1 0 X  
   //������С��
   else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW){
   		right(100,100);
   }
   //��·ѭ�����ŵ�ƽ״̬
   // X 0 0 X
   //����ֱ��
   else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW){
   		run(200,200);	
   }
   else run(60,60); 
   //��Ϊ1 1 1 1ʱС��������һ��С������״̬
}

void MIDDLE()
{
	while(1)
	{
   		distance = Distance_test();
   		if (distance > 30){      //ǰ�����ϰ�����ѭ�� 
   			car();   
   		}
   		else if (distance <= 30){          //ǰ�����ϰ���
     		spin_left(100,100);    
	 		delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
     		brake(1);
     		distance = Distance_test();    //�ٴβ����ж�ǰ������
     		if (distance > 30)
     		{
        		run(100,100);            //ת���ǰ���������25cmʱǰ��
        		LEFT();
     		}
     		else if (distance < 30)
     		{ 
       			spin_right(100,100);
	   			delay(700);               //����С��30cmʱ����ԭ��ת��120��
       			brake(1);
       			distance =  Distance_test();     //�ٴβ����ж�ǰ������
       			if (distance > 30)
       			{
          		run(100,100);            //ת���ǰ���������25cmʱǰ��
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
   		if (distance > 30){      //ǰ�����ϰ�����ѭ�� 
   			car();   
   		}
   		else if (distance <= 30){          //ǰ�����ϰ���
     		spin_right(100,100);    
	 		delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
     		brake(1);
     		distance = Distance_test();    //�ٴβ����ж�ǰ������
     		if (distance > 30)
     		{
        		run(100,100);            //ת���ǰ���������30cmʱǰ��
        		MIDDLE();
     		}
     		else if (distance <= 30)
     		{ 
       			spin_left(100,100);    
	 			delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
	 			
	 			back(20);
	 			
	 			spin_right(100,100);    
	 			delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
	 			
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
   		if (distance > 30){      //ǰ�����ϰ�����ѭ�� 
   			car();   
   		}
   		else if (distance <= 30){          //ǰ�����ϰ���
     		spin_left(100,100);    
	 		delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
     		brake(1);
     		distance = Distance_test();    //�ٴβ����ж�ǰ������
     		if (distance > 30)
     		{
        		run(100,100);            //ת���ǰ���������30cmʱǰ��
        		MIDDLE();
     		}
     		else if (distance <= 30)
     		{ 
       			spin_right(100,100);    
	 			delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
	 			
	 			back(20);
	 			
	 			spin_left(100,100);    
	 			delay(250);		//�������ϰ���ʱԭ����ת��Լ60��
	 			
	 			run(100,100);
	 			delay(400);
	 			LEFT();	
     		}
   		}
 	}
}


































