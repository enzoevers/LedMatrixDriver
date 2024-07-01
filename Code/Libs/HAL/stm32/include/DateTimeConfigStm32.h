#pragma once

#include <stdint.h>

namespace HAL::STM32 {

struct DateTimeConfig {
    volatile uint32_t* PWR_CR;
    volatile uint32_t* RTC_WPR;
    volatile uint32_t* RTC_ISR;
    volatile uint32_t* RTC_CR;
    volatile uint32_t* RTC_TR;
    volatile uint32_t* RTC_DR;
    volatile uint32_t* RTC_SSR;
    uint32_t PWR_CR_DBP_pos;
    uint32_t RTC_ISR_INIT_pos;
    uint32_t RTC_ISR_INITF_pos;
    uint32_t RTC_CR_FMF_pos;

    auto operator==(const DateTimeConfig& other) const -> bool {
        return PWR_CR == other.PWR_CR && RTC_WPR == other.RTC_WPR && RTC_ISR == other.RTC_ISR &&
               RTC_CR == other.RTC_CR && RTC_TR == other.RTC_TR && RTC_DR == other.RTC_DR && RTC_SSR == other.RTC_SSR &&
               PWR_CR_DBP_pos == other.PWR_CR_DBP_pos && RTC_ISR_INIT_pos == other.RTC_ISR_INIT_pos &&
               RTC_ISR_INITF_pos == other.RTC_ISR_INITF_pos && RTC_CR_FMF_pos == other.RTC_CR_FMF_pos;
    }

    auto ContainsNullptr() const -> bool {
        return PWR_CR == nullptr || RTC_WPR == nullptr || RTC_ISR == nullptr || RTC_SSR == nullptr ||
               RTC_TR == nullptr || RTC_DR == nullptr || RTC_CR == nullptr;
    }
};

}  // namespace HAL::STM32