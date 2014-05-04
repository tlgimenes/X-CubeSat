/* X-CubeSat Controller: Real-time communication with satellite program

 Copyright (C)  2014 - Tiago Lobato Gimenes

 Authors: Tiago Lobato Gimenes <tlgimenes@gmail.com>

 Comments, questions and bugreports should be submitted via
 https://github.com/tlgimenes/X-CubeSat
 More details can be found at the project home page:

 https://github.com/tlgimenes/X-CubeSat

 This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program; if not, visit http://www.fsf.org/
*/

#ifndef MODEMS_HPP
#define MODEMS_HPP

#define KANTRONICS /* Will use the KANTRONICS modem */

#ifdef STANDARD_MODEM
#define OEM         "\r"
#define REPLY_OEM   "\r\n"

#elif defined KANTRONICS /* If using the KANTRONICS modem */
#define OEM         "\r"
#define REPLY_OEM   "\r\n"

#endif /* STANDARD_MODEM */

#endif /* MODEMS_HPP */
