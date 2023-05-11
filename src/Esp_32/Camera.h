#ifndef Camera_h
#define Camera_h



#include "esp_camera.h"
#include "Arduino.h"
#include "SD_MMC.h"





class Camera
{
    private:
    camera_config_t _config;


    public:
    Camera( camera_config_t config);
    static int Imagenumber;

    char * Camera_online(size_t* length);

    void Camera_Offline(short delay_s , short repeat , short planId);


};



#endif