/**
 * @file battery_monitor.h
 * @author 宁子希 (1589326497@qq.com)
 * @brief       电池监测模块
 * @version 1.0.0
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

// ADC 配置
#define ADC_CHANNEL     ADC1_CHANNEL_2      //adc1 通道 2
#define ADC_ATTEN_DB    ADC_ATTEN_DB_2_5    // 衰减 2.5dB   0 mV ~ 1050 mV
#define ADC_WIDTH_BIT   ADC_WIDTH_BIT_12    // 12 位分辨率
#define ADC_UNIT        ADC_UNIT_1          // 单通道 ADC

// 电池配置
#define R1              300500.0f    // 分压电阻 1
#define R2              100400.0f    // 分压电阻 2
#define BATTERY_MIN     3.7f         // 电池最低电压 (V)
#define BATTERY_MAX     4.2f         // 电池最高电压 (V)
#define ADC_OFFSET      0.02f        // ADC 偏移电压 (V)
#define VOL_OFFSET      0.00f        // 电池补偿电压 (V)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取 ADC检测端的电压 (单位: V)
 * 
 */
float git_adc_voltage();

/**
 * @brief 初始化电池监测模块
 */
void battery_monitor_init();

/**
 * @brief 获取电池电压 (单位: V)
 * 
 * @return float 电池电压
 */
float battery_monitor_get_voltage();

/**
 * @brief 获取电池电量百分比
 * 
 * @return float 电池电量百分比 (0%-100%)
 */
float battery_monitor_get_percentage();

#ifdef __cplusplus
}
#endif

#endif // BATTERY_MONITOR_H

