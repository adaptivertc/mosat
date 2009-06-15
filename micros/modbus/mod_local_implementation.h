
bool read_di(int n);
bool read_do(int n);
void send_do(int n, bool val);
uint16_t read_ai(int n);
uint16_t read_register(int n);
void write_register(int n, uint16_t val);

bool check_range_di(int start, int end);
bool check_range_do(int start, int end);
bool check_range_ai(int start, int end);
bool check_range_reg_in(int start, int end);
bool check_range_reg_out(int start, int end);

