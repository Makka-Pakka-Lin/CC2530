
#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"

/**********************************************************************************************
*函数：void TIM1_PwmInit(uint16 period, uint8 ration)
*功能：输出正PWM拨，周期period毫秒，占空比为百分之ration
*输入：uint16 period-周期，单位：毫秒, uint8 ration-占空比，单位：%
*输出：无
*返回：无
*特殊说明：无
**********************************************************************************************/
void TIM1_PwmInit(uint16 period, uint8 ration)
{
    uint16 TimPeriod = 0;
    uint16 TimComp = 0;
//    CLKCONCMD &= ~0x40; //设置系统时钟源为32MHZ的晶振
//    while(CLKCONSTA & 0x40); //等待晶振稳定为32MHZ
//    CLKCONCMD &= ~0x07;     //设置系统主时钟频率为32MHZ
    CLKCONCMD |= 0x38;      //定时器标记输出为250KHZ

    //定时器通道设置
    P1SEL |= 0x01;         //定时器1通道2映射至P1_0，功能选择
    PERCFG |= 0x40;        //备用位置2，说明信息
    P2SEL &= ~0x10;        //相对于Timer4，定时器1优先
    P2DIR |= 0xC0;         //定时器通道2-3具有第一优先级
    P1DIR |= 0x01;

    //定时器模式设置
    T1CTL = 0x02;          //250KHZ不分频，模模式
    //此处P1_0口必须装定时器1通道2进行比较
    T1CCTL2 = 0x24;       //在向上比较清除输出。在0设置，到达比较值时清除输出
    //装定时器通道0初值
    TimPeriod = period*250;//周期TimPeriod毫秒，单位：ms
    T1CC0H = (uint8)(TimPeriod>>8);
    T1CC0L = (uint8)TimPeriod;        //PWM信号周期为1ms，频率为1KHZ
    //装定时器通道2比较值
    TimComp = ration*TimPeriod/100;//由占空比生成比较值
    T1CC2H = (uint8)(TimComp>>8);
    T1CC2L = (uint8)TimComp;
}
