; ================================
; - MP-DoS | Source
; ================================
; - Автор: Плюха (Дмитрий Некраш) - https://www.youtube.com/@pluhaoff
; - Файл: ./boot/bootsect.asm
; - Значение файла: Загружает ядро в 32-х битном защищённом режиме
; - Создан в: Kali Linux
; ================================


[org 0x7c00]

KERNEL_OFFSET equ 0x1000

	mov [BOOT_DRIVE], dl

	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_string

	call load_kernel
	call switch_to_pm
	jmp $

%include "print_string.asm"
%include "print_hex.asm"
%include "disk_load.asm"
%include "print_string_pm.asm"
%include "switch.asm"
%include "gdt.asm"

[bits 16]

load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string
	mov bx, KERNEL_OFFSET
	mov dh, 16
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret


[bits 32]

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	call KERNEL_OFFSET
	jmp $


BOOT_DRIVE:			db 0
MSG_REAL_MODE:		db "", 0
MSG_PROT_MODE:		db "BIoS: The subsystem loading was successful! Please wait...", 0
MSG_LOAD_KERNEL:	db "Starting MP-DoS...", 0

times 510-($-$$) db 0
dw 0xaa55
