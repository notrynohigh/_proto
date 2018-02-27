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


/** ------------------------------------------------------------------------------------------no param------ */
static void _proto_heart_handle()
{
    ;
}

static void _proto_version_handle()
{
    ;
}

static void _proto_battery_handle()
{
    ;
}

static void _proto_get_time_handle()
{
    ;
}

static void _proto_get_total_step_handle()
{
    ;
}

static void _proto_syn_xyz_handle()
{
    ;
}

static void _proto_rt_stop_handle()
{
    ;
}

static void _proto_chip_adjust_handle()
{
    ;
}

static void _proto_draw_wave_start_handle()
{
    ;
}

static void _proto_draw_wave_end_handle()
{
    ;
}

static void _proto_led_show_handle()
{
    ;
}

/** _____________________________________________________________________________________________with param________ */

static void _proto_set_time_handle(pro_time_t *parg)
{
    ;
}

static void _proto_syn_walk_handle(pro_syn_require_t *parg)
{
    ;
}

static void _proto_syn_run_handle(pro_syn_require_t *parg)
{
    ;
}

static void _proto_rt_start_handle(pro_rt_start_require_t *parg)
{
    ;
}

static void _proto_set_user_id_handle(pro_user_id_t *parg)
{
    ;
}

static void _proto_set_run_led_onoff_handle(pro_led_en_disable_t *parg)
{
    ;
}

static void _proto_set_led_show_onoff_handle(pro_led_en_disable_t *parg)
{
    ;
}

static void _proto_set_led_color_handle(pro_led_color_t *parg)
{
    ;
}

static void _proto_unsupport_cmd_handle(uint8_t cmd)
{
    ;
}
/** -------------------------------------------------------------------------------------------------------- */

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
            _proto_heart_handle();
            break;
        case CMD_VERSION:
            _proto_version_handle();
            break;
        case CMD_BATTERY:
            _proto_battery_handle();
            break;
        case CMD_SET_TIME:
            _proto_set_time_handle((pro_time_t *)protocol_info.param_buf);
            break;
        case CMD_GET_TIME:
            _proto_get_time_handle();
            break;
        case CMD_GET_TOTAL_STEP:
            _proto_get_total_step_handle();
            break;
        case CMD_SYN_WALK_DATA:
            _proto_syn_walk_handle((pro_syn_require_t *)protocol_info.param_buf);
            break;
        case CMD_SYN_RUN_DATA:
            _proto_syn_run_handle((pro_syn_require_t *)protocol_info.param_buf);
            break;  
        case CMD_SYN_XYZ:
            _proto_syn_xyz_handle();
            break;
        case CMD_RT_RUN_START:
            _proto_rt_start_handle((pro_rt_start_require_t *)protocol_info.param_buf);
            break;
        case CMD_RT_RUN_STOP:
            _proto_rt_stop_handle();
            break;
        case CMD_CHIP_ADJUST:
            _proto_chip_adjust_handle();
            break;
        case CMD_DRAW_WAVE_START:
            _proto_draw_wave_start_handle();
            break;
        case CMD_DRAW_WAVE_END:
            _proto_draw_wave_end_handle();
            break;
        case CMD_SET_USER_ID:
            _proto_set_user_id_handle((pro_user_id_t *)protocol_info.param_buf);
            break;
        case CMD_LED_SHOW:
            _proto_led_show_handle();
            break;  
        case CMD_SET_RUN_LED_ONOFF:
            _proto_set_run_led_onoff_handle((pro_led_en_disable_t *)protocol_info.param_buf);
            break;
        case CMD_SET_LED_SHOW_ONOFF:
            _proto_set_led_show_onoff_handle((pro_led_en_disable_t *)protocol_info.param_buf);
            break;
        case CMD_SET_LED_COLOR:
            _proto_set_led_color_handle((pro_led_color_t *)protocol_info.param_buf);
            break; 
        default:
            _proto_unsupport_cmd_handle(protocol_info.cmd);
            break;
    }
}



















