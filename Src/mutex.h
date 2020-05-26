#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "cmsis_os.h"

class Mutex {
protected:
  osMutexId_t handle_{};
  osMutexAttr_t attributes_;
public:
  explicit Mutex(const char *name) : attributes_({name, 0, nullptr, 0}) {
    this->handle_ = osMutexNew(&this->attributes_);
  }

  void acquire() {
    osMutexAcquire(this->handle_, portMAX_DELAY);
  }

  void release() {
    osMutexRelease(this->handle_);
  }
};

#endif //_MUTEX_H_
