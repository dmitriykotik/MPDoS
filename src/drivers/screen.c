/* 
================================
- MP-DoS | Source
================================
- Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
- Файл: ./drivers/screen.c
- Значение файла: Отвечает за стандартную структуру ядра (ну тоесть не большие функции :D)
- Создан в: Kali Linux
================================
*/

#include "screen.h"
#include "lowlevel_io.h"
#include "../common.h"

int strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0; // Строки не равны
        }
        str1++;
        str2++;
    }
    // Если одна строка закончилась, а другая нет, они не равны
    if (*str1 != '\0' || *str2 != '\0') {
        return 0;
    }
    return 1; // Строки равны
}

void ksleep(int seconds) {
    int counter;
    for (counter = 0; counter < seconds; counter++) {
    }
}

int active = 0;
char input_buffer[256];
u8 buffer_index = 0;

char kbdus[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',
    0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0,
    0, 0, 0,
    0,
    0, 0, 0,
    0, 0, 0,
    0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0,
    0,
};

void init_keyboard() {
    port_byte_out(0x64, 0xAE);
    port_byte_out(0x64, 0x20);
    u8 status = port_byte_in(0x60);
    status |= 0x01;
    status &= ~0x02;
    port_byte_out(0x64, 0x60);
    port_byte_out(0x60, status);
}

char read_keyboard() {
    while (1) {
        if (port_byte_in(0x64) & 0x01) {
            char scancode = port_byte_in(0x60);
            if (scancode < 128) {
                return kbdus[scancode];
            }
        }
    }
}

void clear_char(u16 offset) {
    write(' ', WHITE_ON_BLACK, offset);
    set_cursor(offset);
}


void	kprint_colored(u8 *str, u8 attr)
{
	while (*str)
	{
		if (*str == '_')
			putchar(*str, 0x00);	
		else
			putchar(*str, attr);
		str++;
	}
}

void	kwrite(u8 *str)
{
	while (*str)
	{
		if (*str == '_')
			putchar(*str, 0x00);	
		else
			putchar(*str, 0x7);
		str++;
	}
}

void	kmessage(u8 *str)
{
	while (*str)
	{
		if (*str == '_')
			putchar(*str, 0x00);	
		else
			putchar(*str, 0xf0);
		str++;
	}
}

void	kprint(u8 *str)
{
	/* Функция печати строки */
	
	// u8 *str: указатель на строку (на первый символ строки). Строка должна
	// быть null-terminated.

	while (*str)
	{
		putchar(*str, WHITE_ON_BLACK);
		str++;
	}
}

void	putchar(u8 character, u8 attribute_byte)
{
	/* Более высокоуровневая функция печати символа */

	// u8 character: байт, соответствующий символу
	// u8 attribute_byte: байт, соответствующий цвету текста/фона символа

	u16 offset;

	offset = get_cursor();
	if (character == '\n')
	{
		// Переводим строку.
		if ((offset / 2 / MAX_COLS) == (MAX_ROWS - 1)) 
			scroll_line();
		else
			set_cursor((offset - offset % (MAX_COLS*2)) + MAX_COLS*2);
	}
	else 
	{
		if (offset == (MAX_COLS * MAX_ROWS * 2)) scroll_line();
		write(character, attribute_byte, offset);
		set_cursor(offset+2);
	}
}

void	scroll_line()
{
	/* Функция скроллинга */

	u8 i = 1;		// Начинаем со второй строки.
	u16 last_line;	// Начало последней строки.

	while (i < MAX_ROWS)
	{
		memcpy(
			(u8 *)(VIDEO_ADDRESS + (MAX_COLS * i * 2)),
			(u8 *)(VIDEO_ADDRESS + (MAX_COLS * (i-1) * 2)),
			(MAX_COLS*2)
		);
		i++;
	}

	last_line = (MAX_COLS*MAX_ROWS*2) - MAX_COLS*2;
	i = 0;
	while (i < MAX_COLS)
	{
		write('\0', WHITE_ON_BLACK, (last_line + i * 2));
		i++;
	}
	set_cursor(last_line);
}

void	clear_screen()
{
	/* Функция очистки экрана */

	u16	offset = 0;
	while (offset < (MAX_ROWS * MAX_COLS * 2))
	{
		write('\0', WHITE_ON_BLACK, offset);
		offset += 2;
	}
	set_cursor(0);
}

void	write(u8 character, u8 attribute_byte, u16 offset)
{
	/* Функция печати символа на экран с помощью VGA по адресу 0xb8000 */

	// u8 character: байт, соответствующий символу
	// u8 attribute_byte: байт, соответствующий цвету текста/фона символа
	// u16 offset: смещение (позиция), по которому нужно распечатать символ
	
	u8 *vga = (u8 *) VIDEO_ADDRESS;
	vga[offset] = character;
	vga[offset + 1] = attribute_byte;
}

u16		get_cursor()
{
	/* Функция, возвращающая позицию курсора (char offset). */

	port_byte_out(REG_SCREEN_CTRL, 14);				// Запрашиваем верхний байт
	u8 high_byte = port_byte_in(REG_SCREEN_DATA);	// Принимаем его
	port_byte_out(REG_SCREEN_CTRL, 15);				// Запрашиваем нижний байт
	u8 low_byte = port_byte_in(REG_SCREEN_DATA);	// Принимаем и его
	// Возвращаем смещение умножая его на 2, т.к. порты возвращают смещение в
	// клетках экрана (cell offset), а нам нужно в символах (char offset), т.к.
	// на каждый символ у нас 2 байта
	return (((high_byte << 8) + low_byte) * 2);
}

void	set_cursor(u16 pos)
{
	/* Функция, устаналивающая курсор по смещнию (позиции) pos */
	/* Поиграться с битами можно тут http://bitwisecmd.com/ */

	// конвертируем в cell offset (в позицию по клеткам, а не символам)
	pos /= 2;

	// Указываем, что будем передавать верхний байт
	port_byte_out(REG_SCREEN_CTRL, 14);
	// Передаем верхний байт
	port_byte_out(REG_SCREEN_DATA, (u8)(pos >> 8));
	// Указываем, что будем передавать нижний байт
	port_byte_out(REG_SCREEN_CTRL, 15);
	// Передаем нижний байт
	port_byte_out(REG_SCREEN_DATA, (u8)(pos & 0xff));
}

char *kread(){
	active = 1;
	init_keyboard();
    	while (active) {
       		char key = read_keyboard();
        	if (key == '\n') {
            		input_buffer[buffer_index] = '\0';
            		buffer_index = 0;
            		active = 0;
            		kprint("\n");
            		return(input_buffer);
        	} 
        	else if (key == '\b') {
            		if (buffer_index > 0) {
                		buffer_index--;
                		u16 offset = get_cursor();
                		offset -= 2;
                		clear_char(offset);
                		input_buffer[buffer_index] = '\0';
            		}
        	} 
        	else if (key != 0) {
            		if (buffer_index < sizeof(input_buffer) - 1) {
                		input_buffer[buffer_index] = key;
                		buffer_index++;
                		putchar(key, WHITE_ON_BLACK);
            		}
        	}
    	}
}

void kread_off(){
	active = 0;
	buffer_index = 0;

}
