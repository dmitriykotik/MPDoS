/* 
================================
- MP-DoS | Source
================================
- Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
- Файл: ./common.c
- Значение файла: Всякие удобные функции, константы, типы Ыыыыы)
- Создан в: Kali Linux
================================
*/

#ifndef COMMON_H
#define COMMON_H

typedef unsigned int	u32;
typedef 		 int	s32;
typedef unsigned short	u16;
typedef 		 short	s16;
typedef unsigned char	u8;
typedef 		 char	s8;

void	memcpy(u8 *src, u8 *dest, u32 bytes);

#endif
