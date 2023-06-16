#include "../src/Esp_32/Camera.h"
#include "../src/Websocket/WebSocket.h"





int Seq_ID  ;
int plan_ID; 


camera_config_t config;
WebSocket con("127.0.0.1");
Camera cam(config);

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_PIN GPIO_NUM_4


const int MAX_FILES = 10;






void setup() {

Serial.begin(115200);

   // Initialize camera
   
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;


  if(psramFound()){
    config.frame_size = FRAMESIZE_XGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  cam.SetupUART();


 





   
  gpio_pad_select_gpio(FLASH_PIN);
  gpio_set_direction(FLASH_PIN, GPIO_MODE_OUTPUT);


cam._Image_Number = 0 ;  
}

void loop() {
  delay(1000);
  Serial.println("GG");
  // UART Variable which describe the type of operation if (Online Or Offline)
  if(Serial2.available()){
  
   cam.ReadUARTData();

  plan_ID = 0;
  Serial.println();
  Seq_ID= Camera::bufferUART[0];
  plan_ID= (plan_ID << 8 )+ Camera::bufferUART[2];
  plan_ID= (plan_ID << 8 )+ Camera::bufferUART[3]; 
  Serial.println(plan_ID);
  switch (Camera::bufferUART[1])
  {
     //Online Session Capture Every 5 Second and send it by socket
  case 1:
  {
    size_t length = 0;
     gpio_set_level(FLASH_PIN, HIGH);
    byte* image_data = cam.Camera_online(&length);
    Serial.println(length);
    if (!image_data) 
    {
      Serial.println("Failed to capture image");
      return;
    }
     gpio_set_level(FLASH_PIN, LOW);
    // send in socket
    //con.SendBinary(image_data ,length , 1024);
    // Delete  an array
    
    delete[] image_data;
    delay(300);
    break;
  }
  


  // Offline Session
  case 0:
  {

  gpio_set_level(FLASH_PIN, HIGH);
    //  First Paramter delay between every image in second , Second paramter Reapet and finally paraamter the plan id All get from UART
    cam.Camera_Offline(plan_ID);
    Serial.println("OFF");

  
 
    break;
  }

// telemetry
  case 2:
  {
      // Create arrays to hold the file contents and sizes

    cam.removeAllFilesInFolder(plan_ID);
  }

  default:
  break;
   
  }
 
}
}