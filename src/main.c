#include "spms_libs.h"
#include "spms_bt_airflow.h"

#define HEAP_SIZE 512

static spms_airflow_local* localStorage;
static struct k_timer updateTimer;
static unsigned storageIndex = 0;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
	      0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20,
		  0x95, 0x7, 0xB2, 0x63, 0x01, 0x00, 0x31, 0x1A),
	BT_DATA_BYTES(BT_DATA_NAME_COMPLETE, 'S', 'P', 'M', 'S', ' ', 'N', '0', '1')
};

// Timer expire handler, every minute
void updateHandler() {
	unsigned timeStamp = (unsigned)k_uptime_ticks();
	printk("\r\n[%d] ", timeStamp);
	if(storageIndex < 512) {
		localStorage[storageIndex].time = timeStamp;
		bt_update_airflow(&localStorage[storageIndex], (uint8_t)sys_rand32_get());

		printk("Updated local storage at index: %d", storageIndex);
		storageIndex++;
	}
	else printk("Error: local storage out of memory");
}

// Initialize function
int init_SPMS() {
	#define PRINT_SUCCESS printk("success\r\n")
	#define PRINT_FAILED printk("failed\r\n")

	const char strInit [] = "Initializing";

	printk("\r\nSmart Plant Monitoring System V1\r\n");
	printk("\r\nStarting initialization process\r\n");

	// Local storage
	printk("\t%s local storage ", strInit);
	localStorage = k_malloc(HEAP_SIZE * sizeof(spms_airflow_local));
	if(localStorage != NULL) PRINT_SUCCESS;
	else {PRINT_FAILED;	return 1;}

	// ADC / sensors

	// Bluetooth
	printk("\t%s bluetooth ", strInit);
	if(!bt_enable(NULL)) PRINT_SUCCESS;
	else {PRINT_FAILED;	return 1;}

	// Airflow sensor
	printk("\t%s bluetooth airflow data ", strInit);
	if(!bt_init_airflow(0x1D, 0xFF, 0xABFF)) PRINT_SUCCESS;
	else {PRINT_FAILED;	return 1;}

	// Update timer, callback every minute
	printk("\t%s update timer ", strInit);
	k_timer_init(&updateTimer, updateHandler, NULL);
	k_timer_start(&updateTimer, K_SECONDS(60), K_SECONDS(60));
	PRINT_SUCCESS;

	// Start advertising
	printk("\t%s bluetooth advertising ", strInit);
	if(!bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0)) PRINT_SUCCESS;
	else {PRINT_FAILED;	return 1;}

	printk("Initialization "); PRINT_SUCCESS;
	return 0;
}

int main()
{
	if(!init_SPMS()) {
		printk("\r\nStarting application\r\n");
		while(1)
		{
			
		}
	}
	printk("\r\nShutting down\r\n");
	return 0;
}