#include "protocol.h"
#include "app_timer.h"

#include "wholeconfig.h"

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
static void _proto_version_handle()
{
    pro_version_t pro_version;
    pro_version.fw_main_version = REVISION_NUM;
    pro_version.fw_build_version = REVISION_NUM_DEPUTY;
    pro_version.hw_version = REVISION_BOARD;
    pro_version.protocol_version = REVISION_PROTOCOL;
    pro_version.algo_version = REVISION_ARITH;
    pro_version.manufacturer_code = COMPANY_SHOES;
    pro_version.platform_code = PLATFORM_DEVICE;
    protocol_send(CMD_VERSION, (uint8_t *)(&pro_version), sizeof(pro_version_t));
}

static void _proto_battery_handle()
{
    pro_battery_t pro_battery;
    pro_battery.voltage_mv = battery_get_voltage();
    protocol_send(CMD_BATTERY, (uint8_t *)(&pro_battery), sizeof(pro_battery_t));
}

static void _proto_get_time_handle()
{
    pro_time_t pro_time;
    UTCTimeStruct time;
    UTCTime sec = Get_Clock();
    ConvertUTCTime( &time, sec);
    pro_time.year = time.year - 2000;
    pro_time.month = time.month;
    pro_time.day = time.day;
    pro_time.hour = time.hour;
    pro_time.minute = time.minutes;
    pro_time.second = time.seconds;
    protocol_send(CMD_GET_TIME, (uint8_t *)(&pro_time), sizeof(pro_time_t));
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
    UTCTimeStruct time;
    time.year = 2000 + parg->year;
    time.month = parg->month;
    time.day = parg->day;
    time.hour = parg->hour;
    time.minutes = parg->minute;
    time.seconds = parg->second;
    Set_Clock(ConvertUTCSecs(&time));
    flash_update_time();
    protocol_send(CMD_SET_TIME, b_TP_NULL, 0);
}

static void _proto_get_total_step_handle(pro_total_step_require_t *parg)
{
    pro_total_step_response_t pro_total_step_resp;
    total_step_info_t total_step;
    flash_get_total_step(parg->month, parg->day, &total_step);
    pro_total_step_resp.month = parg->month;
    pro_total_step_resp.day = parg->day;
    pro_total_step_resp.total_step = total_step.total_step;
    protocol_send(CMD_GET_TOTAL_STEP, (uint8_t *)(&pro_total_step_resp), sizeof(pro_total_step_response_t));
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
    heart_beat_clear();
    switch(protocol_info.cmd)
    {
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
            _proto_get_total_step_handle((pro_total_step_require_t *)protocol_info.param_buf);
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



















