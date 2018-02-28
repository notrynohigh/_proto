#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stdint.h"


/**
 * @defgroup PRO_CMD_LIST 
 * @{
 */

#define CMD_HEART               0X00
#define CMD_VERSION             0X01
#define CMD_BATTERY             0X02
#define CMD_SET_TIME            0X03
#define CMD_GET_TIME            0X04
#define CMD_GET_TOTAL_STEP      0X05
#define CMD_SYN_WALK_DATA       0X06
#define CMD_SYN_RUN_DATA        0X07
#define CMD_SYN_XYZ             0X08
#define CMD_RT_RUN_START        0X09
#define CMD_RT_RUN_STOP         0X10
#define CMD_CHIP_ADJUST         0X11
#define CMD_DRAW_WAVE_START     0X12
#define CMD_DRAW_WAVE_END       0X13
#define CMD_SET_USER_ID         0X14
#define CMD_LED_SHOW            0X15
#define CMD_SET_RUN_LED_ONOFF   0X16
#define CMD_SET_LED_SHOW_ONOFF  0X17
#define CMD_SET_LED_COLOR       0X18



#define CMD_REBOOT              0X81
#define CMD_READ_FLASH          0X82
#define CMD_ERASE_FLASH         0X83
#define CMD_GET_REBOOT_TIME     0X84
#define CMD_LOG_ONOFF           0X85
#define CMD_TRANSFER_LOG        0X86


#define CMD_ACK                 0X99

/**
 * @}
 */
 
/**
 * @defgroup CMD_PARAM_STRUCT
 * @{
 */
#pragma pack(1)

/** CMD_VERSION */
typedef struct
{
    uint16_t fw_main_version;
    uint16_t fw_build_version;
    uint8_t  platform_code;
    uint8_t  manufacturer_code;
    uint16_t hw_version;
    uint16_t algo_version;
    uint16_t protocol_version;
}pro_version_t;

/** CMD_BATTERY */
typedef struct
{
    uint16_t voltage_mv; 
}pro_battery_t;


/** CMD_SET_USER_ID */
typedef struct
{
    uint32_t user_id; 
}pro_user_id_t;


/** CMD_SET/GET_TIME */
typedef struct
{
    uint8_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
}pro_time_t;


/** CMD_GET_TOTAL_STEP */
typedef struct
{
    uint8_t  month;
    uint8_t  day;
}pro_total_step_require_t;


typedef struct
{
    uint8_t  month;
    uint8_t  day;
    uint32_t total_step;
}pro_total_step_response_t;


typedef struct
{
    uint8_t  month;
    uint8_t  day;
    uint8_t  s_hour;
    uint8_t  s_minute;
    uint8_t  e_hour;
    uint8_t  e_minute;
}pro_syn_require_t;

/** CMD_SYN_WALK_DATA */
typedef struct
{
    uint8_t  hour;
    uint8_t  minute;
    uint16_t total_step;
    uint16_t slow_walk_step;
    uint16_t fast_walk_step;
    uint16_t run_step;
    uint16_t inside_step;
    uint16_t outside_setp;
    uint16_t normal_step;
}pro_walk_info_t;


typedef struct
{
    uint8_t  month;
    uint8_t  day;
    uint8_t  complete_flag;
    pro_walk_info_t walk_info[1];
}pro_syn_walk_response_t;



/** CMD_SYN_RUN_DATA */
typedef struct
{
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  run_step;
    uint8_t  forefoot_setp;
    uint8_t  midfoot_setp;
    uint8_t  backfoot_setp;
    uint8_t  inside_setp;
    uint8_t  normal_setp;
    uint8_t  outside_setp;
    uint16_t height_cm;
    uint16_t force_g;
    uint16_t land_time_ms;
    uint16_t float_time_ms;
}pro_run_info_t;


typedef struct
{
    uint8_t  month;
    uint8_t  day;
    uint8_t  complete_flag;
    pro_run_info_t run_info[1];
}pro_syn_walk_response_t;


/** CMD_RT_RUN_START */
typedef struct
{
    uint8_t param_reset;
}pro_rt_start_require_t;


typedef struct
{
    uint32_t total_step;
    uint32_t slow_walk_step;
    uint32_t fast_walk_step;
    uint32_t run_step;
    uint32_t walk_inside_step;
    uint32_t walk_normal_step;
    uint32_t walk_outside_step;
    uint32_t run_forefoot_step;
    uint32_t run_midfoot_step;
    uint32_t run_backfoot_step;
    uint32_t run_inside_step;
    uint32_t run_outside_step;
    uint16_t height_cm;
    uint16_t force_g;
    uint16_t land_time_ms;
    uint16_t float_time_ms;
}pro_rt_info_t;

typedef struct
{
    uint8_t flag;
    pro_rt_info_t rt_info;
}pro_rt_detail_response_t;

typedef struct
{
    uint8_t  flag;
    uint32_t total_step;
}pro_rt_simple_response_t;


/** CMD_CHIP_ADJUST */
typedef struct
{
    uint8_t status;
}pro_adjust_t;

/** CMD_DRAW_WAVE_START */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
}pro_xyz_info_t;

typedef struct
{
    pro_xyz_info_t xyz_info;
}pro_xyz_t;


/** CMD_SET_RUN_LED_ONOFF */
typedef struct
{
    uint8_t status;
}pro_led_en_disable_t;


/** CMD_SET_LED_COLOR */
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}pro_led_color_t;


#pragma pack()

/**
 * @}
 */

typedef void(*pfunc)(void);
#define PROTOCOL_BUF_LEN      20
#define PROTO_STATIC_BUF      1
#define PROTO_STATIC_BUF_LEN  128

typedef struct
{
    uint8_t cmd;
    uint8_t param_len;
    uint8_t param_buf[PROTOCOL_BUF_LEN];
}protocol_info_t;

typedef struct
{
    uint8_t cmd;
    uint8_t param[1];
}protocol_struct_t;

typedef struct
{
    uint8_t         valid_data_flag;
    pfunc           timer_start;
    pfunc           timer_stop;
    protocol_info_t protocol_info;
}protocol_control_t;




void protocol_timer_handle(void);
void protocol_handle(protocol_info_t );
void protocol_initialize(pfunc timer_start, pfunc timer_stop);
void protocol_send(uint8_t cmd, uint8_t *pbuf, uint32_t param_len);


void protocol_handle_initialize(void);

#endif




