enum pll_stat_mask {
	TLO = (1<<0),
	THI = (1<<1),
	LOCK = (1<<2),
	ALCLO = (1<<3),
	ALCHI = (1<<4),
	UNLOCK = (1<<5)
};

void set_pll_stat(enum pll_stat_mask stat_mask);
void pll_setup(void);
void pll_write(char addr, char data);
char pll_read(char addr);
