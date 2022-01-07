#define MY_ID 41
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_SIZE 1024
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

#define SEM_KEY1 (0x7000 + MY_ID)
#define SEM_KEY2 (0x8000 + MY_ID)
