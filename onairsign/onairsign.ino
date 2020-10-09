#include <MD_MAX72xx.h>
#include <SPI.h>

#include "src/FrameManager.h"
#include "src/DisplayManager.h"
#include "src/Logger.h"
#include "src/SerialManager.h"
#include "src/MessageHandler.h"

// LED matrix related stuff
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

static FrameManager *fm;
static Logger *logger;
static DisplayManager *dm;
static SerialManager *sm;
static MessageHandler *mh;

void setup() {
  /* Hardware initalization */
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  
  logger = new Logger(LOG_LEVEL_DEBUG);
  dm = new DisplayManager(&M, logger);
  fm = new FrameManager(logger);
  fm->RegisterFrameTask(refreshDisplay);
  mh = new MessageHandler(logger);
  sm = new SerialManager(&Serial, mh, logger);
  
  delay(2000); 
  logger->Log(LOG_LEVEL_INFO, "Started");
}



// frame is a processing window of FRAME_PERIOD_MS length
void refreshDisplay() {
  dm->RefreshDisplay();
}

void loop() {
  static int message_index = 0;

  fm->ProcessFrame();
  sm->ProcessInput();
}
