//--------------------------------
// Microprocessor Systems Lab 6 - Template - Lab06_p1_sample.c
//--------------------------------
//
//

#include "init.h"
#include "usbh_conf.h"
#include "usbh_hid.h"
#include "usbh_core.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

USBH_HandleTypeDef husbh;

void USBH_UserProcess(USBH_HandleTypeDef *, uint8_t);
void USBH_HID_EventCallback();
void ReadDirectory();

HID_MOUSE_Info_TypeDef *my_mouse;
FRESULT Mountres=0;
int ready = 0;
uint8_t output;
uint8_t ActClass;
char USBDISKPath[4];
FATFS USBH_fatfs;
int mounted=0;

int x;
int prev_x = 0;
int change_x;
int y;
int change_y;
int prev_y = 0;
int button;
int first = 1;


int main(void){
	 // System Initializations
	Sys_Init();

	// Application Initializations
	USBH_Init(&husbh, USBH_UserProcess, 0);
	// USBH Driver Initialization

	// USB Driver Class Registrations: Add device types to handle.
	USBH_RegisterClass(&husbh, USBH_HID_CLASS);
	USBH_RegisterClass(&husbh, USBH_MSC_CLASS);

	// Start USBH Driver
	USBH_Start(&husbh);

	//HAL_NVIC_EnableIRQ(OTG_HS_EP1_IN_IRQn);

	while(1){

		USBH_Process(&husbh);
		// Other stuff
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	if(id == 4){
		//printf("Device Connected\r\n");
	}

	if(id == 5){
		//printf("Device Disconnected\r\n");
		ready = 0;
	}

	if(id == 2){
		printf("Device Ready\r\n");
		ActClass=USBH_GetActiveClass(&husbh);
		if(ActClass==USB_MSC_CLASS){
			ReadDirectory();
		}
		else if(ActClass==USB_HID_CLASS){
			ready = 1;
		}

	}


}
void USBH_HID_EventCallback(USBH_HandleTypeDef *phost){
	if(ready == 1){
		my_mouse = USBH_HID_GetMouseInfo(&husbh);
		x = my_mouse->x;
		y = my_mouse->y;
		//first = 5;
		//change_x = x - prev_x;
		//change_y = y - prev_y;
		//prev_x = x;
		//prev_y = y;

		//button = my_mouse->buttons[0];
		//printf("%d\r\n",button);
		printf("%d\r\n",x);
		printf("%d\r\n",y);
	}
}

void ReadDirectory(){
    DIR FileDir;
    FRESULT res;

	static FILINFO fno;

	if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
		if(!mounted){
			Mountres = f_mount(&USBH_fatfs, (TCHAR const*)USBDISKPath, 0);
			mounted = 1;
		}
      if (Mountres == FR_OK)          // (TCHAR const*)USBDISKPath
      {
    	  if(f_opendir(&FileDir,(TCHAR const*)USBDISKPath)==FR_OK){
    		  while(1){
    			  fno.lfsize=_MAX_LFN-1;
    			  res = f_readdir(&FileDir, &fno);
    			  if(res != FR_OK || fno.fname[0] == 0) break;
    			  printf("/%s\r\n",fno.fname);
    		  }
    		  f_closedir(&FileDir);
    	  }
      }
    }
}
