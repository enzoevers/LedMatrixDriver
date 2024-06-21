#pragma once

#include <array>

#include "IDisplay.h"
#include "IHanoverOL037A.h"
#include "Monochrome.h"

class HanoverOL037A : public IHanoverOL037A, public IDisplay<Monochrome> {
   public:
    HanoverOL037A();
    ~HanoverOL037A() override = default;

    //=========================
    // IHanoverOL037A
    //=========================
    auto SetGPIOInterface(const HanoverOL037A_GPIOInterface& gpioInterface) -> bool override;
    auto GetGPIOInterface() const -> const HanoverOL037A_GPIOInterface& override;
    auto SetDelayManager(HAL::IDelay* const delayManager) -> void override;
    auto GetDelayManager() const -> HAL::IDelay* override;

    //=========================
    // IDisplay<Monochrome>
    //=========================
    auto GetResolution() const -> Vec2D override;
    auto GetCurrentDisplay(PixelArea<Monochrome>& pixelArea) const -> void override;
    auto GetCurrentPixelArea(const Vec2D topLeftoordinate, PixelArea<Monochrome>& pixelArea) const -> bool override;
    auto SetPixel(const Vec2D coordinate, const Monochrome color) -> bool override;
    auto SetArea(const Vec2D topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool override;
    auto UpdateDisplay() -> bool override;

    //=========================
    // Locals
    //=========================
    auto GetNumberOfPanels() const -> uint32_t;
    auto GetNumberOfSectionsPerPanel() const -> uint32_t;

    /*!
        \return The byte that can be shifted from bit 0 till 7 into the selection shift register
    */
    auto GetPanelAndSectionSelectionByte(uint32_t panel, uint32_t section) const -> uint8_t;
    auto SelectPanelAndSection(uint32_t panel, uint32_t section) const -> bool;
    auto WritePixels(uint32_t panel, uint32_t section) const -> bool;

   private:
    auto ClockInData(bool state) const -> void;
    auto SetClockInPanelSectionSelectMode() const -> void;
    auto SetClockInPixelDataMode() const -> void;

    static constexpr Vec2D m_resolution{160, 19};
    HanoverOL037A_GPIOInterface m_pGpioInterface;
    HAL::IDelay* m_pDelayManager;
    std::array<Monochrome, (m_resolution.x * m_resolution.y)> m_pixelData;
};