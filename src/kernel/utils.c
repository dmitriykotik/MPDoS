/* 
================================
- MP-DoS | Source
================================
- Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
- Файл: ./drivers/utils.c
- Значение файла: Не большие утилиты
- Создан в: Kali Linux
================================
*/

#include "./utils.h"
#include "../drivers/screen.h"
#include "../common.h"

int active_terminal = 1;

void start_mpdos(){
	clear_screen();
	kmessage("Starting MP-DoS---");
	ksleep(100000000);
	clear_screen();
	kmessage("Starting MP-DoS.--");
	ksleep(100000000);
	clear_screen();
	kmessage("Starting MP-DoS..-");
	ksleep(100000000);
	clear_screen();
	kmessage("Starting MP-DoS...");
	ksleep(100000000);
	clear_screen();
	kmessage("Starting MP-DoS---");
	ksleep(100000000);
	clear_screen();
	kmessage("Starting MP-DoS.--");
	ksleep(100000000);
	kprint(" DONE");
	ksleep(150000000);
	clear_screen();
}

void exit(){
	active_terminal = 0;
}

void reboot(){
	asm volatile (
			"cli;"                 // Отключаем прерывания
			"mov $0x0600, %ax;"    // Загружаем значение 0x0600 в регистр AX
			"mov $0x0001, %bx;"    // Загружаем значение 0x0001 в регистр BX
			"int $0x15;"           // Вызываем прерывание 0x15 с указанными параметрами
			"hlt;"                 // Завершаем выполнение процессора
		);
	
}
int poweroff(){
	active_terminal = 0;
	return 55;
}

void notify_stop(){
	clear_screen();
	kprint_colored("The kernel has stopped. You can now turn off your computer.", 0x4f);
}

int terminal(){
	active_terminal = 1;
	while (active_terminal){
		kprint("\nuser@mpdos:~$ ");
		char *input = kread();
		if (strcmp(input, "hi") == 1){
			kwrite("hello!");
		}
		else if (strcmp(input, "exit") == 1){
			kread_off();
			exit();
		}
		else if (strcmp(input, "reboot") == 1){
			reboot();
		}
		else if (strcmp(input, "poweroff") == 1){
			poweroff();
			return 55;
		}
		else{
			kprint_colored(input, 0x04);
			kprint_colored(": command not found", 0x04);
		}
	}
	return 0;
}
