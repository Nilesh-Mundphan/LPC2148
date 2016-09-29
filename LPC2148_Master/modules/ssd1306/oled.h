#ifndef OLED_H_
#define OLED_H_

#define OLED_ADDR 0x78 

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3
#define CHAR_WIDTH_START_POS 4


#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

 #define SSD1306_LCDWIDTH                  128
 #define SSD1306_LCDHEIGHT                 64

#define byte uint8_t

void oled_init(void);
void oled_on(void);
void oled_off(void);
void oled_reset(void);
void oled_clear(void);
void oled_cmd(uint8_t com);
void oled_data(uint8_t data);
void oled_fill(uint8_t fill_data);

void oled_update(void);

void oled_set_pixel(uint8_t,uint8_t,uint8_t);
void oled_print_xy(int x, int y, char * text);

#endif /* OLED_H_ */
