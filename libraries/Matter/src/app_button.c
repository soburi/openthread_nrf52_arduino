/**
 * Copyright (c) 2012 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "sdk_common.h"
#include "app_button.h"
#include "app_timer.h"
#include "app_error.h"
#include "nrf_assert.h"
#include <Arduino.h>

#define NRF_LOG_MODULE_NAME app_button
#if APP_BUTTON_CONFIG_LOG_ENABLED
#define NRF_LOG_LEVEL       APP_BUTTON_CONFIG_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  APP_BUTTON_CONFIG_INFO_COLOR
#define NRF_LOG_DEBUG_COLOR APP_BUTTON_CONFIG_DEBUG_COLOR
#else //APP_BUTTON_CONFIG_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif //APP_BUTTON_CONFIG_LOG_ENABLED
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/*
 * For each pin state machine is used. Since GPIOTE PORT event is common for all pin is might be
 * missed. Module relies on interrupt from GPIOTE only to active periodic app_timer in which pin
 * is sampled. Timer is stopped when there is no active buttons (all buttons are in idle state).
 *
 * Transition to the new state is based on currently sampled button value. State machine has
 * following transitions:
 *
 * -----------------------------------------------------
 * | value | current state    | new state              |
 * |---------------------------------------------------|
 * |  0    | IDLE             | IDLE                   |
 * |  1    | IDLE             | PRESS_ARMED            |
 * |  0    | PRESS_ARMED      | IDLE                   |
 * |  1    | PRESS_ARMED      | PRESS_DETECTED         |
 * |  1    | PRESS_DETECTED   | PRESSED (push event)   |
 * |  0    | PRESS_DETECTED   | PRESS_ARMED            |
 * |  0    | PRESSED          | RELEASE_DETECTED       |
 * |  1    | PRESSED          | PRESSED                |
 * |  0    | RELEASE_DETECTED | IDLE (release event)   |
 * |  1    | RELEASE_DETECTED | PRESSED                |
 * -----------------------------------------------------
 *
 */
static app_button_cfg_t const *       mp_buttons = NULL;           /**< Button configuration. */
static uint8_t                        m_button_count;              /**< Number of configured buttons. */

static uint64_t m_pin_active;

#define BIT_PER_PIN 4
#define PINS 32*GPIO_COUNT

STATIC_ASSERT(BIT_PER_PIN == 4);

static uint8_t m_pin_states[PINS*BIT_PER_PIN/8];


typedef enum {
    BTN_IDLE,
    BTN_PRESS_ARMED,
    BTN_PRESS_DETECTED,
    BTN_PRESSED,
    BTN_RELEASE_DETECTED
} btn_state_t;


uint32_t btn_state;

void button_isr(uint32_t btnno)
{
	int current = digitalRead(btnno);
	if(current) {
		if(mp_buttons->active_state == APP_BUTTON_ACTIVE_HIGH) {
			btn_state |= (1<<btnno);
			mp_buttons->button_handler(btnno, APP_BUTTON_PUSH);
		}
		else {
			btn_state ^= (1<<btnno);
			mp_buttons->button_handler(btnno, APP_BUTTON_RELEASE);
		}
	}
	else {
		if(mp_buttons->active_state == APP_BUTTON_ACTIVE_HIGH) {
			btn_state ^= (1<<btnno);
			mp_buttons->button_handler(btnno, APP_BUTTON_RELEASE);
		}
		else {
			btn_state |= (1<<btnno);
			mp_buttons->button_handler(btnno, APP_BUTTON_PUSH);
		}
	}
}

void button0_isr() { button_isr(0); }
void button1_isr() { button_isr(1); }
void button2_isr() { button_isr(2); }
void button3_isr() { button_isr(3); }
void button4_isr() { button_isr(4); }
void button5_isr() { button_isr(5); }
void button6_isr() { button_isr(6); }
void button7_isr() { button_isr(7); }
void button8_isr() { button_isr(8); }
void button9_isr() { button_isr(9); }
void button10_isr() { button_isr(10); }
void button11_isr() { button_isr(11); }
void button12_isr() { button_isr(12); }
void button13_isr() { button_isr(13); }
void button14_isr() { button_isr(14); }
void button15_isr() { button_isr(15); }
void button16_isr() { button_isr(16); }
void button17_isr() { button_isr(17); }
void button18_isr() { button_isr(18); }
void button19_isr() { button_isr(19); }
void button20_isr() { button_isr(20); }
void button21_isr() { button_isr(21); }
void button22_isr() { button_isr(22); }
void button23_isr() { button_isr(23); }
void button24_isr() { button_isr(24); }
void button25_isr() { button_isr(25); }
void button26_isr() { button_isr(26); }
void button27_isr() { button_isr(27); }
void button28_isr() { button_isr(28); }
void button29_isr() { button_isr(29); }
void button30_isr() { button_isr(30); }
void button31_isr() { button_isr(31); }

uint32_t app_button_init(app_button_cfg_t const *       p_buttons,
                         uint8_t                        button_count,
                         uint32_t                       detection_delay)
{
	mp_buttons = p_buttons;
	for(int i=0; i<button_count; i++) {
		pinMode(p_buttons[i].pin_no, p_buttons[i].pull_cfg == NRF_GPIO_PIN_PULLUP ? INPUT_PULLUP : INPUT);
		switch(p_buttons[i].pin_no) {
			case 1: attachInterrupt(p_buttons[i].pin_no, button1_isr, CHANGE); break;
			case 2: attachInterrupt(p_buttons[i].pin_no, button2_isr, CHANGE); break;
			case 3: attachInterrupt(p_buttons[i].pin_no, button3_isr, CHANGE); break;
			case 4: attachInterrupt(p_buttons[i].pin_no, button4_isr, CHANGE); break;
			case 5: attachInterrupt(p_buttons[i].pin_no, button5_isr, CHANGE); break;
			case 6: attachInterrupt(p_buttons[i].pin_no, button6_isr, CHANGE); break;
			case 7: attachInterrupt(p_buttons[i].pin_no, button7_isr, CHANGE); break;
			case 8: attachInterrupt(p_buttons[i].pin_no, button8_isr, CHANGE); break;
			case 9: attachInterrupt(p_buttons[i].pin_no, button9_isr, CHANGE); break;
			case 10: attachInterrupt(p_buttons[i].pin_no, button10_isr, CHANGE); break;
			case 11: attachInterrupt(p_buttons[i].pin_no, button11_isr, CHANGE); break;
			case 12: attachInterrupt(p_buttons[i].pin_no, button12_isr, CHANGE); break;
			case 13: attachInterrupt(p_buttons[i].pin_no, button13_isr, CHANGE); break;
			case 14: attachInterrupt(p_buttons[i].pin_no, button14_isr, CHANGE); break;
			case 15: attachInterrupt(p_buttons[i].pin_no, button15_isr, CHANGE); break;
			case 16: attachInterrupt(p_buttons[i].pin_no, button16_isr, CHANGE); break;
			case 17: attachInterrupt(p_buttons[i].pin_no, button17_isr, CHANGE); break;
			case 18: attachInterrupt(p_buttons[i].pin_no, button18_isr, CHANGE); break;
			case 19: attachInterrupt(p_buttons[i].pin_no, button19_isr, CHANGE); break;
			case 20: attachInterrupt(p_buttons[i].pin_no, button20_isr, CHANGE); break;
			case 21: attachInterrupt(p_buttons[i].pin_no, button21_isr, CHANGE); break;
			case 22: attachInterrupt(p_buttons[i].pin_no, button22_isr, CHANGE); break;
			case 23: attachInterrupt(p_buttons[i].pin_no, button23_isr, CHANGE); break;
			case 24: attachInterrupt(p_buttons[i].pin_no, button24_isr, CHANGE); break;
			case 25: attachInterrupt(p_buttons[i].pin_no, button25_isr, CHANGE); break;
			case 26: attachInterrupt(p_buttons[i].pin_no, button26_isr, CHANGE); break;
			case 27: attachInterrupt(p_buttons[i].pin_no, button27_isr, CHANGE); break;
			case 28: attachInterrupt(p_buttons[i].pin_no, button28_isr, CHANGE); break;
			case 29: attachInterrupt(p_buttons[i].pin_no, button29_isr, CHANGE); break;
			case 30: attachInterrupt(p_buttons[i].pin_no, button30_isr, CHANGE); break;
			case 31: attachInterrupt(p_buttons[i].pin_no, button31_isr, CHANGE); break;
		}
	}
}

uint32_t app_button_enable(void)
{
	return 0;
}

