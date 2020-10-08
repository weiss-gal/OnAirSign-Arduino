// ensure this library description is only included once
#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include "Logger.h"

#define MAX_FRAME_TASKS 4
#define FRAME_PERIOD_MS 100

typedef void (*FrameTaskType)();

// library interface description
class FrameManager
{
  // user-accessible "public" interface
  public:
    FrameManager(Logger *logger);
    int RegisterFrameTask(FrameTaskType task);
    void ProcessFrame();

  // library-accessible "private" interface
  private:  
    FrameTaskType frameTasks[MAX_FRAME_TASKS + 1]; // null terminated
    long next_frame = 0; // start value of millis()
    void RunAllTasks(FrameTaskType *frameTasks);
    Logger *logger;
};

#endif