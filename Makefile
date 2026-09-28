# ============================================================
# Luckfox Pico (RV1106) LED 驱动 + 测试程序 Makefile
# ============================================================

# ---- 交叉工具链 ----
CROSS_COMPILE := /home/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-
CC            := $(CROSS_COMPILE)gcc

# ---- 内核源码目录 ----
KERNEL_DIR    := /home/luckfox-pico/sysdrv/source/kernel
ARCH          := arm

# ---- 当前目录 ----
PWD := $(shell pwd)

# ---- 目标文件 ----
obj-m := led_drv.o

# ============================================================
# 默认目标：编译驱动 + 用户态程序
# ============================================================
all: modules led_test

# ---- 编译内核模块 ----
modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# ---- 编译用户态测试程序 ----
led_test: led_test.c
	$(CC) -Wall -O2 -o $@ $<

# ---- 清理 ----
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	rm -f led_test

# ---- 安装模块到板子（需要 adb 已连接）----
install: all
	adb push led_drv.ko /oem/usr/ko/
	adb push led_test    /oem/usr/bin/
	@echo "Push done. On board run:"
	@echo "  insmod /oem/usr/ko/led_drv.ko"
	@echo "  /oem/usr/bin/led_test on"

# ---- 卸载模块 ----
uninstall:
	adb shell "rmmod led_drv" || true

.PHONY: all modules clean install uninstall led_test
