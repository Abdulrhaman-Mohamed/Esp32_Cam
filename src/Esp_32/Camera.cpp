#include "Camera.h"

#define Second 1000

Camera::Camera( camera_config_t config) {
  _config = config;
}

char * Camera::Camera_online(size_t* length) {

  camera_fb_t *fb = NULL;

  // Capture an image
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return NULL;
  }
 // Get image length and assign it to the length pointer
  *length = fb->len;

// Allocate memory for image data
  char * image_data = new char[*length];

  // Copy image data to buffer
  memcpy(image_data, fb->buf, *length);

  // Release frame buffer
  esp_camera_fb_return(fb);

  return image_data;

}


void Camera::Camera_Offline(short delay_s , short repeat , short planId)
{
  // Initialize the SD card
  if(!SD_MMC.begin()) {
    Serial.println("SD card initialization failed!");
    return;
  }

  // Create a directory to store the images
  String image_dir = "/images_"+String(planId);
  if(!SD_MMC.exists(image_dir)) {
    SD_MMC.mkdir(image_dir);
  }

  // Capture and store the images
  for(int i = 1; i <= repeat; i++) {
    // Wait for the delay between every capture
    delay(delay_s * Second );

    // Take the picture
    camera_fb_t *fb = esp_camera_fb_get();

    // Save the picture to the SD card
    String filename = image_dir+"/image_" + String(i) + ".jpg";
    File file = SD_MMC.open(filename, FILE_WRITE);
    if(file) {
      file.write(fb->buf, fb->len);
      file.close();
    } else {
      Serial.println("Failed to open file for writing");
    }

    // Free the memory used by the framebuffer
    esp_camera_fb_return(fb);
  }

  // Unmount the SD card
  SD_MMC.end();


}