#ifndef RTOS_SETTINGS_H
#define RTOS_SETTINGS_H

#define PRINT_DEBUG true

#define HANDLE_STOP_PRIORITY        1
#define PUMP_PRIORITY               2
#define MEASURE_MOTOR_PRIORITY      3
#define MEASURE_WATER_PRIORITY      4
#define HANDLE_AUTOMATIC_PRIORITY   5
#define HANDLE_MANUAL_PRIORITY      6
#define LCD_PRIORITY                7
#define UPLOAD_STATUS_PRIORITY      8

#define MEASURE_WATER_PERIOD    500
#define MEASURE_MOTOR_PERIOD    2000
#define PUMP_PERIOD             500
#define UPLOAD_STATUS_PERIOD    2
#define LCD_PERIOD              1000

#endif