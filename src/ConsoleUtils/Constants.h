#pragma once

#define stdInput GetStdHandle(STD_INPUT_HANDLE)
#define stdOutput GetStdHandle(STD_OUTPUT_HANDLE)

#define UNHANDLED_EVENT 0
#define KEY_DOWN_EVENT 1
#define KEY_UP_EVENT 2


#define TEXT_STYLE_DEFAULT 0x0
#define TEXT_STYLE_BOLD 0x1
#define TEXT_STYLE_GRAYED 0x2
#define TEXT_STYLE_ITALIC 0x4
#define TEXT_STYLE_UNDERLINED 0x8
#define TEXT_STYLE_BLINKING 0x10
#define TEXT_STYLE_INVERTED 0x20
#define TEXT_STYLE_STRIKETHROUGH 0x40
#define TEXT_STYLE_DOUBLE_UNDERLINED 0x80

#define TEXT_COLOR_DEFAULT 39
#define TEXT_COLOR_BLACK 30
#define TEXT_COLOR_RED 31
#define TEXT_COLOR_GREEN 32
#define TEXT_COLOR_YELLOW 33
#define TEXT_COLOR_BLUE 34
#define TEXT_COLOR_MAGENTA 35
#define TEXT_COLOR_CYAN 36
#define TEXT_COLOR_WHITE 37
#define TEXT_COLOR_MOD_BRIGHT 60


#define VK_PIPE_DOWN_LEFT 0xBF
#define VK_PIPE_UP_RIGHT 0xC0
#define VK_PIPE_UP_LEFT_RIGHT 0xC1
#define VK_PIPE_DOWN_LEFT_RIGHT 0xC2
#define VK_PIPE_UP_DOWN_RIGHT 0xC3
#define VK_PIPE_LEFT_RIGHT 0xC4
#define VK_PIPE_UP_DOWN_LEFT_RIGHT 0xC5
#define VK_PIPE_UP_LEFT 0xD9
#define VK_PIPE_DOWN_RIGHT 0xDA
#define VK_PIPE_UP_DOWN 0xB3
#define VK_PIPE_UP_DOWN_LEFT 0xB4

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
