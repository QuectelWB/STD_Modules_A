


  #define configTICK_RATE_HZ                              1000
  /* 定义系统时钟节拍频率, 单位: Hz, 无默认需定义 */

  #define configUSE_PREEMPTION                            1                      
  /* 1: 抢占式调度器, 0: 协程式调度器, 无默认需定义 */

  #define configUSE_PORT_OPTIMISED_TASK_SELECTION         1                      
  /* 1: 使用硬件计算下一个要运行的任务（硬件）,
  0: 使用软件算法计算下一个要运行的任务（通用的方式）-效率较低, 默认: 0 
  特殊方法的效率高于通用方法，但是特殊方法依赖于一个或多个架构的汇编指令
  （一般是类似前导零[CLZ]的指令），因此特殊方法并不支持所有硬件，并且对任务优先级的最大值
  也有限制，通常为32*/


  #define configUSE_TICKLESS_IDLE                         0                       
  /* 1: 使能tickless低功耗模式, 默认: 0 */

  //#define configSYSTICK_CLOCK_HZ                          (configCPU_CLOCK_HZ / 8)
  /* 定义SysTick时钟频率，当SysTick时钟频率与内核时钟频率不同时才可以定义,
  单位: Hz, 默认: 不定义 */





  #define configSUPPORT_STATIC_ALLOCATION                 0
  /* 1: 支持静态申请内存, 默认: 0 */
  如果将 configSUPPORT_STATIC_ALLOCATION 设置为 1，用户 还 需 要 提 供 两 个 回 调 函 数 ： vApplicationGetIdleTaskMemory() 和 vApplicationGetTimerTaskMemory()，
  
  #define configSUPPORT_DYNAMIC_ALLOCATION                1
  /* 1: 支持动态申请内存, 默认: 1 */
  
  #define configTOTAL_HEAP_SIZE                           ((size_t)(10 * 1024))
  /* FreeRTOS堆中可用的RAM总量, 单位: Byte, 无默认需定义 */
  
  
  #define configAPPLICATION_ALLOCATED_HEAP                0
  /* 1: 用户手动分配FreeRTOS内存堆(ucHeap), 默认: 0 */


## 移植 

https://blog.csdn.net/tcjy1000/article/details/132330481


  

## FAQ

arm-none-eabi-ld: gcc/RTOSDemo.axf section `.bss' will not fit in region `SRAM'
arm-none-eabi-ld: region `SRAM' overflowed by 45952 bytes

需要修改MEMORY分配


undefined reference to `_sbrk‘

https://blog.csdn.net/jackcsdnfghdtrjy/article/details/124946338

https://blog.csdn.net/desert187/article/details/45720943

底层库在gcc中不支持




##参考


https://blog.csdn.net/zhangzhoulzl/article/details/124844622





https://blog.csdn.net/zywcxz/article/details/131520802



