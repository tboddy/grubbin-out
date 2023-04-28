void updateControls(u16 joy, u16 changed, u16 state){
	if(joy == JOY_1){
		if(changed){}
		controls.left = (state & BUTTON_LEFT);
		controls.right = (state & BUTTON_RIGHT);
		controls.up = (state & BUTTON_UP);
		controls.down = (state & BUTTON_DOWN);
		controls.a = (state & BUTTON_A);
		controls.b = (state & BUTTON_B);
		controls.c = (state & BUTTON_C);
		controls.start = (state & BUTTON_START);
	}
}