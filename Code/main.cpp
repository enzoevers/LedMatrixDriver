#include "./DisplayControl/DisplayCommunication.h"

#define __HAL_RCC_GPIOE_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

int main(){
    //SysTick_Config(SystemCoreClock);
    __HAL_RCC_GPIOE_CLK_ENABLE();

    SetGpio(true);
    while(true){}
    return 0;
}