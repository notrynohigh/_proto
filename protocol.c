#include "protocol.h"
#include "b_tp.h"
#include "string.h"
#if 0
#define WEAK_FUNC    __weak
#else
#define WEAK_FUNC    __attribute__((weak))
#endif

static protocol_control_t sg_protocol_control;

static uint8_t sg_cnumber = 0;

#if PROTO_STATIC_BUF
static uint8_t sg_proto_buf[PROTO_STATIC_BUF_LEN]; 
#endif
/**
 * @defgroup TP_USER_INTERFACE
 * @{
 */

static void _protocol_tp_callback(uint8_t *pbuf, uint32_t len)
{
    if(pbuf == b_TP_NULL || len > PROTOCOL_BUF_LEN)
    {
        return;
    }
    sg_protocol_control.protocol_info.cmd = pbuf[0];
    memcpy(sg_protocol_control.protocol_info.param_buf, pbuf + 1, len - 1);
    sg_protocol_control.protocol_info.param_len = len - 1;
    sg_protocol_control.valid_data_flag = 0x1;
    sg_protocol_control.timer_start();
}

b_tp_err_code_t _b_tp_rec_check_head(b_tp_head_t *phead)
{
    sg_cnumber = phead->number;
    sg_protocol_control.timer_stop();
    return B_TP_SUCCESS;
}

void _b_tp_send_set_head(b_tp_head_t *phead)
{
    phead->number = sg_cnumber;
}


/**
 * @}
 */

WEAK_FUNC void protocol_handle(protocol_info_t protocol_info)
{
    ;
}

static void _protocol_dispatch(protocol_info_t protocol_info)
{
    // ...
    protocol_handle(protocol_info);
}


/**
 * @defgroup PRO_TIMER_OP
 * @{
 */

void protocol_timer_handle()
{
    if(sg_protocol_control.valid_data_flag == 0x1)
    {
        _protocol_dispatch(sg_protocol_control.protocol_info);
        sg_protocol_control.valid_data_flag = 0;
    }
    else
    {
        sg_protocol_control.timer_stop();
    }
}

/**
 * @}
 */


void protocol_initialize(pfunc timer_start, pfunc timer_stop)
{
    memset(&sg_protocol_control, 0, sizeof(protocol_control_t));
    sg_protocol_control.timer_start = timer_start;
    sg_protocol_control.timer_stop = timer_stop;
    b_tp_reg_callback(_protocol_tp_callback);
}




void protocol_send(uint8_t cmd, uint8_t *pbuf, uint32_t param_len)
{
    uint8_t *p = NULL;
    if(pbuf == NULL && param_len != 0)
    {
        return;
    }
#if PROTO_STATIC_BUF
    p = sg_proto_buf;
    p[0] = cmd;
    memcpy(p + 1, pbuf, param_len);
    b_tp_send_data(p, param_len + 1);
#else
    p = malloc(param_len + 1);
    if(p == NULL)
    {
        return;
    }
    p[0] = cmd;
    memcpy(p + 1, pbuf, param_len);
    b_tp_send_data(p, param_len + 1);
    free(p);
#endif
}











