----------

#### RK3288_MIPI DSI 使用说明 ####

12/25/2019 1 :32 :33 PM 

Author：terrycoder

----------

Config配置

在arch/arm/configs/firefly_defconfig添加配置：

```
CONFIG_LCD_MIPI=y
CONFIG_MIPI_DSI=y
CONFIG_RK32_MIPI_DSI=y

```

设备树配置说明
```
/{
	backlight {
		compatible = "pwm-backlight";
		pwms = <&pwm0 0 25000>;
		brightness-levels =  <//0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
			28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59
			60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91
			92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117
			118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141
			142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165
			166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189
			190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213
			214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237
			238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255>;
		default-brightness-level = <200>;
	};

```

属性:

- pwms属性：配置PWM，Firefly-RK3288使用pwm1，范例中的10000是PWM频率。

- brightness-levels属性：配置背光亮度数组，最大值为255，配置暗区和亮区，并把亮区数组做255的比例调节。比如范例中暗区是255-221，亮区是220-0。

- default-brightness-level属性：开机时默认背光亮度，范围为0-255。

具体请参考kernel中的说明文档： Documentation/devicetree/bindings/video/backlight/pwm-backlight.txt

```
/* about mipi */
	disp_mipi_init: mipi_dsi_init{
		compatible = "rockchip,mipi_dsi_init";
		rockchip,screen_init    = <1>;
		rockchip,dsi_lane               = <4>;
		rockchip,dsi_hs_clk             = <1200>;
		rockchip,mipi_dsi_num   = <1>;
	};
```
属性：

- rockchip,screen_init属性：0表示不需要特殊指令初始化显示屏，1,表示需要初始化指令。

- rockchip,dsi_lane属性：数据lane的数量。

- rockchip,dsi_hs_clk属性：配置hsclk。

- rockchip,mipi_dsi_num：配置只用DSI接口的数量，即单通道MIPI屏为1，双通道MIPI屏为2。

具体请参考kernel中的说明文档： Documentation/devicetree/bindings/video/rockchip_mipidsi_lcd.txt


```
disp_mipi_init_cmds: screen-on-cmds {
        	rockchip,cmd_debug = <0>;
        	compatible = "rockchip,screen-on-cmds";
        	//rockchip,on-cmds1 {
        	//	compatible = "rockchip,on-cmds";
        	//	rockchip,cmd_type = <LPDT>;
        	//	rockchip,dsi_id = <2>;
        	//	rockchip,cmd = <0x05 11>;
        	//	rockchip,cmd_delay = <10>;
        	//};
        	//rockchip,on-cmds2 {
        	//	compatible = "rockchip,on-cmds";
        	//	rockchip,cmd_type = <LPDT>;
        	//	rockchip,dsi_id = <2>;
        	//	rockchip,cmd = <0x05 dcs_set_display_on>;
        	//	rockchip,cmd_delay = <10>;
        	//};
	};
```

配置初始化命令
- 当rockchip,screen_init为1时需要配置显示屏的初始化命令，初始化命令在节点disp_mipi_init_cmds中配置。

- rockchip,cmd_debug属性：打开可输出指令调试信息。

- rockchip,on-cmdsXX子节点：配置每条指令的信息。

- rockchip,cmd_type：数据传输模式，LPDT或HSDT。

- rockchip,dsi_id：指令传输的DSI接口，0为向DSI0（双通道MIPI屏时为左半屏）发送指令，1为向DSI1（双通道MIPI屏时为右半屏）发送指令,2为同时向两个DSI发送数据。

- rockchip,cmd：指令序列。其中第一个字节为DSI数据类型，第二个字节为REG，后面的字节为指令内容。

- rockchip,cmd_delay：发送指令后的延时，单位为ms。


```
disp_mipi_power_ctr: mipi_power_ctr {
    compatible = "rockchip,mipi_power_ctr";

	/*mipi_lcd_en:mipi_lcd_en {
                compatible = "rockchip,lcd_en";
                        rockchip,gpios = <&gpio7 GPIO_A2 GPIO_ACTIVE_HIGH>;
                        rockchip,delay = <10>;
        };*/
        mipi_lcd_rst:mipi_lcd_rst{
                compatible = "rockchip,lcd_rst";
                        rockchip,gpios = <&gpio7 GPIO_A3 GPIO_ACTIVE_HIGH>;
                        rockchip,delay = <200>;
        };
	};
```

mipi lcd屏幕电源使能和复位管脚配置。

```
disp_timings: display-timings {
		native-mode = <&timing0>;
		timing0: timing0 {
			screen-type = <SCREEN_MIPI>;
			lvds-format = <LVDS_8BIT_2>;
			out-face    = <OUT_P888>;
			clock-frequency = <140000000>;
			hactive = <1920>;
			vactive = <1200>; 
			
			hsync-len = <20>;
			hback-porch = <60>;
			hfront-porch = <16>;
			
			vsync-len = <3>;
			vback-porch = <23>;
			vfront-porch = <12>;
			
			hsync-active = <0>;
			vsync-active = <0>;
			de-active = <0>;
			pixelclk-active = <0>;
			swap-rb = <0>;
			swap-rg = <0>;
			swap-gb = <0>;
			ddr-change-notify-screen = <0>;
		};
	};
};

&pwm0 {
//        interrupts = <GIC_SPI 78 IRQ_TYPE_LEVEL_HIGH>;
        status = "okay";
};


&rk_screen {
	 display-timings = <&disp_timings>;
};
```

screen-type：屏幕类型，mipi 屏有两种：单 mipi（SCREEN_MIPI）、双 mipi（SCREEN_DUAL_MIPI）。

lvds-format：lvds 数据格式。对于 mipi 屏是无效参数，不用配置。

out-face：屏幕接线格式。

上述三个参数的取值在 kernel/include/dt-bindings/rkfb/rk_fb.h中定义。

clock-frequency：dclk 频率，单位为 Hz，一般屏的规格书中有，也可以通过公式计算：H*V（包括同步信号）*fps

Hactive：水平有效像素

Vactive：垂直有效像素

hback-porch/hfront-porch/hsync-len：水平同步信号

vback-porch/vfront-porch/vsync-len：垂直同步信号

hsync-active、vsync-active、de-active、pixelclk-active：分别为 hync、vsync、DEN、dclk 的极性控制。置 1 将对极性进行翻转。

swap-rb、swap-rg、swap-gb：置 1 将对对应的颜色进行翻转。