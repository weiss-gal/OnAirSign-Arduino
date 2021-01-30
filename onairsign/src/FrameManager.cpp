#include "FrameManager.h"
#include "Logger.h"
#include "Arduino.h"

FrameManager::FrameManager(Logger *logger){
  this->logger = logger;

  for (int i = 0; i < MAX_FRAME_TASKS + 1; i++) 
    frameTasks[i] = {0};
}

int FrameManager::RegisterFrameTask(FrameTaskType task, int freq){
  for (int i = 0; i < MAX_FRAME_TASKS; i++){
    if (!frameTasks[i].frameTaskCB) {
      frameTasks[i].frameTaskCB = task;
      frameTasks[i].freq = freq;
      return 0;
    }
  }

  return 1;
}

void FrameManager::RunAllTasks(FrameTask_t *frameTasks){
  for (int i = 0; frameTasks[i].frameTaskCB; i++){
    if (frameCount % frameTasks[i].freq)
      return;
    frameTasks[i].frameTaskCB();  
  }
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
     RunAllTasks(frameTasks);  
     this->frameCount++;
   }
}