#include "HanoverOL037A.h"

#include <cstring>  // std::memcpy

constexpr Vec2D HanoverOL037A::m_resolution;
static constexpr int delayAfterIO_us = 5;

HanoverOL037A::HanoverOL037A() : m_pDelayManager(nullptr) { std::memset(m_pixelData.data(), 0, sizeof(m_pixelData)); }

//=========================
// IHanoverOL037A
//=========================

auto HanoverOL037A::SetGPIOInterface(const HanoverOL037A_GPIOInterface& gpioInterface) -> bool {
    if (gpioInterface.ContainsNullptr()) {
        return false;
    }

    m_pGpioInterface = gpioInterface;
    return true;
}

auto HanoverOL037A::GetGPIOInterface() const -> const HanoverOL037A_GPIOInterface& { return m_pGpioInterface; }

auto HanoverOL037A::SetDelayManager(IDelay* const delayManager) -> void { m_pDelayManager = delayManager; }

auto HanoverOL037A::GetDelayManager() const -> IDelay* { return m_pDelayManager; }

//=========================
// IDisplay<Monochrome>
//=========================
auto HanoverOL037A::GetResolution() const -> Vec2D { return HanoverOL037A::m_resolution; }

auto HanoverOL037A::GetCurrentDisplay(PixelArea<Monochrome>& pixelArea) const -> void {
    pixelArea.size = GetResolution();
    std::memcpy(pixelArea.pixels, m_pixelData.data(), m_resolution.x * m_resolution.y);
}

auto HanoverOL037A::GetCurrentPixelArea(const Vec2D topLeftoordinate, PixelArea<Monochrome>& pixelArea) const -> bool {
    const auto resolution = GetResolution();

    if (topLeftoordinate.x >= resolution.x || topLeftoordinate.y >= resolution.y ||
        (topLeftoordinate.x + pixelArea.size.x - 1) >= resolution.x ||
        (topLeftoordinate.y + pixelArea.size.y - 1) >= resolution.y || pixelArea.size.x == 0 || pixelArea.size.y == 0 ||
        pixelArea.pixels == nullptr) {
        return false;
    }

    for (auto y = 0; y < pixelArea.size.y; y++) {
        const auto startIndexSrc = ((y + topLeftoordinate.y) * resolution.x) + topLeftoordinate.x;
        const auto startIndexDst = y * pixelArea.size.x;
        std::memcpy(&pixelArea.pixels[startIndexDst], &m_pixelData.at(startIndexSrc),
                    pixelArea.size.x * sizeof(Monochrome));
    }

    return true;
}

auto HanoverOL037A::SetPixel(const Vec2D coordinate, const Monochrome color) -> bool {
    const auto resolution = GetResolution();

    if (coordinate.x >= resolution.x || coordinate.y >= resolution.y) {
        return false;
    }

    std::size_t index = coordinate.y * resolution.x + coordinate.x;
    m_pixelData.at(index) = color;

    return true;
}

auto HanoverOL037A::SetArea(const Vec2D topLeftoordinate, const PixelArea<Monochrome>& pixelArea) -> bool {
    const auto resolution = GetResolution();

    if (topLeftoordinate.x >= resolution.x || topLeftoordinate.y >= resolution.y ||
        (topLeftoordinate.x + pixelArea.size.x - 1) >= resolution.x ||
        (topLeftoordinate.y + pixelArea.size.y - 1) >= resolution.y || pixelArea.size.x == 0 || pixelArea.size.y == 0 ||
        pixelArea.pixels == nullptr) {
        return false;
    }

    for (auto y = 0; y < pixelArea.size.y; y++) {
        const auto startIndexSrc = y * pixelArea.size.x;
        const auto startIndexDst = ((y + topLeftoordinate.y) * resolution.x) + topLeftoordinate.x;
        std::memcpy(&m_pixelData.at(startIndexDst), &pixelArea.pixels[startIndexSrc],
                    pixelArea.size.x * sizeof(Monochrome));
    }

    return true;
}

auto HanoverOL037A::UpdateDisplay() -> bool {
    // TODO: Only update sections that are changes since last update

    const auto nrPanels = GetNumberOfPanels();
    const auto nrSectionsPerPanel = GetNumberOfSectionsPerPanel();

    m_pGpioInterface.latch->SetState(0);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

    for (auto p = 0; p < nrPanels; ++p) {
        for (auto s = 0; s < nrSectionsPerPanel; ++s) {
            if (!SelectPanelAndSection(p, s)) {
                return false;
            }
            if (!WritePixels(p, s)) {
                return false;
            }
        }
    }

    m_pGpioInterface.latch->SetState(1);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
    m_pGpioInterface.latch->SetState(0);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

    // TODO: remove this once the the `enableDisplay()` function is implemented
    m_pGpioInterface.ledOE->SetState(0);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

    return true;
}

//=========================
// Locals
//=========================

auto HanoverOL037A::GetNumberOfPanels() const -> uint32_t { return 4; }

auto HanoverOL037A::GetNumberOfSectionsPerPanel() const -> uint32_t {
    // Note that even though the hardware has physically 5 sections, the grouping of clocks makes this effectively
    // only 3 sections as far as the software is concerned.
    return 3;
}

auto HanoverOL037A::GetPanelAndSectionSelectionByte(uint32_t panel, uint32_t section) const -> uint8_t {
    if (panel >= GetNumberOfPanels() || section >= GetNumberOfSectionsPerPanel()) {
        return 0;
    }

    return ((section & 0b11) | (~(0b1 << (6 - (panel & 0b111111) - 1)) << 2));
}

inline auto clockInData(bool state) -> void {
    // TODO: first make sure that the tests still work before implementing this
}

auto HanoverOL037A::SelectPanelAndSection(uint32_t panel, uint32_t section) const -> bool {
    if (panel >= GetNumberOfPanels() || section >= GetNumberOfSectionsPerPanel() ||
        m_pGpioInterface.ContainsNullptr()) {
        return false;
    }

    m_pGpioInterface.clk->SetState(1);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

    const auto byteToWrite = GetPanelAndSectionSelectionByte(panel, section);
    for (auto i = 0; i < 8; i++) {
        m_pGpioInterface.clkEn->SetState(0);
        m_pGpioInterface.data->SetState((byteToWrite >> i) & 0b1);
        // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
        m_pGpioInterface.clkEn->SetState(1);
        // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
    }

    return true;
}

auto HanoverOL037A::WritePixels(uint32_t panel, uint32_t section) const -> bool {
    if (panel >= GetNumberOfPanels() || section >= GetNumberOfSectionsPerPanel() ||
        m_pGpioInterface.ContainsNullptr()) {
        return false;
    }

    m_pGpioInterface.clk->SetState(0);
    // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

    constexpr uint32_t columns = 40;
    const uint32_t rows = (section == 2) ? 3 : 8;

    for (auto c = 0; c < columns; c++) {
        m_pGpioInterface.clkSelEn->SetState(0);
        // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);

        std::array<Monochrome, 8> columnData;
        auto pixelArea = PixelArea<Monochrome>{{1, rows}, columnData.data()};
        const auto topLeftCoordinate = Vec2D{(columns * panel) + c, section * 8};

        GetCurrentPixelArea(topLeftCoordinate, pixelArea);

        for (auto r = 0; r < rows; ++r) {
            m_pGpioInterface.clkEn->SetState(0);
            m_pGpioInterface.data->SetState(pixelArea.pixels[r]);
            // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
            m_pGpioInterface.clkEn->SetState(1);
            // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
        }
        if (section == 2) {
            for (auto r = 0; r < 8 - rows; ++r) {
                // The last section requires padding
                m_pGpioInterface.clkEn->SetState(0);
                m_pGpioInterface.data->SetState(0);
                // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
                m_pGpioInterface.clkEn->SetState(1);
                // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
            }
        }

        m_pGpioInterface.clkSelEn->SetState(1);
        // m_pDelayManager->SynchronousWait_us(delayAfterIO_us);
    }

    return true;
}