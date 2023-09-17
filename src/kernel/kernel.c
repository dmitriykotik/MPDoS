/* 
================================
- MP-DoS | Source
================================
- Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
- Файл: ./kernel/kernel.c
- Значение файла: Знакомьтесь, это само ядро MP-DoS. Писал его я сам :D
- Создан в: Kali Linux
================================
*/

#include "../common.h"
#include "../drivers/screen.h"
#include "./utils.h"

s32		kmain()
{	
	int act = 0;
	char *one = "hi";
	start_mpdos();
	kmessage("Welcome to MP-DoS!", 0xf0);
	kprint("\n\n");
	kwrite("MultiPlayer MP-DoS [Version 1.0/Alpha]\n(C) MultiPlayer, 2019.\n");
	if (terminal() == 55){
		notify_stop();
		asm volatile (
			"cli;"                 // Отключаем прерывания
			"hlt;"                 // Завершаем выполнение процессора
		);
		return 0;
	}
	clear_screen();
	kmessage("BOOT CONSOLE\n");
	act = 1;
	while (act){
		kprint("> ");
		if (strcmp(kread(),"write kernel")){
			kread_off();
			if (strcmp(kread(),"boot")){
				act = 0;
				reboot();
			}
		}
	}
	
	return 0;
}

