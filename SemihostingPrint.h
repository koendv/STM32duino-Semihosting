#ifndef SEMIHOSTINGPRINT_H
#define SEMIHOSTINGPRINT_H

/* 
 * Arduino class to write to semihosting debug output (stderr, unbuffered)
 * 2020 Koen De Vleeschauwer
 * No copyright asserted
 */

#include <Print.h>

class SemihostingPrint : public Print
{
  private:
    int stderr_handle;
  public:
    SemihostingPrint();
    ~SemihostingPrint();
    virtual size_t write(uint8_t ch);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual void flush();
};
#endif

/* not truncated */
