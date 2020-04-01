
#define CLAVE 0x12345678L
#define CNL_SRV 0x100L
#define CNL_CLI 0x200L

#define MAX_BUF 256
#define NCLI 3

//#define FIFO
#define FIFONAME "/tmp/fifo-srv"
#define NSEMS 5
#define SIZE_SHM 1024

#define MAX_MSG 7
#define MAX_TXT 30
#define MAX_CNT 5
#define MAX_REG NCLI

#define DSP_VAL 100
#define DSP_REG 120
#define DSP_CNT 300

union semun {
	int val;
	long pid;
};

struct st_reg {
	unsigned char st;
	long pid;
//	char txt[MAX_TXT];
};
