#include "protocol.h"
#include "app_timer.h"

APP_TIMER_DEF(proto_app_timer_id);
#define PROTO_TIMER_TICK    APP_TIMER_TICKS(100)
static uint8_t sg_timer_flag = 0;


static void _proto_timer_handler(void *parg)
{
    protocol_timer_handle();
}


static void _proto_timer_start()
{
    uint32_t err_code;
    if(sg_timer_flag == 0x1)
    {
        return;
    }    
    err_code = app_timer_start(proto_app_timer_id, PROTO_TIMER_TICK, NULL);
    APP_ERROR_CHECK(err_code);
    sg_timer_flag = 0x1;
}

static void _proto_timer_stop()
{
    uint32_t err_code;
    if(sg_timer_flag == 0x0)
    {
        return;
    }
    err_code = app_timer_stop(proto_app_timer_id);
    APP_ERROR_CHECK(err_code); 
    sg_timer_flag = 0x0;
}


void protocol_handle_initialize()
{
	uint32_t err_code;
    err_code = app_timer_create(&proto_app_timer_id, APP_TIMER_MODE_REPEATED, _proto_timer_handler);
    APP_ERROR_CHECK(err_code);
    protocol_initialize(_proto_timer_start, _proto_timer_stop);
}


void protocol_handle(protocol_info_t protocol_info)
{
    switch(protocol_info.cmd)
    {
        case CMD_HEART:
            proto_heart_handle();
            break;
        case CMD_VERSION:
            proto_version_handle();
            break;
        case CMD_BATTERY:
            proto_battery_handle();
            break;
        case CMD_SET_TIME:
            proto_set_time_handle((pro_time_t *)protocol_info.param_buf);
            break;
        case CMD_GET_TIME:
            proto_get_time_handle();
            break;
        case CMD_GET_TOTAL_STEP:
            proto_get_total_step_handle();
            break;
        case CMD_SYN_WALK_DATA:
            proto_syn_walk_handle((pro_syn_require_t *)protocol_info.param_buf);
            break;
        case CMD_SYN_RUN_DATA:
            proto_syn_run_handle((pro_syn_require_t *)protocol_info.param_buf);
            break;  
        case CMD_SYN_XYZ:
            proto_syn_xyz_handle();
            break;
        case CMD_RT_RUN_START:
            proto_rt_start_handle((pro_rt_start_require_t *)protocol_info.param_buf);
            break;
        case CMD_RT_RUN_STOP:
            proto_rt_stop_handle();
            break;
        case CMD_CHIP_ADJUST:
            proto_chip_adjust_handle();
            break;
        case CMD_DRAW_WAVE_START:
            proto_draw_wave_start_handle();
            break;
        case CMD_DRAW_WAVE_END:
            proto_draw_wave_end_handle();
            break;
        case CMD_SET_USER_ID:
            proto_set_user_id_handle((pro_user_id_t *)protocol_info.param_buf);
            break;
        case CMD_LED_SHOW:
            proto_led_show_handle();
            break;  
        case CMD_SET_RUN_LED_ONOFF:
            proto_set_run_led_onoff_handle((pro_led_en_disable_t *)protocol_info.param_buf);
            break;
        case CMD_SET_LED_SHOW_ONOFF:
            proto_set_led_show_onoff_handle((pro_led_en_disable_t *)protocol_info.param_buf);
            break;
        case CMD_SET_LED_COLOR:
            proto_set_led_color_handle((pro_led_color_t *)protocol_info.param_buf);
            break; 
        default:
            proto_unsupport_cmd_handle(protocol_info.cmd);
            break;
    }
}



















