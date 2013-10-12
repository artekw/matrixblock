/**
 * @file boneheader.h
 *
 * author https://github.com/millerap/ECE497/blob/master/BoneHeader.h
 * rewrite by Artur Wronowski
 *
 * This file contains the headers for boneheader.c
 *
 * Licensed under the MIT License (MIT)
 * See MIT-LICENSE file for more information
 */


#ifndef BONEHEADER_H
#define BONEHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 127

int export_gpio(int gpio);
int is_exported_gpio(int gpio);
int unexport_gpio(int gpio);
int set_gpio_direction(int gpio, const char* direction);
int set_gpio_value(int gpio, int value);
int set_gpio_edge(int gpio, char* edge);
int gpio_fd_open(int gpio);
int gpio_fd_close(int fd);
int set_mux_value(char* mux, int value);
int read_ain(char* ain);
int set_pwm(char* pwm, int period_freq, int duty_percent);
int unset_pwm(char* pwm);

#endif /* BONEHEADER_H */
