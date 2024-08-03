#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>

bool fpga_platform_init(unsigned int argc, const char *argv[]);
void fpga_platform_cleanup();
typedef enum
{
    FPGA_MSG_PRINTF_INFO,         //!< In default printf, this type of message is prefixed with "Info: "
    FPGA_MSG_PRINTF_WARNING,      //!< In default printf, this type of message is prefixed with "Warning: "
    FPGA_MSG_PRINTF_ERROR,        //!< In default printf, this type of message is prefixed with "Error: "
    FPGA_MSG_PRINTF_DEBUG         //!< In default printf, this type of message is prefixed with "Debug: "
} FPGA_MSG_PRINTF_TYPE;
typedef int (*FPGA_MSG_PRINTF) ( FPGA_MSG_PRINTF_TYPE type, const char * format, va_list args);
void fpga_platform_register_printf(FPGA_MSG_PRINTF msg_printf);
typedef void (*FPGA_RUNTIME_EXCEPTION_HANDLER) ( const char *function, const char *file, int lineno, const char * format, va_list args);
void fpga_platform_register_runtime_exception_handler(FPGA_RUNTIME_EXCEPTION_HANDLER handler);


#ifdef __cplusplus
}
#endif
