/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Luca Weiss <luca@z3ntu.xyz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include "razerreport.h"

/**
 * Calculate the checksum for the usb message
 *
 * Checksum byte is stored in the 2nd last byte in the messages payload.
 * The checksum is generated by XORing all the bytes in the report starting
 * at byte number 2 (0 based) and ending at byte 88.
 */
unsigned char razer_calculate_crc(razer_report *report)
{
    /*second to last byte of report is a simple checksum*/
    /*just xor all bytes up with overflow and you are done*/
    unsigned char crc = 0;
    unsigned char *_report = (unsigned char *) report;

    unsigned int i;
    for (i = 3; i < 89; i++) {
        crc ^= _report[i];
    }

    return crc;
}

razer_report get_razer_report(unsigned char command_class, unsigned char command_id, unsigned char data_size)
{
    razer_report new_report;
    memset(&new_report, 0, sizeof(razer_report));

    new_report.status = 0x00;
    new_report.transaction_id.id = 0xFF;
    new_report.remaining_packets = 0x00;
    new_report.protocol_type = 0x00;
    new_report.command_class = command_class;
    new_report.command_id.id = command_id;
    new_report.data_size = data_size;

    return new_report;
}

razer_report razer_chroma_standard_set_led_brightness(RazerVarstore variable_storage, RazerLedId led_id, unsigned char brightness)
{
    razer_report report = get_razer_report(0x03, 0x03, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = brightness;

    return report;
}

razer_report razer_chroma_standard_get_led_brightness(RazerVarstore variable_storage, RazerLedId led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x83, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_set_led_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicEffectId led_effect)
{
    struct razer_report report = get_razer_report(0x03, 0x02, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = led_effect;

    return report;
}

razer_report razer_chroma_standard_get_led_effect(RazerVarstore variable_storage, RazerLedId led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x82, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_set_led_rgb(RazerVarstore variable_storage, RazerLedId led_id, unsigned char red, unsigned char green, unsigned char blue)
{
    struct razer_report report = get_razer_report(0x03, 0x01, 0x05);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = red;
    report.arguments[3] = green;
    report.arguments[4] = blue;

    return report;
}

razer_report razer_chroma_standard_get_led_rgb(RazerVarstore variable_storage, RazerLedId led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x81, 0x05);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    return report;
}

razer_report razer_chroma_standard_set_led_state(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicLedState led_state)
{
    struct razer_report report = get_razer_report(0x03, 0x00, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = led_state;

    return report;
}

razer_report razer_chroma_standard_get_led_state(RazerVarstore variable_storage, RazerLedId led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x80, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_get_firmware_version()
{
    return get_razer_report(0x00, 0x81, 0x02);
}

razer_report razer_chroma_standard_get_serial()
{
    return get_razer_report(0x00, 0x82, 0x16);
}

razer_report razer_chroma_standard_get_device_mode()
{
    return get_razer_report(0x00, 0x84, 0x02);
}

razer_report razer_chroma_standard_get_keyboard_layout()
{
    return get_razer_report(0x00, 0x86, 0x02);
}

razer_report razer_chroma_standard_matrix_effect(RazerMatrixEffectId effect)
{
    struct razer_report report = get_razer_report(0x03, 0x0A, 80);
    report.arguments[0] = static_cast<uchar>(effect); // Effect ID

    return report;
}

razer_report razer_chroma_extended_mouse_matrix_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerMouseMatrixEffectId effect)
{
    struct razer_report report = get_razer_report(0x0F, 0x02, 80);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = static_cast<uchar>(effect);

    return report;
}
