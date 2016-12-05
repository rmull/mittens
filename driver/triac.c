/*
 * This file implements a zero-cross detector and triac driver.
 *
 * The zero-cross circuit provides a pulse whenever the AC waveform crosses
 * zero. When the pulse is detected, we need to start a one-shot timer with a
 * variable duration that depends on our application's AC power needs. When the
 * timer expires, we need to strobe the triac enable pin for an amount of time
 * that is long enough to latch the triac on, but we need to make sure it is
 * low by the time the next zero-crossing occurs or the triac will stay on. In
 * this manner, we "chop" the leading edge of the AC waveform and reduce the
 * power at the output of the triac.
 *
 * A half-cycle of 60Hz AC takes 8.33ms to elapse. Therefore, if we use a
 * 16-bit timer peripheral, a 4MHz count source would be ideal because an 8MHz
 * count source will max out at 8.19ms. With a 16-bit 4MHz we get 250ns
 * precision, and our timer load register should never be greater than 33,333.
 */

/* TODO:
 * Goal: No interrupts needed. Try:
 * GPIODMATriggerEnable(): Set the triac input pin to trigger a uDMA transaction
 *
 * Approach on Tiva: Use a pair of chained timers
 *
 */
#include "os/timer.h"

#include "triac.h"

void
triac_ctl(uint16_t target)
{
    /*
     * There are hard limits on the triac timer load value because we have to
     * leave enough time to unset the triac enable pin before the next zero
     * crossing.
     */

    if (target < (TRIAC_HALFWAVE_TICKS - TRIAC_LATCH_TICKS)) {
        timer_triac_port_set_load(target);
    }
}

void
triac_init(void)
{
    timer_triac_port_init();
    //timer_triac_port_start();
    //triac_ctl(10000);   /* TODO: Remove this */
}
