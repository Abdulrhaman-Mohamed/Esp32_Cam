#ifndef Camera_h
#define Camera_h



#include "esp_camera.h"
#include "Arduino.h"
#include "SD_MMC.h"
#include "FS.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <driver/gpio.h>




class Camera
{
    private:
    camera_config_t _config;

    


    public:
    Camera( camera_config_t config);
    static byte * bufferUART;
    
    static int Imagenumber;
    static int32_t _Image_Number  ;

    byte * Camera_online(size_t* length);

    void Camera_Offline(const int planId);
// Take path of photo (Plan ID) and repeat of command
   

    void removeAllFilesInFolder(const int planID );

    void SetupUART();

    void ReadUARTData();


};



#endif
