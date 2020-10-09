#include "FrameManager.h"
#include "Logger.h"
#include "Arduino.h"

FrameManager::FrameManager(Logger *logger){
  this->logger = logger;

  for (int i = 0; i < MAX_FRAME_TASKS + 1; i++) 
    frameTasks[i] = NULL;
}

int FrameManager::RegisterFrameTask(FrameTaskType task){
  for (int i = 0; i < MAX_FRAME_TASKS; i++){
    if (!frameTasks[i]) {
      frameTasks[i] = task;
      return 0;
    }
  }

  return 1;
}

void FrameManager::RunAllTasks(FrameTaskType *frameTasks){
  for (int i = 0; frameTasks[i]; i++)
    frameTasks[i]();  
}

void FrameManager::ProcessFrame(){
  
  //Converting to signed making wrap around arithmetics very simple
  union MillisResult_t {
    unsigned long asUnsigned;
    long asSigned;  
  } now;

  // TODO: force wrap-around after one minute
  now.asUnsigned = millis();
 
  long delta = now.asSigned - this->next_frame;

  if (delta > 0) {
     this->next_frame = (now.asSigned / FRAME_PERIOD_MS + 1) * FRAME_PERIOD_MS;
     //logger->Log(LOG_LEVEL_DEBUG, "Starting frame at signed time %ld, next frame at %ld", now.asSigned, this->next_frame);
     this->RunAllTasks(frameTasks);
   }
}