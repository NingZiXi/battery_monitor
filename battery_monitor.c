/**
 * @file battery_monitor.c
 * @author 宁子希 (1589326497@qq.com)
 * @brief    电池监测模块
 * @version 1.0.0
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "battery_monitor.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "BatteryMonitor";


// 全局变量
static esp_adc_cal_characteristics_t *adc_char;

//初始化电池监测模块
void battery_monitor_init()
{
    // 配置 ADC 宽度和衰减
    adc1_config_width(ADC_WIDTH_BIT);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB);
    // 校准 ADC
    adc_char = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT, ADC_ATTEN_DB, ADC_WIDTH_BIT, ESP_ADC_CAL_VAL_DEFAULT_VREF, adc_char);

    ESP_LOGI(TAG, "Battery monitor initialized.");
}

//获取 ADC 电压
float git_adc_voltage(){
    // 多次采样求平均值
    const int sampling_count = 500;
    uint32_t sum = 0;

    for (int i = 0; i < sampling_count; i++) {
        sum += adc1_get_raw(ADC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(1)); // 延时 1ms
    }

    uint32_t raw_adc = sum / sampling_count;
    uint32_t adc_mv = esp_adc_cal_raw_to_voltage(raw_adc, adc_char); // 转换为 mV

    float adc_voltage = (float)adc_mv / 1000.0f + ADC_OFFSET; // 转换为 V 并校正

    return adc_voltage;
}

//获取电池电压 (单位: V)
float battery_monitor_get_voltage()
{
    float adc_voltage=git_adc_voltage();    // 获取 ADC 电压
    float battery_voltage = (R1 + R2) / R2 * adc_voltage + VOL_OFFSET; // 推算电池电压

    return battery_voltage;
}


//获取电池电量百分比
float battery_monitor_get_percentage()
{
    float voltage = battery_monitor_get_voltage();
    float percentage = ((voltage - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN)) * 100.0f;

    if (percentage < 0) {
        percentage = 0;
    } else if (percentage > 100) {
        percentage = 100;
    }

    return percentage;
}
