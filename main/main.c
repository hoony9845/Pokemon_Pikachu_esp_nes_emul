//#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "nofrendo.h"
#include "menu.h"

#define SAVE_ENTRY_SIZE	(24*1024)
#define SAVE_PARTITION_TYPE	0x60
#define SAVE_PARTITION_SUBTYPE	1
#define SAVE_ENTRY (romPartition * SAVE_ENTRY_SIZE)

int romPartition;

size_t save_partition_offset;

void esp_clear_save_partion_offset(void) {
      printf("### esp_clear_save_partion_offset ### \n");
      save_partition_offset = 0;
}

void esp_move_save_partion_offset(int offset) {
      printf("### esp_clear_save_partion_offset ### (cur:%d, offset:%d)\n",save_partition_offset, offset);
      save_partition_offset += offset;
}

void esp_erase_save_partion(void) {
      const esp_partition_t* part;
      esp_err_t err;

      printf("### esp_erase_save_partion ### \n");
      printf("  ### find save partition ### \n");
      part = esp_partition_find_first(SAVE_PARTITION_TYPE, SAVE_PARTITION_SUBTYPE, NULL);
      if (part == 0) {
         printf("Fail to find the save partition\n");
	 return;
      }

      printf("  ### erase save partition ### \n");
      err = esp_partition_erase_range(part, SAVE_ENTRY, SAVE_ENTRY_SIZE);
      if (err != ESP_OK) {
         printf("Fail to erase to the save partition err:%d\n", err);
	 return;
      }
      printf("  ### erase save partition ### done\n");
}

int esp_write_save_partition(void *buf, size_t size) {
      const esp_partition_t* part;
      esp_err_t err;

      printf("### esp_write_save_partition ### (size:%d)\n", size);
      printf("  ### find save partition ###\n");
      part = esp_partition_find_first(SAVE_PARTITION_TYPE, SAVE_PARTITION_SUBTYPE, NULL);
      if (part == 0) {
         printf("Fail to find the save partition\n");
	 return -1;
      }

      printf("  ### write save partition ###\n");
      err = esp_partition_write(part, SAVE_ENTRY + save_partition_offset, buf, size);
      if (err != ESP_OK) {
         printf("Fail to write to the save partition err:%d\n", err);
	 return -1;
      }
      save_partition_offset += size;
      return 1;
}

int esp_read_save_partition(void *buf, size_t size) {
      const esp_partition_t* part;
      esp_err_t err;

      printf("### esp_read_save_partition ### (size:%d)\n", size);
      printf("  ### find save partition ###\n");
      part = esp_partition_find_first(SAVE_PARTITION_TYPE, SAVE_PARTITION_SUBTYPE, NULL);
      if (part == 0) {
         printf("Fail to find the save partition\n");
	 return -1;
      }
      printf("  ### read save partition ###\n");
      err = esp_partition_read(part, SAVE_ENTRY + save_partition_offset, buf, size);
      if (err != ESP_OK) {
         printf("Fail to read to the save partition err:%d\n", err);
	 return -1;
      }
      save_partition_offset += size;
      return 1;
}

char *osd_getromdata() {
	//printf("choosen: %d\n",romPartition);
	char* romdata;
	const esp_partition_t* part;
	spi_flash_mmap_handle_t hrom;
	esp_err_t err;
	nvs_flash_init();
	
	
	part=esp_partition_find_first(0x41+romPartition, 1, NULL);
	if (part==0) printf("Couldn't find rom part!\n");
	
	int partSize;
	switch(romPartition) {
		case 0: case 5: case 6: case 7:  	partSize = 100; break;
		case 1: case 4: case 11:	partSize = 260; break;
		case 2:								partSize = 388; break;
		case 3:					partSize = 132; break;
		case 8:								partSize = 772; break;
		case 9:								partSize = 516; break;
		case 10:							partSize = 296; break;
		default:							partSize = 0; break;
	}
	err=esp_partition_mmap(part, 0, partSize*1024, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom);
	if (err!=ESP_OK) printf("Couldn't map rom part!\n");
	printf("Initialized. ROM@%p\n", romdata);
    return (char*)romdata;
}

void esp_wake_deep_sleep(){
	esp_restart();
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

extern int  vid_setmode(int width, int height);
int app_main(void)
{
	vid_setmode(256, 240);
	romPartition=runMenu();
	printf("NoFrendo start!\n");
	nofrendo_main(0, NULL);
	printf("NoFrendo died? WtF?\n");
	asm("break.n 1");
    return 0;
}

