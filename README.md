LED灯接GPIO1_A0_d 32  
在板子上 insmod led_drv.ko 记得chmod +x 赋予权限  
./led_test on       # 点亮  
./led_test off      # 熄灭  
./led_test blink 5  # 闪烁 5 次  
