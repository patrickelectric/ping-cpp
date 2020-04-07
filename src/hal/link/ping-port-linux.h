#pragma once

#include "ping-port.h"

#include <cstdio>

class PingPortLinux : public PingPort {
public:
  PingPortLinux(const char* filename);
  ~PingPortLinux();

  virtual int read(uint8_t* buffer, int nBytes) override final;
  void sendBreak(int milliseconds);
  bool setBaudrate(int baudrate);
  virtual int write(const uint8_t* data, int nBytes) override final;

private:
  char _filename[256];
  std::FILE* _handle;
};
