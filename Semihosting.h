#include <Stream.h>

#ifndef SEMIHOSTINGSTREAM_H
#define SEMIHOSTINGSTREAM_H

/* 
 * Arduino class to write to and read from semihosting console 
 */

class SemihostingStream : public Stream
{
  private:
    int stdin_handle;
    int stdout_handle;
    char peekbuf;
    bool peekbuf_valid;
  public:
    SemihostingStream();
    ~SemihostingStream();
    virtual size_t write(uint8_t ch);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual void flush();
    virtual int read();
    virtual size_t readBytes(char *buf, size_t length);
    virtual int available();
    virtual int peek();
};
#endif

/* not truncated */
