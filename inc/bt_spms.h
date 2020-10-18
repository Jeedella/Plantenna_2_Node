#ifndef bt_spms
#define bt_spms

// Bluetooth interace for the Smart Plant Monitoring System (SPMS)

#define AIRFLOW_INIT 0

#define AIRFLOW_BASE_UUID BT_UUID_INIT_128(	\
	0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20, 0x95, 0x7, 0xB2, 0x63, 0x01, 0x07, 0x31, 0x1A 	\
)

#define AIRFLOW_BCAST_UUID BT_UUID_INIT_128(	\
	0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20, 0x95, 0x7, 0xB2, 0x63, 0x02, 0x07, 0x31, 0x1A 	\
)

typedef struct {
    uint32_t time;
    int16_t temp;
    uint16_t humid;
    uint16_t press;
    uint16_t PM_1_0;
    uint16_t PM_2_5;
    uint16_t PM_10_0;
    uint16_t PC_0_3;
    uint16_t PC_0_5;
    uint16_t PC_1_0;
    uint16_t PC_2_5;
    uint16_t PC_5_0;
    uint16_t PC_10_0;
} bt_spms_mem_data;

uint8_t bt_spms_init(uint8_t profile);
uint8_t bt_spms_update(bt_spms_data* spms_local);

void cts_notify(void);

#endif