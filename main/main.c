
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

void PWM_Task(void *pvParameter);

#define PWM_RESOLUTION  LEDC_TIMER_12_BIT     // 12-bit PWM resolution
#define MAX_DUTY_CYCLE  4095                  // 2^PWM_RESOLUTION(12) - 1
#define PWM_CHANNEL_NUM  3

int PWM_GPIO_PINS[PWM_CHANNEL_NUM] = {12, 15, 25};           // GPIO 25 for PWM output

void setLight_with_255(int Red_old, int Blue_old, int Green_old) {
  int Red_new, Blue_new, Green_new; 
  
  Red_new = 4096 - (Red_old*16);       // newValue(~4096) = 4096 - (oldValue(~255) * 16)
  Blue_new = 4096 - (Blue_old*16);
  Green_new = 4096 - (Blue_old*16);


    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, Red_new);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, Blue_new);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, Green_new);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);

    vTaskDelay(pdMS_TO_TICKS(10));
}

void setLight(int x, int y, int z) {
    
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, x);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, y);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, z);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);

    vTaskDelay(pdMS_TO_TICKS(10));

}

void app_main(void)
{
    // Create PWM task
    xTaskCreate(PWM_Task, "PWM_Task", 2048, NULL, 5, NULL);
}

void PWM_Task(void *pvParameter)
{
    // Configure PWM timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .timer_num = LEDC_TIMER_0,          // Use Timer 0
        .duty_resolution = PWM_RESOLUTION,  // 12-bit duty cycle  (2^12 = 4096)
        .freq_hz = 5000,                    // 5 kHz PWM frequency
        .clk_cfg = LEDC_USE_APB_CLK         // Use APB clock for faster response
    };
    ledc_timer_config(&ledc_timer);

    // Configure PWM channel
    ledc_channel_config_t ledc_channels[PWM_CHANNEL_NUM] = {
        { .channel = LEDC_CHANNEL_0,          // Using Channel 
        .duty = 4096,                          // Start with 0 duty cycle
        .gpio_num = PWM_GPIO_PINS[0],               // GPIO for output pwm
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .hpoint = 0,                        // Default high point
        .timer_sel = LEDC_TIMER_0          // Use Timer 0
        },
        
        {
       .channel = LEDC_CHANNEL_1,          // Using Channel 
        .duty = 4096,                          // Start with 0 duty cycle
        .gpio_num = PWM_GPIO_PINS[1],               // GPIO for output pwm
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .hpoint = 0,                        // Default high point
        .timer_sel = LEDC_TIMER_0          // Use Timer 0      
        },
        {
       .channel = LEDC_CHANNEL_2,          // Using Channel 
        .duty = 4096,                          // Start with 0 duty cycle
        .gpio_num = PWM_GPIO_PINS[2],               // GPIO for output pwm
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .hpoint = 0,                        // Default high point
        .timer_sel = LEDC_TIMER_0          // Use Timer 0      
        }        
    };
    

     // initializing all channels
    for(int channel = 0; channel<PWM_CHANNEL_NUM;channel++) {
           ESP_ERROR_CHECK(ledc_channel_config(&ledc_channels[channel]));
    }

    for(;;) {
      // setLight(1968,3472,2000);     // setLight(Red, Blue, Green)   // newValue(~4096) = 4096 - (oldValue(~255) * 16)
      setLight_with_255(224,16,227);  // funtion to pass RGB values
    }
    //   for(int dutyCycle=0;dutyCycle<=MAX_DUTY_CYCLE;dutyCycle+=100) {
    //      setLight(dutyCycle,MAX_DUTY_CYCLE-dutyCycle,MAX_DUTY_CYCLE);
    //      vTaskDelay(pdMS_TO_TICKS(20));    
    //   }
    //   for(int dutyCycle=0;dutyCycle<=MAX_DUTY_CYCLE;dutyCycle+=100) {
    //      setLight(MAX_DUTY_CYCLE,dutyCycle,MAX_DUTY_CYCLE-dutyCycle);
    //      vTaskDelay(pdMS_TO_TICKS(20));    
    //   }

    //   for(int dutyCycle=0;dutyCycle<=MAX_DUTY_CYCLE;dutyCycle+=100) {
    //      setLight(MAX_DUTY_CYCLE-dutyCycle,MAX_DUTY_CYCLE,dutyCycle);
    //      vTaskDelay(pdMS_TO_TICKS(20));    
    //   }


/* **************************************************************************** */
        // for mapping 0-100 to 0-4095
        // dutyCycleValue = (dutyCycleFrom0to100 / 100.0) * MAX_DUTY_CYCLE
    
}