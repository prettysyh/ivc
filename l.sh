# !/bin/bash
aarch64-linux-gnu-gcc -o test.elf start.s exception.s gic.c aaa_print.c aaa_test.c aaa_handler.c aaa_timer.c ivc_echo.c hsp.c -Tlink.lds -nostartfiles

aarch64-linux-gnu-objcopy -O binary test.elf test.bin

mkimage -A arm64  -C none -T kernel -a 0x80100000 -e 0x80100000 -d test.bin uImage

scp uImage syh@192.168.2.80:/home/syh/boot-image