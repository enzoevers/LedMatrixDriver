#include "DisplayCommunication.h"

void SetGpio(bool on) {
    // ===== Config =====    
    // Reset 
    CLEAR_REG(GPIOE->MODER);
    CLEAR_REG(GPIOE->PUPDR);
    CLEAR_REG(GPIOE->OTYPER);
    CLEAR_REG(GPIOE->OSPEEDR);

    SET_BIT(GPIOE->MODER, 0b1 << GPIO_MODER_MODER13_Pos);
    CLEAR_BIT(GPIOE->MODER, 0b1 << (1+GPIO_MODER_MODER13_Pos));
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_13);
    CLEAR_BIT(GPIOE->OSPEEDR, 0b11 << GPIO_OSPEEDER_OSPEEDR13_Pos);
    CLEAR_BIT(GPIOE->PUPDR, 0b11 << GPIO_PUPDR_PUPDR13_Pos);

    // ===== Data =====
    if(on) {
        SET_BIT(GPIOE->ODR, GPIO_ODR_13); 
    } else {
        CLEAR_BIT(GPIOE->ODR, GPIO_ODR_13); 
    }

    // ===== Set/Reset =====
    //GPIOE->BSRR
}