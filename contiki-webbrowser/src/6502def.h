/*
 * Copyright (c) 2007, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Oliver Schmidt <ol.sc@web.de>
 *
 */

#ifndef S502DEF_H_
#define S502DEF_H_

#include <ctype.h>
#include <conio.h>
#include <stdint.h>
#include <unistd.h>

#define CC_CONF_REGISTER_ARGS 1

#define CCIF
#define CLIF

#define CLOCK_CONF_SECOND 4
typedef unsigned short clock_time_t;

#define LOADER_CONF_ARCH "unload.h"

#define LOG_CONF_ENABLED 0

#define CTK_CONF_WIDGET_FLAGS  0
#define CTK_CONF_WINDOWS       0
#define CTK_CONF_WINDOWMOVE    0
#define CTK_CONF_WINDOWCLOSE   0
#define CTK_CONF_ICONS         0
#define CTK_CONF_MENUS         0
#define CTK_CONF_SCREENSAVER   0
#define CTK_CONF_MOUSE_SUPPORT 1

#define ctk_arch_keyavail kbhit
#define ctk_arch_getkey   cgetc
#define ctk_arch_isprint  isprint

#endif /* S502DEF_H_ */
