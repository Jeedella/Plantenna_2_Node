// Includes
#include "spms_libs.h"
#include "spms_ble.h"
#include "spms_sensor.h"

// Compiler macros
#define HEAP_SIZE 512

//Includes
static airflow_local* localStorage;
static struct k_timer updateTimer;
static unsigned storageIndex = 0;

// Timer expire handler
void updateHandler()
{
	unsigned timeStamp = (unsigned)k_uptime_ticks();

	printk("[Time] Timer expired at %d\n", timeStamp);
	printk("[Log] Current storage index is %d\n", storageIndex);

	if(storageIndex < 512) {
		localStorage[storageIndex].time = timeStamp;
		sensor_read(&localStorage[storageIndex]);
		ble_update_airflow(&localStorage[storageIndex], (uint8_t)sys_rand32_get());
		storageIndex++;
        printk("[Log] Updated local storage\n");
	}
	else printk("[Error] local storage out of memory\n");
}

// Initialization
int init_SPMS()
{
	#define WARNING 0x1
	#define ERROR 0x2

	const static char strInit [] = "Initializing";
	const static char strPass [] = "[Passed]";
	const static char strFail [] = "[Failed]";

	int status = 0;

	printk("\nSmart Plant Monitoring System V1\n\n");

	// Local storage
	printk("[%s] local storage\n", strInit);
	localStorage = k_malloc(HEAP_SIZE * sizeof(airflow_local));
	if (localStorage != NULL) printk("%s %s local storage\n", strPass, strInit);
	else {printk("%s %s local storage\n", strPass, strInit); status = status ^ ERROR;}

	// ADC / sensors
	printk("[%s] Sensors\n", strInit);
	if (!sensor_init()) printk("%s %s Sensors\n", strPass, strInit);
	else {printk("%s %s Sensors\n", strFail, strInit); status = status ^ WARNING;}

	// Bluetooth
	printk("[%s] bluetooth\n", strInit);
	if(!bt_enable(NULL)) printk("%s %s bluetooth\n", strPass, strInit);
	else {printk("%s %s bluetooth\n", strFail, strInit); status = status ^ ERROR;}

	// Start "update" timer, callback every minute
	printk("[%s] update timer\n", strInit);
	k_timer_init(&updateTimer, updateHandler, NULL);
	k_timer_start(&updateTimer, K_SECONDS(5), K_SECONDS(5));
	printk("%s %s update timer\n", strPass, strInit);

	// Start advertising
	printk("[%s] airflow advertising\n", strInit);
	if(!ble_adv_start_airflow()) printk("%s %s airflow advertising\n", strPass, strInit);
	else {printk("%s %s airflow advertising\n", strFail, strInit); status = status ^ WARNING;}

	// Status check
	if (status & ERROR) {printk("[Error] Initialization failed\n"); return 1;}
	else if (status & WARNING) {printk("[Warning] Initialization passed with warnings\n");}
	else printk("[Pass] Initialization passed without warnings\n");

	return 0;
}

// Main
int main()
{
	if(!init_SPMS()) {
		printk("[Starting] Application\n");
		updateHandler();
		while(1)
		{

		}
	}
	printk("[Stopping] Application\n");

	return 0;
}