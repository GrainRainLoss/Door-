# Door 智能门锁
### 基于正点原子给出的stm32F103的库写出的智能门锁，能够实现远程开锁，并附带有手机网页版的app设计。主要是利用ESP8266实现与上位机服务器的通信，从而实现更多的目的
* The mechanical structure of the stm32 single-chip microcomputer and the steering gear is used to complete the unlocking. And through the esp8266 module to achieve network communication for control. Some open source libraries of stm32 are used in the code. Some network deployments are also involved. 
* 利用stm32单片机与舵机的机械结构完成开锁。并通过ESP8266模块实现网络通信进行控制。代码中利用了部分stm32的开源库。还涉及一些网络部署。

### 门锁机械结构大致如下图所示
![image](https://github.com/GrainRainLoss/Door-/blob/main/door_example.jpg)
### 门锁的界面演示大概如图
![image](https://github.com/GrainRainLoss/Door-/blob/main/door%E7%95%8C%E9%9D%A2.jpg)

* 本门锁硬件部分代码运用了正点原子给出的官方库并对其进行修改

### 本门锁硬件主要设备为 
* 舵机*1
* 奇奇怪怪的开锁结构
* stm32开发板*1
* rfid读卡器*1
* ESP8266模块*1


### 本门锁实现的主要功能
* 能够通过网络发送信号打开门锁
* 能够利用rfid读卡器模块来实现刷卡进门
* 与小艾同学的部分开源库相结合可以做到，大喊，小艾同学，开门，然后帅气的把门打开

### 本门锁项目致谢
感谢我的沙雕室友们帮我完成了前端设计、网络部署、机械结构的制作，仅以此纪念最好的2018宿舍
