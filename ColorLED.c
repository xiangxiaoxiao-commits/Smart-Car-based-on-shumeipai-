/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         ColorLED.c
* @author       Danny
* @version      V1.0
* @date         2017.08.16
* @brief        �߲�̽�յ�ʵ��
* @details
* @par History  ������˵��
*/
#include <wiringPi.h>

#define ON 1     //ʹ��LED
#define OFF 0    //��ֹLED

//��������
int LED_R = 3;  //LED_R����Raspberry�ϵ�wiringPi����3��
int LED_G = 2;  //LED_G����Raspberry�ϵ�wiringPi����2��
int LED_B = 5;  //LED_B����Raspberry�ϵ�wiringPi����5��

/**
* Function       color_led
* @author        Danny
* @date          2017.08.16
* @brief         ��R,G,B��ɫ�Ĳ�ͬ����γ�7�ֲ�ͬ��ɫ��
* @param[in1]    Red����
* @param[in2]    Green����
* @param[in3]    Blue����
* @retval        void
* @par History   ��
*/
void color_led(int v_iRed, int v_iGreen, int v_iBlue)
{
  //��ɫLED
  v_iRed == ON ? digitalWrite(LED_R, HIGH): digitalWrite(LED_R, LOW);
 
  //��ɫLED
  v_iGreen == ON ? digitalWrite(LED_G, HIGH) : digitalWrite(LED_G, LOW);
  
  //��ɫLED
  v_iBlue == ON ? digitalWrite(LED_B, HIGH) : digitalWrite(LED_B, LOW);
}

/**
* Function       main
* @author        Danny
* @date          2017.08.16
* @brief         ѭ����7ɫLED
* @param[in]     void
* @retval        void
* @par History   ��
*/
int main()
{
	wiringPiSetup();
	
	//RGB����ģʽ����Ϊ���ģʽ
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
	
	while (1)
	{                        //   LED_R   LED_G    LED_B
    color_led(ON, OFF, OFF); //   1        0        0
    delay(1000);
    color_led(OFF, ON, OFF); //   0        1        0
    delay(1000);
    color_led(OFF, OFF, ON); //   0        0        1
    delay(1000);
    color_led(ON, ON, OFF);  //   1        1        0
    delay(1000);
    color_led(ON, OFF, ON);  //   1        0        1
    delay(1000);
    color_led(OFF, ON, ON);  //   0        1        1
    delay(1000);
    color_led(ON, ON, ON);   //   1        1        1	
	}
   return 0;	
}