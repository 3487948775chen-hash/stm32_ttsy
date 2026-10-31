# STM32 Peripheral Driver Experiments

<p align="center">
  <b>STM32 外设驱动学习与实践</b>
  <br>
  基于 STM32F103C8T6 · Standard Peripheral Library · C Language
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-STM32F103C8T6-blue" />
  <img src="https://img.shields.io/badge/Language-C-orange" />
  <img src="https://img.shields.io/badge/Library-STM32%20SPL-green" />
  <img src="https://img.shields.io/badge/IDE-Keil%20MDK-purple" />
  <img src="https://img.shields.io/badge/Status-Learning%20Completed-success" />
</p>

## 1. Introduction

本仓库记录了我跟随 B 站 **铁头山羊 STM32 教程** 学习 STM32 的全过程，主要包含基础外设操作、通信协议、驱动程序封装、中断系统、定时器以及 ADC 等实验。

开发平台主要使用 STM32F103C8T6，采用 STM32 标准外设库（Standard Peripheral Library）进行开发。

学习过程中，从最初的 GPIO 寄存器及外设初始化，逐步过渡到驱动封装和多外设协同工作，尝试理解 STM32 各个片上外设的工作原理，而不仅仅是调用库函数完成实验。

本仓库共包含 **24 个实验工程及 1 个基础工程模板**，按照学习过程进行编号，方便后续查阅、复习和扩展。

## 2. Development Environment

| Configuration | Description |
|---|---|
| MCU | STM32F103C8T6 |
| Core | ARM Cortex-M3 |
| Language | C |
| IDE | Keil MDK |
| Peripheral Library | STM32 Standard Peripheral Library |
| Debugger | ST-LINK V2 |
| Development Board | STM32F103C8T6 Development Board |

## 3. Repository Structure

```text
STM32/
│
├── 0_template/               # STM32 工程模板
│
├── 1_blinky_LED/             # GPIO LED 闪烁
├── 2_key_LED/                # 按键控制 LED
│
├── 3_USART_Init/             # USART 初始化
├── 4_USART_Send/             # USART 数据发送
├── 5_USART_printf_test/      # printf 串口重定向
├── 6_USART_RX_test/          # USART 数据接收
├── 7_USARTLibTest/           # USART 驱动封装
│
├── 8_I2CTest/                # 硬件 I2C
├── 9_SoftwareI2CTest/        # 软件模拟 I2C
│
├── 10_MyLibTest/             # 个人驱动库
├── 11_OLEDTest/              # OLED 显示
│
├── 12_ButtonTest/            # 按键外部中断
├── 13_ButtonLibTest/         # 按键驱动封装
├── 14_SPITest/               # SPI 通信
├── 15_USART_IT_Test/         # USART 接收中断
├── 16_DualButtonTest/        # 双按键中断
│
├── 17_ClockTreeTest/         # STM32 时钟树
├── 18_UserDelay/             # 定时器延时
├── 19_BreathingLight/        # PWM 呼吸灯
│
├── 20_HCSR04Test/            # 超声波测距
├── 21_PWMMeasurement/        # PWM 参数测量
│
├── 22_RegularSingleConv/     # ADC 单通道转换
├── 23_ADCTimerTrigger/       # 定时器触发 ADC
├── 24_MultiConv/             # ADC 多通道转换
│
└── .gitignore
```

## 4. Experiments

### 01. GPIO & Basic Control

#### 1. `1_blinky_LED` — LED Blinking

学习 STM32 最基础的 GPIO 输出控制，通过控制 GPIO 引脚的高低电平实现 LED 闪烁。

主要内容：
- GPIO 外设时钟使能
- GPIO 工作模式配置
- 推挽输出模式
- GPIO 输出电平控制
- 基础延时程序

#### 2. `2_key_LED` — Key Input & LED Control

通过按键输入控制 LED 状态，学习 GPIO 输入功能以及按键电平检测。

主要内容：
- GPIO 输入模式配置
- 上拉与下拉输入
- 按键状态检测
- 按键消抖的基本处理
- GPIO 输入与输出协同控制

### 02. USART Communication

#### 3. `3_USART_Init` — USART Initialization

学习 STM32 USART 外设的基本配置，建立串口通信的基础。

主要内容：
- USART 外设时钟使能
- GPIO 复用功能配置
- USART 参数配置
- 波特率设置
- 数据位、停止位及校验位
- USART 外设使能

#### 4. `4_USART_Send` — USART Data Transmission

在完成 USART 初始化的基础上，实现串口数据发送。

主要内容：
- USART 数据寄存器
- TXE（发送数据寄存器空）标志位
- 数据发送流程
- 串口发送字符串
- 轮询方式发送数据

#### 5. `5_USART_printf_test` — printf Redirection

将 C 语言标准输出函数 printf 重定向到 USART，实现通过串口输出格式化信息。

主要内容：
- printf 工作机制
- 标准输出重定向
- fputc 函数
- 串口格式化输出
- 整数、字符串等数据打印

#### 6. `6_USART_RX_test` — USART Data Reception

学习 USART 数据接收流程，实现串口接收数据并进行相应处理。

主要内容：
- USART 接收数据寄存器
- RXNE（读数据寄存器非空）标志位
- 数据接收流程
- 轮询方式接收数据
- 接收数据读取与处理

#### 7. `7_USARTLibTest` — USART Driver Library

对 USART 基础操作进行封装，尝试将底层外设初始化与上层应用逻辑分离。

主要内容：
- USART 驱动封装
- `.c` 与 `.h` 文件组织
- 函数接口设计
- 驱动初始化与调用
- 模块化程序设计

### 03. I2C Communication

#### 8. `8_I2CTest` — Hardware I2C

学习 STM32 硬件 I2C 外设的工作机制，通过硬件 I2C 控制外部设备。

主要内容：
- I2C 总线基本协议
- SCL 与 SDA 信号
- START 与 STOP 条件
- ACK 与 NACK 应答机制
- 7 位设备地址
- I2C 主机发送与接收
- I2C 状态标志位与通信时序

#### 9. `9_SoftwareI2CTest` — Software I2C

通过 GPIO 模拟 I2C 总线时序，实现软件 I2C 通信。

主要内容：
- GPIO 模拟 I2C
- 软件产生 START 和 STOP 条件
- 数据位发送与接收
- ACK 与 NACK 处理
- I2C 时序控制
- 软件 I2C 与硬件 I2C 的区别

### 04. Driver Library & OLED

#### 10. `10_MyLibTest` — Personal Driver Library

对前面学习的外设驱动进行整理与封装，建立自己的驱动代码库。

主要内容：
- 驱动模块化设计
- `.c` 与 `.h` 文件组织
- 驱动接口封装
- 多个外设驱动的复用
- 底层驱动与应用程序分离

涉及的驱动模块包括：
- Button
- Delay
- I2C
- OLED
- USART
- SPI

#### 11. `11_OLEDTest` — OLED Display

通过 I2C 驱动 OLED 显示屏，实现字符及图形显示。

主要内容：
- OLED 初始化
- I2C 通信控制
- OLED 显存与显示机制
- 字符显示
- 数字显示
- 字符串显示
- 图形显示
- 字库与字体文件管理

### 05. Interrupt System

#### 12. `12_ButtonTest` — External Interrupt

学习 STM32 外部中断控制器 EXTI，通过按键触发中断，实现按键事件响应。

主要内容：
- EXTI 外部中断
- GPIO 与 EXTI 映射
- 上升沿触发
- 下降沿触发
- 双边沿触发
- 中断标志位
- NVIC 中断配置

#### 13. `13_ButtonLibTest` — Button Driver Library

对按键驱动进行进一步封装，降低应用程序对底层 GPIO 操作的依赖。

主要内容：
- 按键初始化封装
- 按键驱动接口
- 按键状态处理
- 驱动模块化设计
- 按键功能复用

#### 14. `14_SPITest` — SPI Communication

学习 STM32 SPI 外设，通过 SPI 总线与外部存储器进行通信。

主要内容：
- SPI 通信协议
- SCK、MOSI、MISO 与 NSS 信号
- SPI 主从模式
- CPOL 与 CPHA
- SPI 全双工通信
- 数据发送与接收
- SPI 状态标志位
- W25Q64 Flash 存储器读写实验

#### 15. `15_USART_IT_Test` — USART Interrupt

在 USART 基础通信的基础上，引入中断接收机制，实现非阻塞式数据接收。

主要内容：
- USART 接收中断
- RXNE 中断标志
- NVIC 中断优先级配置
- USART 中断服务函数
- 中断方式接收数据
- 中断与主程序协同工作

#### 16. `16_DualButtonTest` — Dual Button Interrupt

通过两个按键实现双路外部中断控制，进一步理解 STM32 中断系统。

主要内容：
- 多路 GPIO 输入
- 双按键中断配置
- EXTI 中断线映射
- NVIC 优先级管理
- 多中断事件处理

### 06. Clock System & Timer

#### 17. `17_ClockTreeTest` — STM32 Clock Tree

学习 STM32 时钟系统，理解 MCU 内部时钟来源以及各个外设时钟的产生过程。

主要内容：
- HSI 内部高速时钟
- HSE 外部高速时钟
- PLL 锁相环
- SYSCLK 系统时钟
- AHB 总线时钟
- APB1 与 APB2 总线时钟
- 时钟分频与倍频
- 外设时钟使能
- RCC 时钟配置

#### 18. `18_UserDelay` — Timer Based Delay

学习 STM32 通用定时器的基本结构，使用定时器实现精确的时间控制。

主要内容：
- 定时器时钟配置
- 预分频器 PSC
- 自动重装载寄存器 ARR
- 计数器 CNT
- 定时器更新事件
- 更新中断
- 定时器标志位
- 基于定时器实现毫秒级及秒级延时

#### 19. `19_BreathingLight` — PWM Breathing Light

利用定时器 PWM 输出控制 LED 亮度，实现呼吸灯效果。

主要内容：
- 定时器输出比较
- PWM 输出模式
- PWM Mode 1
- CCR 捕获比较寄存器
- PWM 频率与占空比
- 占空比动态调整
- LED 亮度控制

### 07. Input Capture & PWM Measurement

#### 20. `20_HCSR04Test` — Ultrasonic Distance Measurement

使用 HC-SR04 超声波模块测量距离，通过定时器输入捕获获取回波脉冲宽度。

主要内容：
- HC-SR04 工作原理
- TRIG 触发信号
- ECHO 回波信号
- 定时器输入捕获
- 上升沿与下降沿捕获
- CCR 捕获寄存器
- 回波脉冲宽度计算
- 超声波距离换算

#### 21. `21_PWMMeasurement` — PWM Signal Measurement

利用一个定时器产生 PWM 信号，另一个定时器捕获输入信号，实现 PWM 参数测量。

主要内容：
- PWM 信号生成
- 定时器输入捕获
- 输入捕获通道配置
- 定时器从模式控制器
- TRGI 与 TRGO
- PWM 周期测量
- PWM 占空比计算
- 定时器之间的协同工作

### 08. Analog-to-Digital Conversion

#### 22. `22_RegularSingleConv` — ADC Regular Single Conversion

学习 STM32 ADC 的基本工作原理，实现单通道模拟信号采样与数字转换。

主要内容：
- ADC 基本工作原理
- 逐次逼近型 ADC
- ADC 分辨率
- 采样时间与转换时间
- ADC 常规通道
- 单通道转换
- EOC 转换完成标志
- ADC 数据读取

#### 23. `23_ADCTimerTrigger` — Timer Triggered ADC

利用定时器触发 ADC 转换，实现周期性、自动化的 ADC 采样。

主要内容：
- ADC 外部触发
- 定时器触发源
- 定时器更新事件
- ADC 规则组
- 定时器与 ADC 协同工作
- 周期性采样
- 采样时序控制

#### 24. `24_MultiConv` — ADC Multi-Channel Conversion

学习 ADC 多通道扫描转换，实现多个模拟输入通道的连续采样。

主要内容：
- ADC 多通道配置
- 规则组通道序列
- 扫描转换模式
- 通道采样顺序
- 转换结果读取
- 多通道数据管理

## 5. Peripheral Summary

本仓库涉及的主要 STM32 外设及技术如下：

| Peripheral / Module | Learning Content |
|---|---|
| GPIO | 输入、输出、复用功能 |
| USART | 串口收发、中断、printf 重定向 |
| I2C | 硬件 I2C、软件模拟 I2C |
| SPI | SPI 通信、W25Q64 读写 |
| EXTI | 外部中断、边沿触发 |
| NVIC | 中断使能与优先级 |
| RCC | 时钟树、系统时钟配置 |
| TIM | 定时器时基、更新中断 |
| PWM | 输出比较、PWM 输出 |
| Input Capture | 输入捕获、脉冲测量 |
| ADC | 单通道、多通道、定时器触发 |

## 6. Code Organization

各实验采用相对独立的 Keil 工程，尽可能保留原始学习过程中的工程结构。

主要文件类型：

| File Type | Purpose |
|---|---|
| `.c` | C 源代码 |
| `.h` | 头文件与函数声明 |
| `.s` | 汇编启动文件 |
| `.uvprojx` | Keil 工程文件 |
| `.uvoptx` | Keil 工程选项文件 |

部分实验包含以下目录：

- `my_lib/`：个人封装的外设驱动。
- `startup/`：STM32 启动文件。
- `std_periph_driver/`：标准外设库相关文件。
- `DebugConfig/`：Keil 调试配置。

## 7. Learning Outcomes

通过这一阶段的学习，我逐步建立了对 STM32 外设体系的整体认识：

1. 掌握 GPIO、USART、I2C、SPI 等基础外设的配置与使用。
2. 理解 STM32 中断系统以及 NVIC 的基本工作机制。
3. 理解 RCC 时钟树及各总线时钟的关系。
4. 掌握定时器时基、PWM 输出、输入捕获等功能。
5. 理解 ADC 转换流程及多通道扫描机制。
6. 初步掌握驱动代码封装、模块化组织与接口设计。

这部分学习为后续 FreeRTOS 和嵌入式 Linux 的学习打下了基础。

## 8. Acknowledgements

感谢 B 站 UP 主 **铁头山羊** 提供的 STM32 教学课程。

本仓库中的实验代码主要是在跟随课程学习过程中编写、调试和整理的，部分代码参考了课程中的设计思路。

本项目主要用于个人学习记录、技术积累与复习，不代表所有驱动均为独立原创实现。

---

**Keep Learning. Keep Building.**

