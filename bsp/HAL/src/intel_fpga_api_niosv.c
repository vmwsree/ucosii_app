#include "sys/intel_fpga_api_niosv.h"
#include <stdarg.h>

int fpga_msg_printf(FPGA_MSG_PRINTF_TYPE type, const char * format, ...)
{
    int     ret;
    
    va_list args;
    va_start(args, format);
    
    ret = g_niosv_fpga_platform_printf(type, format, args);
    
    va_end(args);


    return ret;
}

//JoP specific at the moment
unsigned int fpga_get_num_of_interfaces()
{
    return 1;
}

//JoP specific at the moment
FPGA_MMIO_INTERFACE_HANDLE fpga_open(unsigned int index)
{
    
    return index;
}

void fpga_throw_runtime_exception(const char *function, const char *file, int lineno, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    
    g_niosv_fpga_platform_runtime_exception_handler(function, file, lineno, format, args);
    
    va_end(args);
}
