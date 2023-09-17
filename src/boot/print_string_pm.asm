; ================================
; - MP-DoS | Source
; ================================
; - Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
; - Файл: ./boot/print_string_pm.asm
; - Значение файла: Вывод строки на экран в 32-х битном защищённом режиме
; - Создан в: Kali Linux
; ================================


[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f		

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY	

print_string_pm_loop:
						
	mov al, [ebx]			
	mov ah, WHITE_ON_BLACK	
	cmp al, 0				
	je print_string_pm_done
	mov [edx], ax			
	add ebx, 1				
	add edx, 2
	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
