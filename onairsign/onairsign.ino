#include <MD_MAX72xx.h>
#include <SPI.h>

#include "src/FrameManager.h"
#include "src/DisplayManager.h"
#include "src/Logger.h"
#include "src/SerialManager.h"
#include "src/MessageHandler.h"
#include "src/ConnectivityManager.h"
 
// LED matrix related stuff
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//TODO: we don't really have an isConnected implementation yet
static DisplayState_t displayState = {true, false, false, false};
static FrameManager *fm;
static Logger *logger;
static DisplayManager *dm;
static SerialManager *sm;
static MessageHandler *mh;
static ConnectivityManager *cm;

void refreshDisplay() {
  dm->RefreshDisplay();
}

void tickConnectivityManager(){
  cm->Tick();
}

void pingConnectivityManager(){
  cm->Ping();
}

void connectivityStateUpdate(bool isConnected){
  // If display is not connected, everything defaults to false
  if (!isConnected) {
    displayState.isAudioPlaying = false;
    displayState.isAudioCapturing = false;
    displayState.isCameraCapturing = false;
  }
  displayState.isConnected = isConnected;
  dm->SetDisplayState(displayState);
}

void requestDisplayStateUpdate(RequestedDisplayState_t state) {
  displayState.isAudioPlaying = state.isAudioPlaying;
  displayState.isAudioCapturing = state.isAudioCapturing;
  displayState.isCameraCapturing = state.isCameraCapturing;
  dm->SetDisplayState(displayState);
}

void setup() {
  /* Hardware initalization */
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  
  logger = new Logger(LOG_LEVEL_DEBUG);
  
  dm = new DisplayManager(&M, logger);
  //dm->SetDisplayState(displayState);
  cm = new ConnectivityManager(connectivityStateUpdate, logger);
  fm = new FrameManager(logger);
  fm->RegisterFrameTask(refreshDisplay, 1); 
  fm->RegisterFrameTask(tickConnectivityManager, 1);
  mh = new MessageHandler(requestDisplayStateUpdate, pingConnectivityManager, logger);
  sm = new SerialManager(&Serial, mh, logger);
}

void loop() {
  fm->ProcessFrame();
  sm->ProcessInput();
}
