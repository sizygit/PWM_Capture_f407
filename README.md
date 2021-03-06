# PWM_Capture_f407
PWM capture mode in STM32F407

## PWM输入捕获的应用（此例程为STM32F407系列核心板）

### 使用步骤

1. 配置好时钟频率，查询好所需要定时器的**时钟总线**和对应频率（TIM1　APB2 ）

2. 打开定时器，设置时钟源为 **Internal Clock**,打开Combined Channels 选择**PWM Input on CH1**,根据情况选择定时器CH1的实际引脚。

3. 配置定时器参数：设置分频为168-1（定时器频率为1000000 HZ）,向上计数模式，计数周期(自动重载值)为60000(一次周期采样上限为60000 ms，若考虑越界需要单独处理)，使能自动重装载。配置通道CH1为**上升沿**，CH2为**下降沿**,无分频。

4. 打开中断。

5. 打开freertos，新建一个弱定义的task,函数命名为　

   ```c++
   void StartPwmCapture(void const * argument)
   ```

6. 创建一个application文件夹，将capture_task的cpp及.h文件均加入后即可。
7. 可直接在**float duty　float period_ms**　这两个全局变量读出周期与占空比。

### 注意问题

1. 注意回调函数无需在.h文件中再次声明，检查回调函数是否进入。
2. 该程序用于一般电机或遥控器的PWM的检测,PWM采样周期**最大为60ms**,若需要更大范围的检测，需调整分频和做好定时器溢出回调函数的处理。
