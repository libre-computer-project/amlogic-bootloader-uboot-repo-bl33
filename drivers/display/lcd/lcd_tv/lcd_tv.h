
/*
 * drivers/display/lcd/lcd_tv/lcd_tv.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/

#ifndef __AML_LCD_TV_H__
#define __AML_LCD_TV_H__
#include <amlogic/aml_lcd.h>

//**********************************
//lcd driver version
//**********************************
#define LCD_DRV_TYPE      "tv"

#define LCD_DRV_VERSION    "20151105"
//**********************************

extern int lvds_init(struct lcd_config_s *pconf);
extern int lvds_disable(struct lcd_config_s *pconf);
extern int vbyone_init(struct lcd_config_s *pconf);
extern int vbyone_disable(struct lcd_config_s *pconf);

extern void set_vbyone_config(struct lcd_config_s *pconf);

#endif