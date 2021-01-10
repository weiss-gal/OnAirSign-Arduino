// ensure this library description is only included once
#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include "Logger.h"

#define MAX_FRAME_TASKS 4
#define FRAME_PERIOD_MS 100

typedef void (*FrameTaskType)();
typedef struct {
  FrameTaskType frameTaskCB;
  int freq; // Task frequency in frames 
} FrameTask_t;

// library interface description
class FrameManager
{
  // user-accessible "public" interface
  public:
    FrameManager(Logger *logger);
    int RegisterFrameTask(FrameTaskType task, int freq);
    void ProcessFrame();

  // library-accessible "private" interface
  private:  
    FrameTask_t frameTasks[MAX_FRAME_TASKS + 1]; // null terminated
    long next_frame = 0; // start value of millis()
    int frameCount = 0;
    void RunAllTasks(FrameTask_t *frameTasks);
    Logger *logger;
};

#endif