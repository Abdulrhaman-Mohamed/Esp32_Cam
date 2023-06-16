#include "Camera.h"

#define BAUD_RATE 115200
#define TXD_PIN 12
#define RXD_PIN 13


byte *Camera::bufferUART = new byte[5];

int32_t Camera::_Image_Number = 0 ;

Camera::Camera( camera_config_t config ) {
  _config = config;
  
}



byte * Camera::Camera_online(size_t* length) {

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
  byte * image_data = new byte[*length];

  // Copy image data to buffer
  memcpy(image_data, fb->buf, *length);

  // Release frame buffer
  esp_camera_fb_return(fb);

  return image_data;

}


void Camera::Camera_Offline(const int planId)
{


     if (!SD_MMC.begin()) {
    Serial.println("Failed to mount SD card");
    return;
  }


  // Create a directory to store the images
  String image_dir = "/images_"+String(planId);
  if(!SD_MMC.exists(image_dir)) {
    SD_MMC.mkdir(image_dir);
  }

  // Capture and store the images




    // Take the picture
    camera_fb_t *fb = esp_camera_fb_get();

    // Save the picture to the SD card
    String filename = image_dir+"/image_" + String(_Image_Number)+".txt"   ;
    File file = SD_MMC.open(filename, FILE_WRITE);
    if(file) {
       for (size_t i = 0; i < fb->len; i++) {
        file.printf("%02X", fb->buf[i]);
  }
      //file.write(fb->buf, fb->len);
      file.close();
    } else {
      Serial.println("Failed to open file for writing");
    }

    // Free the memory used by the framebuffer
    esp_camera_fb_return(fb);
  
  _Image_Number++;

SD_MMC.end();
}






void Camera:: removeAllFilesInFolder(const  int planID ) {

     if (!SD_MMC.begin()) {
    Serial.println("Failed to mount SD card");
    return;
  }
  
  String folderPath = "/images_"+String(planID) ;

  // Loop through all files in the folder
  File dir = SD_MMC.open(folderPath);
  if (!dir || !dir.isDirectory()) {
    Serial.println("Failed to open directory");
    return;
  }

  int fileIndex = 0;

  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      Serial.println("No more files");
      break;
    }

    if (file.isDirectory()) {
      file.close();
      continue;
    }

    String fileName = file.name();

    // Get the size of the file
  size_t fileSize = file.size();
  
  // Allocate memory for the byte array
  char* byteArray = new char[fileSize];
  
  // Read the file into the byte array
  file.readBytes(byteArray, fileSize);

    


 /*

  // Print the contents of the byte array to serial output
  Serial.println("Contents of file:");
  for (size_t i = 0; i < fileSize; i++) {
    Serial.print(byteArray[i]);
    Serial.print(" ");
  }
  Serial.println();
  */

     // Delete the file content array
     delete[] byteArray;
    
    SD_MMC.remove(fileName);

    file.close();

    fileIndex++;
  }

  dir.close();

  // Remove the folder
  SD_MMC.rmdir(folderPath);
  Serial.println("Done");
  _Image_Number=0;
  SD_MMC.end();
}




void Camera::SetupUART()
{
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RXD_PIN, TXD_PIN);

}

void Camera::ReadUARTData()
{
 Serial2.readBytes(Camera::bufferUART , 4);
}
