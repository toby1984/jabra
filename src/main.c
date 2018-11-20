#include <stdbool.h>
#include "Common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static unsigned short attachedDeviceID;

static bool scanDone = false;

static bool verbose = false;

void firstScanDoneFunc() {

   /* Get battery status, if supported by device.
   *  @param[in] : Id for a specific device.
   *  @param[in] : Battery level in percent (0 - 100).
   *  @param[in] : charging: Indicates if battery is being charged.
   *  @param[in] : Indicates if battery level is low.
   *  @return    : Return_Ok if get battery information is returned.
   Device_Unknown if deviceID is wrong.
   Not_Supported if device does not have battery information.
  LIBRARY_API Jabra_ReturnCode Jabra_GetBatteryStatus(
    unsigned short deviceID,
    int *levelInPercent,
    bool *charging,
    bool *batteryLow
  );
   */
   int levelPerc = 0;
   bool isCharging = false;
   bool isBatteryLow = false;

   Jabra_ReturnCode result = Jabra_GetBatteryStatus( attachedDeviceID, &levelPerc, &isCharging, &isBatteryLow );
   if ( result != Return_Ok ) {
       printf("Failed to get battery status\n");
   } else {
     printf("return_code=%d, battery_level=%d, is_charging=%d, is_low=%d\n",result,levelPerc, isCharging, isBatteryLow );
   }
   scanDone = true;
}

void deviceAttached(Jabra_DeviceInfo devInfo)
{
  if ( verbose ) {
    printf("Device attached: %d\n",devInfo.deviceID);
  }

  attachedDeviceID = devInfo.deviceID;
}

void deviceRemoved(unsigned short deviceID) {
  if ( verbose ) {
    printf("Device removed: %d\n",deviceID);
  }
}

void buttonInRawHidFunc(unsigned short deviceID, unsigned short usagePage, unsigned short usage, bool buttonInData) {
  if ( verbose ) {
    printf("buttonInRawHidFunc: device=%d\n, usage_page=%d, usage=%d, button_data=%d",deviceID,usagePage,usage,buttonInData);
  }
}

void buttonInTranslatedFunc(unsigned short deviceID, Jabra_HidInput translatedInData, bool buttonInData)
{
  if ( verbose ) {
    printf("buttonInTranslatedFunc: device=%d\n, hid_input=%d, button_data=%d",deviceID,translatedInData,buttonInData);
  }
}

int main(int argc,char **arg) {

  if ( argc >= 1 && strcmp(arg[0],"-v") == 0 ) {
    verbose = true;
  }

  Jabra_SetAppID("140737488344880");

  if ( verbose ) {
    printf("Now initializing library...\n");
  }
  if ( Jabra_Initialize( &firstScanDoneFunc, &deviceAttached, &deviceRemoved, &buttonInRawHidFunc, &buttonInTranslatedFunc,0) )
  {
    while ( ! scanDone ) {
      usleep( 250 * 1000 );
    }
  } else {
    printf("ERROR: Failed to initialize library\n");
  }
}
