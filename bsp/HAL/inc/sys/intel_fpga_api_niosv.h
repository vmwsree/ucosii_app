#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "intel_fpga_platform_niosv.h"
#include "io.h"
#include "sys/alt_irq.h"
#include "priv/alt_irq_table.h"

#ifdef __cplusplus
extern "C" {
#endif


unsigned int fpga_get_num_of_interfaces();
bool fpga_get_interface_at(unsigned int index, FPGA_INTERFACE_INFO *info);
FPGA_MMIO_INTERFACE_HANDLE fpga_open(unsigned int index);
void fpga_close(unsigned int index);
FPGA_INTERRUPT_HANDLE fpga_interrupt_open(unsigned int index);
void fpga_interrupt_close(unsigned int index);

static inline uint8_t fpga_read_8(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset)
{
    return IORD_8DIRECT(handle, offset);
}

static inline void fpga_write_8(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint8_t value)
{
    IOWR_8DIRECT(handle, offset ,value);
}

static inline uint16_t fpga_read_16(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset)
{
    return IORD_16DIRECT(handle, offset);
}

static inline void fpga_write_16(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint16_t value)
{
    IOWR_16DIRECT(handle, offset ,value);
}

static inline uint32_t fpga_read_32(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset)
{
    return IORD_32DIRECT(handle, offset);
}

static inline void fpga_write_32(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint32_t value)
{
    IOWR_32DIRECT(handle, offset ,value);
}

static inline uint64_t fpga_read_64(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset)
{
   uint32_t value[64/32];

   value[0] = IORD_32DIRECT(handle, offset);
   value[1] = IORD_32DIRECT(handle, offset+4);
   return *((uint64_t *)value);
}

static inline void fpga_write_64(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint64_t value)
{
   IOWR_32DIRECT(handle, offset ,  ((uint32_t)value));
   IOWR_32DIRECT(handle, offset+4 ,((uint32_t)(value>>32)));
}

static inline void fpga_read_512(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint8_t *ptr)
{
   int i;
   uint32_t *ptr_32;

   ptr_32 = (uint32_t *)ptr;

   for(i = 0; i < (512/32); ++i)
   {
     *ptr_32 = IORD_32DIRECT(handle, offset);
     ++ptr_32;
     offset += 4;
   }
}

static inline void fpga_write_512(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t offset, uint8_t *ptr)
{
   int     i;
   uint32_t *ptr_32;
  
   ptr_32 = (uint32_t *)ptr;

   for(i = 0; i < (512/32); ++i)
   {
      IOWR_32DIRECT(handle, offset ,*ptr_32);
      ++ptr_32;
      offset += 4;
   }
}


typedef void (*FPGA_ISR) ( void *isr_context );

//Interrupt controller ID is always zero
static inline int fpga_enable_interrupt(FPGA_INTERRUPT_HANDLE handle)
{
  int ret;
  //Check if ISR has been registered or not.Return 1 if ISR not registered
  if(alt_irq[handle].handler == NULL){
    ret = 1;
  }
  else{
    //If ISR enable succeeds, return 0
    ret = alt_ic_irq_enable(0, handle);
    //as per protodriver guidelines, hardware fault will return 2
    if (ret < 0){
      ret = 2;
    }
  }
  return ret;
}


//Interrupt controller ID is always zero
static inline int fpga_disable_interrupt(FPGA_INTERRUPT_HANDLE handle)
{
  int ret = -2;
  //Check if ISR has been registered or not.Return 1 if ISR not registered
  if(alt_irq[handle].handler == NULL){
    ret = 1;
  }
  else{
    if( alt_ic_irq_enabled(0,handle) ){
      //If ISR disable succeeds, return 0
      ret = alt_ic_irq_disable(0, handle);
      //as per protodriver guidelines, hardware fault will return 3
      if (ret < 0){
        ret = 3;
      }
    }
    else{
      //Return 2 if irq not enabled
      ret = 2;
    }
  }
  return ret;
}

static inline int fpga_register_isr(FPGA_INTERRUPT_HANDLE handle, FPGA_ISR isr, void *isr_context)
{

  int ret = -2;

  if(isr){
    //Check prior to registration. Return 1 if ISR is previously registered.
    // Overwrite the ISR if already registered.
    if(alt_irq[handle].handler != NULL){
      ret = 1;
    }
    
    //Registration
    alt_ic_isr_register(0, handle, isr, isr_context, NULL);

    //Check after registration. Return 0 if ISR registration is successful, else return eror -2.
    // -1 from alt_ic_isr_register has been remaped to -2
    if(ret != 1){
    ret = (alt_irq[handle].handler == NULL) ? -2:0;
    }

  }else{
    //ISR null pointer check failed. Dont register the ISR and return with error code
    ret = -1;
  }
  return ret;
}

void *fpga_malloc(FPGA_MMIO_INTERFACE_HANDLE handle, uint32_t size);
void fpga_free(FPGA_MMIO_INTERFACE_HANDLE handle, void *address);
FPGA_PLATFORM_PHYSICAL_MEM_ADDR_TYPE fpga_get_physical_address(void *address);
int fpga_msg_printf(FPGA_MSG_PRINTF_TYPE type, const char * format, ... );
void fpga_throw_runtime_exception(const char *function, const char *file, int lineno, const char * format, ... );


#ifdef __cplusplus
}
#endif
