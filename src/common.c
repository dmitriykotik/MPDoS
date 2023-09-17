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

#include "common.h"

void	memcpy(u8 *src, u8 *dest, u32 bytes)
{
	u32 i;

	i = 0;
	while (i < bytes)
	{
		dest[i] = src[i];
		i++;
	}
}
