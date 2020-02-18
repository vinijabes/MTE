#pragma once

/* from glfw3.h */
#define KINEMATICS_KEY_SPACE              32
#define KINEMATICS_KEY_APOSTROPHE         39  /* ' */
#define KINEMATICS_KEY_COMMA              44  /* , */
#define KINEMATICS_KEY_MINUS              45  /* - */
#define KINEMATICS_KEY_PERIOD             46  /* . */
#define KINEMATICS_KEY_SLASH              47  /* / */
#define KINEMATICS_KEY_0                  48
#define KINEMATICS_KEY_1                  49
#define KINEMATICS_KEY_2                  50
#define KINEMATICS_KEY_3                  51
#define KINEMATICS_KEY_4                  52
#define KINEMATICS_KEY_5                  53
#define KINEMATICS_KEY_6                  54
#define KINEMATICS_KEY_7                  55
#define KINEMATICS_KEY_8                  56
#define KINEMATICS_KEY_9                  57
#define KINEMATICS_KEY_SEMICOLON          59  /* ; */
#define KINEMATICS_KEY_EQUAL              61  /* = */
#define KINEMATICS_KEY_A                  65
#define KINEMATICS_KEY_B                  66
#define KINEMATICS_KEY_C                  67
#define KINEMATICS_KEY_D                  68
#define KINEMATICS_KEY_E                  69
#define KINEMATICS_KEY_F                  70
#define KINEMATICS_KEY_G                  71
#define KINEMATICS_KEY_H                  72
#define KINEMATICS_KEY_I                  73
#define KINEMATICS_KEY_J                  74
#define KINEMATICS_KEY_K                  75
#define KINEMATICS_KEY_L                  76
#define KINEMATICS_KEY_M                  77
#define KINEMATICS_KEY_N                  78
#define KINEMATICS_KEY_O                  79
#define KINEMATICS_KEY_P                  80
#define KINEMATICS_KEY_Q                  81
#define KINEMATICS_KEY_R                  82
#define KINEMATICS_KEY_S                  83
#define KINEMATICS_KEY_T                  84
#define KINEMATICS_KEY_U                  85
#define KINEMATICS_KEY_V                  86
#define KINEMATICS_KEY_W                  87
#define KINEMATICS_KEY_X                  88
#define KINEMATICS_KEY_Y                  89
#define KINEMATICS_KEY_Z                  90
#define KINEMATICS_KEY_LEFT_BRACKET       91  /* [ */
#define KINEMATICS_KEY_BACKSLASH          92  /* \ */
#define KINEMATICS_KEY_RIGHT_BRACKET      93  /* ] */
#define KINEMATICS_KEY_GRAVE_ACCENT       96  /* ` */
#define KINEMATICS_KEY_WORLD_1            161 /* non-US #1 */
#define KINEMATICS_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KINEMATICS_KEY_ESCAPE             256
#define KINEMATICS_KEY_ENTER              257
#define KINEMATICS_KEY_TAB                258
#define KINEMATICS_KEY_BACKSPACE          259
#define KINEMATICS_KEY_INSERT             260
#define KINEMATICS_KEY_DELETE             261
#define KINEMATICS_KEY_RIGHT              262
#define KINEMATICS_KEY_LEFT               263
#define KINEMATICS_KEY_DOWN               264
#define KINEMATICS_KEY_UP                 265
#define KINEMATICS_KEY_PAGE_UP            266
#define KINEMATICS_KEY_PAGE_DOWN          267
#define KINEMATICS_KEY_HOME               268
#define KINEMATICS_KEY_END                269
#define KINEMATICS_KEY_CAPS_LOCK          280
#define KINEMATICS_KEY_SCROLL_LOCK        281
#define KINEMATICS_KEY_NUM_LOCK           282
#define KINEMATICS_KEY_PRINT_SCREEN       283
#define KINEMATICS_KEY_PAUSE              284
#define KINEMATICS_KEY_F1                 290
#define KINEMATICS_KEY_F2                 291
#define KINEMATICS_KEY_F3                 292
#define KINEMATICS_KEY_F4                 293
#define KINEMATICS_KEY_F5                 294
#define KINEMATICS_KEY_F6                 295
#define KINEMATICS_KEY_F7                 296
#define KINEMATICS_KEY_F8                 297
#define KINEMATICS_KEY_F9                 298
#define KINEMATICS_KEY_F10                299
#define KINEMATICS_KEY_F11                300
#define KINEMATICS_KEY_F12                301
#define KINEMATICS_KEY_F13                302
#define KINEMATICS_KEY_F14                303
#define KINEMATICS_KEY_F15                304
#define KINEMATICS_KEY_F16                305
#define KINEMATICS_KEY_F17                306
#define KINEMATICS_KEY_F18                307
#define KINEMATICS_KEY_F19                308
#define KINEMATICS_KEY_F20                309
#define KINEMATICS_KEY_F21                310
#define KINEMATICS_KEY_F22                311
#define KINEMATICS_KEY_F23                312
#define KINEMATICS_KEY_F24                313
#define KINEMATICS_KEY_F25                314
#define KINEMATICS_KEY_KP_0               320
#define KINEMATICS_KEY_KP_1               321
#define KINEMATICS_KEY_KP_2               322
#define KINEMATICS_KEY_KP_3               323
#define KINEMATICS_KEY_KP_4               324
#define KINEMATICS_KEY_KP_5               325
#define KINEMATICS_KEY_KP_6               326
#define KINEMATICS_KEY_KP_7               327
#define KINEMATICS_KEY_KP_8               328
#define KINEMATICS_KEY_KP_9               329
#define KINEMATICS_KEY_KP_DECIMAL         330
#define KINEMATICS_KEY_KP_DIVIDE          331
#define KINEMATICS_KEY_KP_MULTIPLY        332
#define KINEMATICS_KEY_KP_SUBTRACT        333
#define KINEMATICS_KEY_KP_ADD             334
#define KINEMATICS_KEY_KP_ENTER           335
#define KINEMATICS_KEY_KP_EQUAL           336
#define KINEMATICS_KEY_LEFT_SHIFT         340
#define KINEMATICS_KEY_LEFT_CONTROL       341
#define KINEMATICS_KEY_LEFT_ALT           342
#define KINEMATICS_KEY_LEFT_SUPER         343
#define KINEMATICS_KEY_RIGHT_SHIFT        344
#define KINEMATICS_KEY_RIGHT_CONTROL      345
#define KINEMATICS_KEY_RIGHT_ALT          346
#define KINEMATICS_KEY_RIGHT_SUPER        347
#define KINEMATICS_KEY_MENU               348

#define KINEMATICS_MOD_SHIFT           0x0001
#define KINEMATICS_MOD_CONTROL         0x0002
#define KINEMATICS_MOD_ALT             0x0004
#define KINEMATICS_MOD_SUPER           0x0008
#define KINEMATICS_MOD_CAPS_LOCK       0x0010
#define KINEMATICS_MOD_NUM_LOCK        0x0020

#define KINEMATICS_MOUSE_BUTTON_1			0
#define KINEMATICS_MOUSE_BUTTON_2			1
#define KINEMATICS_MOUSE_BUTTON_3			2
#define KINEMATICS_MOUSE_BUTTON_4			3
#define KINEMATICS_MOUSE_BUTTON_5			4
#define KINEMATICS_MOUSE_BUTTON_6			5
#define KINEMATICS_MOUSE_BUTTON_7			6
#define KINEMATICS_MOUSE_BUTTON_8			7
#define KINEMATICS_MOUSE_BUTTON_LAST		KINEMATICS_MOUSE_BUTTON_8
#define KINEMATICS_MOUSE_BUTTON_LEFT		KINEMATICS_MOUSE_BUTTON_1
#define KINEMATICS_MOUSE_BUTTON_RIGHT		KINEMATICS_MOUSE_BUTTON_2
#define KINEMATICSGLFW_MOUSE_BUTTON_MIDDLE	KINEMATICS_MOUSE_BUTTON_3