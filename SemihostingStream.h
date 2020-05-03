
#ifndef SEMIHOSTINGSTREAM_H
#define SEMIHOSTINGSTREAM_H

/* 
 * Arduino class to write to and read from semihosting console (stdin/stdout, buffered)
 * 2020 Koen De Vleeschauwer
 * No copyright asserted
 */

#include <Stream.h>

class SemihostingStream : public Stream
{
  private:
    int stdin_handle;
    int stdout_handle;
    static const uint8_t inBufferSize = 64;
    static const uint8_t outBufferSize = 64;
    uint8_t inBuffered;
    uint8_t inReadPos;
    uint8_t inBuffer[inBufferSize];
    uint8_t outBuffered;
    uint8_t outBuffer[outBufferSize];
    void fillBuffer();
  public:
    SemihostingStream();
    ~SemihostingStream();
    virtual size_t write(uint8_t ch);
    virtual void flush();
    virtual int read();
    virtual int available();
    virtual int peek();
};
#endif

/* not truncated */
