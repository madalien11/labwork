enum JOYSTICK_POS {LEFT ,RIGHT ,NEUTRAL, UP, DOWN};


struct controller {
	volatile uint8_t x_axis;
	volatile uint8_t y_axis;
	volatile uint8_t slider_left;
	volatile uint8_t slider_right;
	int joystick_pressed;
	int left_button_pressed;
	int right_button_pressed;
	enum JOYSTICK_POS pos;
};

struct controller get_controller();