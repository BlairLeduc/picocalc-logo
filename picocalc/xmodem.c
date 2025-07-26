#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

#define SOH  0x01  // Start of 128-byte data block
#define EOT  0x04  // End of transmission
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define C    0x43  // 'C' character to request CRC mode

#define BLOCK_SIZE 128

uint16_t crc16_ccitt(const uint8_t *data, int length) {
    uint16_t crc = 0;
    for (int i = 0; i < length; i++) {
        crc ^= ((uint16_t)data[i]) << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
            else crc <<= 1;
        }
    }
    return crc;
}

bool xmodem_receive(uint8_t *dest, size_t max_size) {
    int block_num = 1;
    size_t offset = 0;
    uart_putc(uart0, 'C');  // Start CRC mode

    while (true) {
        uint8_t header = 0;
        absolute_time_t timeout = make_timeout_time_ms(1000);
        while (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
            if (uart_is_readable(uart0)) {
                header = uart_getc(uart0);
                break;
            }
        }

        if (header == SOH) {
            uint8_t blk_num = uart_getc(uart0);
            uint8_t blk_inv = uart_getc(uart0);

            if ((blk_num ^ blk_inv) != 0xFF || blk_num != block_num) {
                uart_putc(uart0, NAK);
                continue;
            }

            uint8_t data[BLOCK_SIZE];
            for (int i = 0; i < BLOCK_SIZE; i++) {
                data[i] = uart_getc(uart0);
            }

            uint8_t crc_hi = uart_getc(uart0);
            uint8_t crc_lo = uart_getc(uart0);
            uint16_t crc_rx = (crc_hi << 8) | crc_lo;
            uint16_t crc_calc = crc16_ccitt(data, BLOCK_SIZE);

            if (crc_rx != crc_calc) {
                uart_putc(uart0, NAK);
                continue;
            }

            // Store data
            if (offset + BLOCK_SIZE <= max_size) {
                memcpy(&dest[offset], data, BLOCK_SIZE);
                offset += BLOCK_SIZE;
            } else {
                uart_putc(uart0, CAN);
                return false;
            }

            uart_putc(uart0, ACK);
            block_num++;
        }
        else if (header == EOT) {
            uart_putc(uart0, ACK);
            return true;
        }
        else if (header == CAN) {
            return false;  // Transmission canceled
        }
        else {
            uart_putc(uart0, NAK);  // Unknown header
        }
    }
}


bool xmodem_send(const uint8_t *data, size_t size) {
    // Wait for receiver to send 'C'
    absolute_time_t timeout = make_timeout_time_ms(3000);
    while (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
        if (uart_is_readable(uart0)) {
            if (uart_getc(uart0) == C)
                break;
        }
    }

    int block_num = 1;
    size_t offset = 0;
    uint8_t block[BLOCK_SIZE];

    while (offset < size) {
        size_t chunk = (size - offset >= BLOCK_SIZE) ? BLOCK_SIZE : (size - offset);
        memset(block, 0x1A, BLOCK_SIZE);  // Pad with Ctrl-Z (0x1A)
        memcpy(block, &data[offset], chunk);

        // Send SOH + block number + inverse
        uart_putc(uart0, SOH);
        uart_putc(uart0, block_num);
        uart_putc(uart0, 255 - block_num);

        // Send data
        for (int i = 0; i < BLOCK_SIZE; i++) {
            uart_putc(uart0, block[i]);
        }

        // Send CRC
        uint16_t crc = crc16_ccitt(block, BLOCK_SIZE);
        uart_putc(uart0, (crc >> 8) & 0xFF);
        uart_putc(uart0, crc & 0xFF);

        // Wait for ACK or NAK
        timeout = make_timeout_time_ms(3000);
        while (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
            if (uart_is_readable(uart0)) {
                uint8_t resp = uart_getc(uart0);
                if (resp == ACK) {
                    offset += BLOCK_SIZE;
                    block_num++;
                    break;
                } else if (resp == NAK) {
                    // Resend same block
                    break;
                } else if (resp == CAN) {
                    return false;
                }
            }
        }
    }

    // Send EOT
    uart_putc(uart0, EOT);
    // Wait for ACK after EOT
    timeout = make_timeout_time_ms(2000);
    while (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
        if (uart_is_readable(uart0) && uart_getc(uart0) == ACK) {
            return true;
        }
    }

    return false;
}