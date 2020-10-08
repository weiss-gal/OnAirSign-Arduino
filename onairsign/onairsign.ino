#include <MD_MAX72xx.h>
#include <SPI.h>

#include "src/FrameManager.h"
#include "src/DisplayManager.h"
#include "src/Logger.h"
#include <SPI.h>


// LED matrix related stuff
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Serial management
#define MSG_BUFFER_LEN 128

static FrameManager *fm;
static Logger *logger;
static DisplayManager *dm;

void setup() {
  /* Hardware initalization */
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
 
  logger = new Logger();
  dm = new DisplayManager(&M, logger);
  fm = new FrameManager(logger);
  fm->RegisterFrameTask(refreshDisplay);
  delay(2000); 
  logger->Log(LOG_LEVEL_INFO, "Started");
  
}



// frame is a processing window of FRAME_PERIOD_MS length
void refreshDisplay() {
  dm->RefreshDisplay();
}

// The message 'msg' is a null terminated string and is only available
// during handle_msg (you cannot copy the pointer to the message for later use)
void handle_msg(unsigned char *msg) {
  char buffer[256];
  snprintf(buffer, 256, "Handling message '%s'", msg);
  //Serial.println(buffer);
}

void send_messsage_trace(const char *trace_message) {
  //Serial.print("TRACE ");
  //Serial.print(trace_message);
  //Serial.print("\r");
}

void loop() {
  //TODO: fix this to calculate next frame using millis()
  static unsigned char message_buffer[MSG_BUFFER_LEN] = {0};
  static int message_index = 0;

  
  fm->ProcessFrame();

  if (Serial.available() > 0)  {
    int input = Serial.read();
    if (input == 13) {
      message_buffer[message_index] = 0;
      handle_msg(message_buffer);
      message_index = 0;
    } else {
      message_buffer[message_index++] = input;
    }

    if (message_index == MSG_BUFFER_LEN - 1) {
      message_index = 0;
      send_messsage_trace("message buffer overflow");
    }
  }
}
