# -*- coding:UTF-8 -*-
import RPi.GPIO as GPIO
import time

# 定义循迹数字
tracknum = 0

# 循迹红外引脚定义
# TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
#      3                 5                  4                   18
TrackSensorLeftPin1 = 3  # 定义左边第一个循迹红外传感器引脚为3口
TrackSensorLeftPin2 = 5  # 定义左边第二个循迹红外传感器引脚为5口
TrackSensorRightPin1 = 4  # 定义右边第一个循迹红外传感器引脚为4口
TrackSensorRightPin2 = 18  # 定义右边第二个循迹红外传感器引脚为18口

#RGB三色灯引脚定义
LED_R = 22
LED_G = 27
LED_B = 24

# 小车电机引脚定义
IN1 = 20
IN2 = 21
IN3 = 19
IN4 = 26
ENA = 16
ENB = 13

#蜂鸣器引脚定义
buzzer = 8

# speed
speed1 = 25
speed2 = 10
speed3 = 20

# 小车按键定义
key = 8

# 超声波引脚定义
EchoPin = 0
TrigPin = 1

# 设置GPIO口为BCM编码方式
GPIO.setmode(GPIO.BCM)

# 忽略警告信息
GPIO.setwarnings(False)


# 电机引脚初始化为输出模式
# 按键引脚初始化为输入模式
# 超声波引脚初始化
def init():
    global pwm_ENA
    global pwm_ENB
    global pwm_rled
    global pwm_gled
    global pwm_bled
    GPIO.setup(ENA, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setup(IN1, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(IN2, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(ENB, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setup(IN3, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(IN4, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(key, GPIO.IN)
    GPIO.setup(TrackSensorLeftPin1, GPIO.IN)
    GPIO.setup(TrackSensorLeftPin2, GPIO.IN)
    GPIO.setup(TrackSensorRightPin1, GPIO.IN)
    GPIO.setup(TrackSensorRightPin2, GPIO.IN)
    GPIO.setup(EchoPin, GPIO.IN)
    GPIO.setup(TrigPin, GPIO.OUT)
    # 蜂鸣器定义输出
    GPIO.setup(buzzer, GPIO.OUT, initial=GPIO.HIGH)
    # RGB三色引脚定义输出
    GPIO.setup(LED_R, GPIO.OUT)
    GPIO.setup(LED_G, GPIO.OUT)
    GPIO.setup(LED_B, GPIO.OUT)
    # 设置pwm引脚和频率为2000hz
    pwm_ENA = GPIO.PWM(ENA, 2000)
    pwm_ENB = GPIO.PWM(ENB, 2000)
    pwm_ENA.start(0)
    pwm_ENB.start(0)
    pwm_rled = GPIO.PWM(LED_R, 1000)
    pwm_gled = GPIO.PWM(LED_G, 1000)
    pwm_bled = GPIO.PWM(LED_B, 1000)
    pwm_rled.start(0)
    pwm_gled.start(0)
    pwm_bled.start(0)

#七彩灯亮指定颜色
def color_led_pwm(iRed,iGreen, iBlue):
    v_red = (100*iRed)/255
    v_green = (100*iGreen)/255
    v_blue = (100*iBlue)/255
    pwm_rled.ChangeDutyCycle(v_red)
    pwm_gled.ChangeDutyCycle(v_green)
    pwm_bled.ChangeDutyCycle(v_blue)
    time.sleep(0.02)

def find_line():
    while True:

        run(15,15)

        if Distance_test() < 30:
            return 0

        # 检测到黑线时循迹模块相应的指示灯亮，端口电平为LOW
        # 未检测到黑线时循迹模块相应的指示灯灭，端口电平为HIGH
        TrackSensorLeftValue1 = GPIO.input(TrackSensorLeftPin1)
        TrackSensorLeftValue2 = GPIO.input(TrackSensorLeftPin2)
        TrackSensorRightValue1 = GPIO.input(TrackSensorRightPin1)
        TrackSensorRightValue2 = GPIO.input(TrackSensorRightPin2)

        if TrackSensorLeftValue1 == False or TrackSensorLeftValue2 == False or TrackSensorRightValue1 == False or TrackSensorRightValue2 == False:
            return 0


# 循迹
def trackingB():
    while True:

        if Distance_test() < 18:
            return 0

        # 检测到黑线时循迹模块相应的指示灯亮，端口电平为LOW
        # 未检测到黑线时循迹模块相应的指示灯灭，端口电平为HIGH
        TrackSensorLeftValue1 = GPIO.input(TrackSensorLeftPin1)
        TrackSensorLeftValue2 = GPIO.input(TrackSensorLeftPin2)
        TrackSensorRightValue1 = GPIO.input(TrackSensorRightPin1)
        TrackSensorRightValue2 = GPIO.input(TrackSensorRightPin2)
        print("tracknum is : " + str(tracknum))

        # 四路循迹引脚电平状态
        # 0 0 X 0
        # 1 0 X 0
        # 0 1 X 0
        # 以上6种电平状态时小车原地右转
        # 处理右锐角和右直角的转动
        if (TrackSensorLeftValue1 == False or TrackSensorLeftValue2 == False) and TrackSensorRightValue2 == False:
            spin_right(speed1, speed1)
            time.sleep(0.08)

        # 四路循迹引脚电平状态
        # 0 X 0 0
        # 0 X 0 1
        # 0 X 1 0
        # 处理左锐角和左直角的转动
        elif TrackSensorLeftValue1 == False and (TrackSensorRightValue1 == False or TrackSensorRightValue2 == False):
            spin_left(speed1, speed1)
            time.sleep(0.08)

        # 0 X X X
        # 最左边检测到
        elif TrackSensorLeftValue1 == False:
            spin_left(speed2, speed2)

        # X X X 0
        # 最右边检测到
        elif TrackSensorRightValue2 == False:
            spin_right(speed2, speed2)

        # 四路循迹引脚电平状态
        # X 0 1 X
        # 处理左小弯
        elif TrackSensorLeftValue2 == False and TrackSensorRightValue1 == True:
            left(0, speed3)

        # 四路循迹引脚电平状态
        # X 1 0 X
        # 处理右小弯
        elif TrackSensorLeftValue2 == True and TrackSensorRightValue1 == False:
            right(speed3, 0)

        # 四路循迹引脚电平状态
        # X 0 0 X
        # 处理直线
        elif TrackSensorLeftValue2 == False and TrackSensorRightValue1 == False:
            run(speed1, speed1)

        # 当为1 1 1 1时小车保持上一个小车运行状态


# 小车前进
def run(leftspeed, rightspeed):
    #前进颜色为红
    GPIO.output(LED_R, GPIO.HIGH)
    GPIO.output(LED_G, GPIO.LOW)
    GPIO.output(LED_B, GPIO.LOW)
    #前进指令
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 按时间前进
def run_time(delaytime):
    # 前进颜色为红
    GPIO.output(LED_R, GPIO.HIGH)
    GPIO.output(LED_G, GPIO.LOW)
    GPIO.output(LED_B, GPIO.LOW)
    # 前进指令
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    pwm_ENA.ChangeDutyCycle(20)
    pwm_ENB.ChangeDutyCycle(20)
    time.sleep(delaytime)


# 小车后退
def back(leftspeed, rightspeed):
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.HIGH)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.HIGH)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 小车左转
def left(leftspeed, rightspeed):
    # 左转颜色为绿
    GPIO.output(LED_R, GPIO.LOW)
    GPIO.output(LED_G, GPIO.HIGH)
    GPIO.output(LED_B, GPIO.LOW)
    # 左转指令
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 小车右转
def right(leftspeed, rightspeed):
    # 右转颜色为蓝
    GPIO.output(LED_R, GPIO.LOW)
    GPIO.output(LED_G, GPIO.LOW)
    GPIO.output(LED_B, GPIO.HIGH)
    # 右转指令
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 小车原地左转
def spin_left(leftspeed, rightspeed):
    # 左转颜色为绿
    GPIO.output(LED_R, GPIO.LOW)
    GPIO.output(LED_G, GPIO.HIGH)
    GPIO.output(LED_B, GPIO.LOW)
    # 左转指令
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.HIGH)
    GPIO.output(IN3, GPIO.HIGH)
    GPIO.output(IN4, GPIO.LOW)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 小车原地右转
def spin_right(leftspeed, rightspeed):
    # 右转颜色为蓝
    GPIO.output(LED_R, GPIO.LOW)
    GPIO.output(LED_G, GPIO.LOW)
    GPIO.output(LED_B, GPIO.HIGH)
    # 右转指令
    GPIO.output(IN1, GPIO.HIGH)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.HIGH)
    pwm_ENA.ChangeDutyCycle(leftspeed)
    pwm_ENB.ChangeDutyCycle(rightspeed)


# 小车停止
def brake():
    # 停止颜色为紫色
    GPIO.output(LED_R, GPIO.HIGH)
    GPIO.output(LED_G, GPIO.LOW)
    GPIO.output(LED_B, GPIO.HIGH)
    # 停止指令
    GPIO.output(IN1, GPIO.LOW)
    GPIO.output(IN2, GPIO.LOW)
    GPIO.output(IN3, GPIO.LOW)
    GPIO.output(IN4, GPIO.LOW)

# 小车鸣笛
def whistle():
    GPIO.output(buzzer, GPIO.LOW)
    time.sleep(0.2)
    GPIO.output(buzzer, GPIO.HIGH)
    time.sleep(0.001)

# 按键检测
def key_scan():
    while GPIO.input(key):
        pass
    while not GPIO.input(key):
        time.sleep(0.01)
        if not GPIO.input(key):
            time.sleep(0.01)
            while not GPIO.input(key):
                pass


# 超声波函数
def Distance_test():
    GPIO.output(TrigPin, GPIO.HIGH)
    time.sleep(0.000015)
    GPIO.output(TrigPin, GPIO.LOW)
    while not GPIO.input(EchoPin):
        pass
    t1 = time.time()
    while GPIO.input(EchoPin):
        pass
    t2 = time.time()
    print("distance is %d " % (((t2 - t1) * 340 / 2) * 100))
    time.sleep(0.01)
    return ((t2 - t1) * 340 / 2) * 100


# 延时2s
time.sleep(2)

# try/except语句用来检测try语句块中的错误，
# 从而让except语句捕获异常信息并处理。
try:
    init()
    key_scan()
    while True:
        distance = Distance_test()
        if distance > 150:
            run_time(1.3)
            brake()
            time.sleep(1)
            break
        elif distance > 30:
            run(20, 20)  # 当距离障碍物较远时全速前进
        elif 14 <= distance <= 30:
            run(10, 10)  # 当快靠近障碍物时慢速前进
        elif distance < 14:

            whistle()  # 鸣笛

            spin_right(41, 41)
            print("spin_right success!")
            time.sleep(0.34)  # 当靠近障碍物时原地右转大约90度

            run_time(0.8)  # 直走0.1s
            print("go strait success!")

            brake()  # stop 0.1s
            print("brake success!")
            time.sleep(0.3)

            spin_left(47, 47)  # 左转90度
            print("spin_left success!")
            time.sleep(0.31)

            run_time(1.234)  # 直走0.06s
            print("go strait 2 success!")

            brake()  # stop 0.1s
            print("brake 2 success!")
            time.sleep(0.3)

            spin_left(44, 44)  # 左转90度
            print("spin_left 2 success!")
            time.sleep(0.3)

            brake()  # stop 0.1s
            print("brake 3 success!")
            time.sleep(0.3)

            run(33, 33)  # 绕过障碍物往回走到线上
            print("run back success!")
            time.sleep(0.35)
            
            if find_line() == 0:
                spin_right(28, 28)
                time.sleep(0.37)  # 当靠近障碍物时原地右转大约90度
                
            if (trackingB() == 0):  # 最后一个右拐之前进行循迹
                brake()  # stop 0.1s
                print("tracking stop!")
                time.sleep(0.3)

except KeyboardInterrupt:
    pass
pwm_ENA.stop()
pwm_ENB.stop()
GPIO.cleanup()
