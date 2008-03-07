
int spd_beep(void);
int spd_getch(void);
int spd_endwin(void);
void spd_print_auto(bool b);
int spd_wait_key(char *msg);
int spd_prompt_user(char *msg1, char *msg2);
void spd_redraw_segment(void);
void spd_init_segment(char *st1, char *st2);
void spd_init_screen();
void spd_redraw_all();
void spd_print_current(double desired, double actual, int type, bool warn, 
       double pct, double total_distance, double now, double total_time);
void spd_show_loading(int time);
void spd_create_image(char *base_name, char *gtitle, bool window);
int select_from_list(int n, char *list[], char *prompt);
