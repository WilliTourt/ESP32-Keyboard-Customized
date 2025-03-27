#include "oled.h"

uint8_t OLED_HEIGHT = 64;
uint8_t OLED_IIC_BUS = 0;

/**
  * @brief  OLED 写入命令
  * @param  command: 命令
  * @retval None
  */
void OLED_WriteCommand(uint8_t command) {
    IIC_Start(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, OLED_IIC_ADDR);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, 0x00);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, command);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_Stop(OLED_IIC_BUS);
}

/**
  * @brief  OLED 向GDDRAM写入数据
  * @param  Data: 数据
  * @retval None
  */
void OLED_WriteData(uint8_t Data) {
    IIC_Start(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, OLED_IIC_ADDR);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, 0x40);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_SendByte(OLED_IIC_BUS, Data);
    IIC_RcvACK(OLED_IIC_BUS);
    IIC_Stop(OLED_IIC_BUS);
}


/**
  * @brief  OLED 设置光标位置
  * @param  x: X坐标 (0-127)
  * @param  y: 行坐标 (0-7)
  * @note   (0,0)------------->(127,0)
  * @note   (0,7)------------->(127,7)
  * @retval None
  */
void OLED_SetCursor(uint8_t x, uint8_t y) {
    uint8_t lines = 7;
    if (OLED_HEIGHT == 32) {
        lines = 3;
    }
    if (y > lines) {
        y = lines;
    }
    if (x > 127) {
        x = 127;
    }
    OLED_WriteCommand(0xB0 + y); //设置页地址
    OLED_WriteCommand(0x00 | (x & 0x0F)); //设置列低地址
    OLED_WriteCommand(0x10 | ((x & 0xF0) >> 4)); //设置列高地址
}

/**
  * @brief  OLED 清屏
  * @param  None
  * @retval None
  */
void OLED_Clear() {
    uint8_t i, j;
    uint8_t lines = 8;

    if (OLED_HEIGHT == 32) {
        lines = 4;
    }

    for (i = 0; i < lines; i++) {
        OLED_SetCursor(0, i);
        for (j = 0; j < 128; j++) {
            OLED_WriteData(0x00);
        }
    }
    OLED_SetCursor(0, 0);
}

/**
  * @brief  OLED 清除部分屏幕
  * @param  x1: X坐标起始 (0-128)
  * @param  y1: 行坐标起始 (0-8)
  * @param  x2: X坐标结束 (0-128)
  * @param  y2: 行坐标结束 (0-8)
  * @retval None 
  */
void OLED_ClearPart(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t i, j;
    for (i = y1; i < y2; i++) {
        OLED_SetCursor(x1, i);
        for (j = x1; j < x2; j++) {
            OLED_WriteData(0x00);
        }
    }
    OLED_SetCursor(x1, y1);
}

/**
  * @brief  OLED 初始化
  * @param  None
  * @retval None
  */
void OLED_Init(uint8_t scl, uint8_t sda, uint8_t height, uint8_t bus) {

    OLED_HEIGHT = height;
    OLED_IIC_BUS = bus;

    delay(50);
    IIC_Init(scl, sda, 0, bus);
    delay(20);

    OLED_WriteCommand(0xAE); //关闭显示

    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80); //设置时钟分频比/振荡器频率

    OLED_WriteCommand(0xA8);
    if (height == 32) {
        OLED_WriteCommand(0x1F);
    } else {
        OLED_WriteCommand(0x3F); //设置驱动路数
    }

    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00); //设置显示偏移

    OLED_WriteCommand(0x40); //设置显示起始行

    OLED_WriteCommand(0xA1); //设置段重映射

    OLED_WriteCommand(0xC8); //设置COM扫描方向

    OLED_WriteCommand(0xDA);
    if (height == 32) {
        OLED_WriteCommand(0x02);
    } else {
        OLED_WriteCommand(0x12); //设置COM硬件配置
    }

    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xCF); //设置亮度

    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1); //设置预充电周期

    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x30); //设置VCOMH

    OLED_WriteCommand(0xA4); //设置显示开启

    OLED_WriteCommand(0xA6); //设置正常显示

    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14); //设置充电泵开启

    OLED_WriteCommand(0xAF); //开启显示

    delay(30);

    OLED_Clear();
}

/**
  * @brief  OLED 显示文本(暂仅支持ASCII字符)
  * @param  x: X坐标 (0-127)
  * @param  y: 行坐标 (0-7)
  * @param  str: 显示的字符串
  * @param  size: 字体大小 (8/16)
  * @retval None
  */
void OLED_PrintText(uint8_t x, uint8_t y, const char* str, uint8_t size) {
    uint8_t i, j = 0;
    uint8_t character;

    if (str[1] == '\0') {

        character = str[0];
        OLED_SetCursor(x, y);
        if (size == 8) {
            for (i = 0; i < 6; i++) {
                OLED_WriteData(OLED_ASCII6x8[character - ' '][i]);
            }
        } else if (size == 16) {
            for (i = 0; i < 8; i++) {
                OLED_WriteData(OLED_ASCII8x16[character - ' '][i]);
            }
            OLED_SetCursor(x, y + 1);
            for (i = 0; i < 8; i++) {
                OLED_WriteData(OLED_ASCII8x16[character - ' '][i + 8]);
            }
        }
    
    } else {

        while (str[j] != '\0') {
            OLED_SetCursor(x, y);
            character = str[j]; //获取当前字符

            if (size == 8 && (character - ' ') < 95) {
                for (i = 0; i < 6; i++) {
                    OLED_WriteData(OLED_ASCII6x8[character - ' '][i]);
                }
                x += 6;
            } else if (size == 16 && (character - ' ') < 95) {
                for (i = 0; i < 8; i++) {
                    OLED_WriteData(OLED_ASCII8x16[character - ' '][i]);
                }
                OLED_SetCursor(x, y + 1);
                for (i = 0; i < 8; i++) {
                    OLED_WriteData(OLED_ASCII8x16[character - ' '][i + 8]);
                }
                x += 8;
            }

            j++; //指向下一个字符

            if (x > 127) { //超出屏幕，换行
                x = 0;
                if (size == 8) {
                    y++;
                } else if (size == 16) {
                    y += 2;
                }
            }
        }
    }
}

/**
  * @brief  OLED 显示图片
  * @param  x: X坐标 (0-127)
  * @param  y: Y坐标 (0-63)
  * @param  width: 图片宽度 (1-128)
  * @param  height: 图片高度 (1-8)
  * @param  image: 图片数据(一维数组)
  * @retval None
  */
void OLED_PrintImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* image) {
    uint8_t i, j;
    for (i = 0; i < height; i++) {
        OLED_SetCursor(x, y + i);
        for (j = 0; j < width; j++) {
            OLED_WriteData(image[i * width + j]);
        }
    }
}

bool cmpStrings(const char* str1, const char* str2) {
    while (*str1 && *str2) { // 当两个字符串都没结束时
        if (*str1 != *str2) {
            return false; // 如果字符不同，返回 false
        }
        str1++;
        str2++;
    }
    return (*str1 == *str2); // 如果两者都到达字符串末尾，则相等
}

/**
  * @brief  OLED 显示变量
  * @param  x: X坐标 (0-127)
  * @param  y: Y坐标 (0-7)
  * @param  num: 变量值
  * @param  type: 变量类型 ("int"/"float"/"hex")
  * @param  length: 打印时最小长度 (1-16)
  * @retval None
  */
void OLED_PrintVar(uint8_t x, uint8_t y, float num, const char* type, uint8_t length) {
    uint8_t i, j;
    char str[16];

    if (cmpStrings(type, "int")) {
        sprintf(str, "%0*d", length, (int)num);
    } else if (cmpStrings(type, "float")) {
        sprintf(str, "%0.*f", length, (float)num);
    } else if (cmpStrings(type, "hex")) {
        sprintf(str, "%0*X", length, (unsigned int)num);
    } else {
        sprintf(str, "ERR");
    }

    for (j = 0; str[j] != '\0'; j++) {
        OLED_SetCursor(x, y);
        for (i = 0; i < 6; i++) {
            OLED_WriteData(OLED_ASCII6x8[str[j] - ' '][i]);
        }
        x += 6;
    }
}

/**
  * @brief  OLED 显示水平
  * @param  x: X坐标 (0-127)
  * @param  y: Y坐标 (0-7)
  * @param  width: 线宽 (1-128)
  * @retval None
  */
void OLED_PrintHLine(uint8_t x, uint8_t y, uint8_t width) {
    uint8_t i;
    for (i = 0; i < width; i++) {
        OLED_SetCursor(x + i, y);
        OLED_WriteData(0x01);
    }
}

/**
  * @brief  OLED 显示垂直线
  * @param  x: X坐标 (0-127)
  * @param  y: Y坐标 (0-7)
  * @param  height: 线高 (1-8)
  * @retval None
  */
void OLED_PrintVLine(uint8_t x, uint8_t y, uint8_t height) {
    uint8_t i;
    for (i = 0; i < height; i++) {
        OLED_SetCursor(x, y + i);
        OLED_WriteData(0xFF);
    }
}

void OLED_LowBrightness(bool lowBrightness) {
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x81);
    if(lowBrightness) {
        OLED_WriteCommand(0x01);
    } else {
        OLED_WriteCommand(0xCF);
    }
}

void OLED_Power(bool state) {
    if(state) {
        OLED_WriteCommand(0xAF);
    } else {
        OLED_WriteCommand(0xAE);
    }
}