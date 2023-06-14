#include "ec11_api.h"
#include <stdint.h>
#include "nrfx_qdec.h"

#include "events.h"
#include "keyboard_evt.h"

#include "config.h"

int16_t p_acc;
uint16_t p_accdbl;

void m_QDEC_interrupt(nrfx_qdec_event_t event)
{
    switch (event.type)
    {
    case NRF_QDEC_EVENT_REPORTRDY:
        nrfx_qdec_disable();
        p_acc = event.data.report.acc;
        p_accdbl = event.data.report.accdbl;

        nrfx_qdec_enable();
        if (p_accdbl == 0)
        {
            if (p_acc < 0)
            {
                keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_ROTATE_LEFT);
            }
            else
            {
                keyboard_evt_trigger_param(KBD_EVT_ENCODER, EC_ROTATE_RIGHT);
            }
        }
        break;
    default:
        break;
    }
}

void QDEC_init(void)
{
    nrfx_qdec_config_t p_config = {
        .dbfen = true,
        .interrupt_priority = 3,
        .ledpol = NRF_QDEC_LEPOL_ACTIVE_LOW,
        .ledpre = 0,
        .psela = EC11_A_PIN,
        .pselb = EC11_B_PIN,
        .pselled = 0xffffffff,
        .reportper = NRF_QDEC_REPORTPER_80,
        .sampleper = NRF_QDEC_SAMPLEPER_2048us,
        .sample_inten = 0};

    nrfx_qdec_init(&p_config, m_QDEC_interrupt);

    nrfx_qdec_enable();
}

void QDEC_uninit()
{
    nrfx_qdec_uninit();
}