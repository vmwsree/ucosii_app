#include "sys/intel_fpga_platform_api_niosv.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int niosv_fpga_platform_default_printf(FPGA_MSG_PRINTF_TYPE type, const char * format, va_list args);
static void niosv_fpga_platform_default_runtime_exception_handler(const char *function, const char *file, int lineno, const char * format, va_list args);

FPGA_MSG_PRINTF g_niosv_fpga_platform_printf = niosv_fpga_platform_default_printf;
FPGA_RUNTIME_EXCEPTION_HANDLER  g_niosv_fpga_platform_runtime_exception_handler = niosv_fpga_platform_default_runtime_exception_handler;

void fpga_platform_register_printf(FPGA_MSG_PRINTF msg_printf)
{
    g_niosv_fpga_platform_printf = msg_printf;
}

int niosv_fpga_platform_default_printf(FPGA_MSG_PRINTF_TYPE type, const char * format, va_list args)
{
    int ret = 0;
    
#ifdef INTEL_FPGA_MSG_PRINTF_ENABLE
    switch(type)
    {
        case FPGA_MSG_PRINTF_INFO:
#ifdef INTEL_FPGA_MSG_PRINTF_ENABLE_INFO
            fputs( "INFO: ", stdout );
            ret = vprintf( format, args );
            fputs( "\n", stdout );
#endif            
            break;
        case FPGA_MSG_PRINTF_WARNING:
#ifdef INTEL_FPGA_MSG_PRINTF_ENABLE_WARNING
            fputs( "WARNING: ", stdout );
            ret = vprintf( format, args );
            fputs( "\n", stdout );
#endif            
            break;
        case FPGA_MSG_PRINTF_ERROR:
#ifdef INTEL_FPGA_MSG_PRINTF_ENABLE_ERROR
            fputs( "ERROR: ", stdout );
            ret = vprintf( format, args );
            fputs( "\n", stdout );
#endif            
            break;
        case FPGA_MSG_PRINTF_DEBUG:
#ifdef INTEL_FPGA_MSG_PRINTF_ENABLE_DEBUG
            fputs( "DEBUG: ", stdout );
            ret = vprintf( format, args );
            fputs( "\n", stdout );
#endif            
            break;
        default:
            fpga_throw_runtime_exception( __FUNCTION__, __FILE__, __LINE__, "invalid message print type is specified." );
    }
    
#endif //INTEL_FPGA_MSG_PRINTF_ENABLE

    return ret;
}

void niosv_fpga_platform_default_runtime_exception_handler(const char *function, const char *file, int lineno, const char * format, va_list args)
{
    printf( "Exception occured in %s() at line %d in %s due to ", function, lineno, file );
    vprintf( format, args );
    printf( "\n\nApplication is terminated.\n" );
    
    exit(1);
}
