## [SPI-0.96-inch-OLED-Driver-Library-base-on-STM32F103ZET6](https://github.com/whik/SPI-0.96-inch-OLED-Driver-Library-base-on-STM32F103ZET6)
---
### 2018-03-16 添加二维码显示库，支持二维码显示

- 添加二维码库，驱动库来自：[简谈二维码（QRcode）的C语言生成，在单片机平台的实现](http://blog.csdn.net/qq_21475601/article/details/71480028)，感谢这位大神。
- 支持较短的字符串显示，后续会优化；
- 针对12864屏幕像素点小，对二维码点进行2倍放大；
- 只支持支付宝扫码，很奇怪。

---
### 2018-3-15 创建OLED驱动库，添加部分显示函数，支持2种字体和多种图形显示

- SPI接口0.96寸OLED屏幕驱动库，基于STM32F103ZET6；
- 支持两种字体6x8和8x16，可以显示字符，数字，字符串，可反白显示；
- 支持BMP图片显示，自带多个LOGO图片库：WINDOWS/ANDROID/STMCU/APPLE 等LOGO图片显示，内含制作软件和方法；
- 支持几何图形绘制：圆，直线，矩形，填充的矩形等多种几何图形，后续增加更多图形显示。

---
项目地址：https://github.com/whik/SPI-0.96-inch-OLED-Driver-Library-base-on-STM32F103ZET6
