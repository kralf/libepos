/* 
 * Copyright (C) 2005 Roland Philippsen <roland dot philippsen at gmx net>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */


#ifndef ASL_PDEBUG_H
#define ASL_PDEBUG_H

#include <stdio.h>

#define PDEBUG_ERR(fmt, arg...) fprintf(stderr, "%s(): " fmt, __func__, ## arg)
#define PDEBUG_SNIP_ERR(fmt, arg...) fprintf(stderr, fmt, ## arg)
#define PDEBUG_OFF(fmt, arg...)

#ifdef ASL_VERBOSE_DEBUG
# define ASL_DEBUG
# define PVDEBUG PDEBUG_ERR
# define PVDEBUG_SNIP PDEBUG_SNIP_ERR
#else
# define PVDEBUG PDEBUG_OFF
# define PVDEBUG_SNIP PDEBUG_OFF
#endif

#ifdef ASL_DEBUG
# define PDEBUG PDEBUG_ERR
# define PDEBUG_SNIP PDEBUG_SNIP_ERR
#else
# define PDEBUG PDEBUG_OFF
# define PDEBUG_SNIP PDEBUG_OFF
#endif

#endif // ASL_PDEBUG_H
