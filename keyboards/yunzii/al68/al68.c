// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "uart.h"
#include "common/smart_ble.h"
#include "al68.h"
#include "ws2812.h"

void keyboard_pre_init_kb(void) {
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk);
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE; // diable JTAG (GD32 different to STM32)
    gpio_set_pin_output(A8);
    gpio_write_pin_high(A8); // ENABLE USB
    uart_init(460800);
    ws2812_init();
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        if ((keycode >= KC_BLE1) && (keycode <= KC_24G)) {
            wireless_start(keycode - KC_USB);
            return false;
        } else if (keycode == KC_USB) {
            wireless_stop();
            return false;
        }
    }
    return process_record_user(keycode, record);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, row: %2u, col: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.row, record->event.key.col, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    if (keycode == RM_NEXT) {
        rgb_matrix_step();
        uprintf("RGB Mode: %s\n", rgb_matrix_get_mode_name(rgb_matrix_get_mode()));
        rgb_matrix_step_reverse();
    }
#endif 
  return true;
}