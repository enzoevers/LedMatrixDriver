#include "HanoverOL037A.h"
//-----
#include "MockIDelay.h"
#include "MockIGPIOOutput.h"
//-----
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace HanoverOL037ATesting {

class FixtureHanoverOL037A : public Test {
   public:
    FixtureHanoverOL037A()
        : hanoverOL037A(std::make_unique<HanoverOL037A>()),
          clk(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          clkEn(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          clkSelEn(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          data(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          latch(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          ledOE(std::make_unique<NiceMock<MockIGPIOOutput>>()),
          gpioInterface(CreateGPIOInterfaceWithMocks()),
          mockDelay(std::make_unique<NiceMock<MockIDelay>>()) {
        hanoverOL037A->SetDelayManager(mockDelay.get());
    }

    ~FixtureHanoverOL037A() {}

    auto CreateGPIOInterfaceWithMocks() -> HanoverOL037A_GPIOInterface {
        return HanoverOL037A_GPIOInterface{clk.get(),  clkEn.get(), clkSelEn.get(),
                                           data.get(), latch.get(), ledOE.get()};
    }

    std::unique_ptr<HanoverOL037A> hanoverOL037A;

    std::unique_ptr<NiceMock<MockIGPIOOutput>> clk;
    std::unique_ptr<NiceMock<MockIGPIOOutput>> clkEn;
    std::unique_ptr<NiceMock<MockIGPIOOutput>> clkSelEn;
    std::unique_ptr<NiceMock<MockIGPIOOutput>> data;
    std::unique_ptr<NiceMock<MockIGPIOOutput>> latch;
    std::unique_ptr<NiceMock<MockIGPIOOutput>> ledOE;
    HanoverOL037A_GPIOInterface gpioInterface;

    std::unique_ptr<NiceMock<MockIDelay>> mockDelay;
};

//---------------
// IHanoverOL037A
//---------------

//====================
// SetGPIOInterface()
//====================

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_clk_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.clk = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_clkEn_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.clkEn = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_clkSelEn_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.clkSelEn = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_data_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.data = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_latch_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.latch = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsFalseIf_ledOE_IsNullptr) {
    auto localGpioInterface = gpioInterface;
    localGpioInterface.ledOE = nullptr;
    auto success = hanoverOL037A->SetGPIOInterface(localGpioInterface);
    ASSERT_FALSE(success);
}

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_ReturnsTrueWhenNoNullptrInInterface) {
    auto success = hanoverOL037A->SetGPIOInterface(gpioInterface);
    ASSERT_TRUE(success);
}

//====================
// SetGPIOInterface() + GetGPIOInterface()
//====================

TEST_F(FixtureHanoverOL037A, SetGPIOInterface_SetsInterfaceAndGetGPIOInterfaceReturnsThis) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);
    ASSERT_EQ(hanoverOL037A->GetGPIOInterface(), gpioInterface);
}

//====================
// GetDelayManager()
//====================

TEST_F(FixtureHanoverOL037A, GetDelayManager_ReturnsNullptrByDefault) {
    HanoverOL037A localDisplay;
    ASSERT_EQ(localDisplay.GetDelayManager(), nullptr);
}

//====================
// SetDelayManager() + GetDelayManager()
//====================

TEST_F(FixtureHanoverOL037A, SetDelayManager_SetsAndGetsCorrectPointer) {
    MockIDelay delay;
    hanoverOL037A->SetDelayManager(&delay);
    ASSERT_EQ(hanoverOL037A->GetDelayManager(), &delay);
}

TEST_F(FixtureHanoverOL037A, SetDelayManager_CanSetNullptr) {
    MockIDelay delay;
    hanoverOL037A->SetDelayManager(&delay);
    ASSERT_EQ(hanoverOL037A->GetDelayManager(), &delay);

    hanoverOL037A->SetDelayManager(nullptr);
    ASSERT_EQ(hanoverOL037A->GetDelayManager(), nullptr);
}

//---------------
// IDisplay
//---------------

//====================
// GetResolution()
//====================

TEST_F(FixtureHanoverOL037A, GetResolution_ReturnsCorrectResolution) {
    const auto resolution = hanoverOL037A->GetResolution();
    const auto expectedResolution = Vec2D{160, 19};

    ASSERT_EQ(resolution, expectedResolution);
}

//====================
// GetCurrentDisplay()
//====================

TEST_F(FixtureHanoverOL037A, GetCurrentDisplay_ReturnsAreaOfCorrectResolution) {
    const auto resolution = hanoverOL037A->GetResolution();
    Monochrome dataArray[resolution.x * resolution.y];

    PixelArea<Monochrome> pixelArea{resolution, &dataArray[0]};
    hanoverOL037A->GetCurrentDisplay(pixelArea);

    const auto expectedSize = hanoverOL037A->GetResolution();

    ASSERT_EQ(pixelArea.size, expectedSize);
}

//====================
// GetCurrentPixelArea()
//====================

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfTopLeftCoordinateOutOfRangeInX) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{1, 1};
    Monochrome areaData[size.x * size.y];
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x, resolution.y - 1};

    ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfTopLeftCoordinateOutOfRangeInY) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{1, 1};
    Monochrome areaData[size.x * size.y];
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - 1, resolution.y};

    ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfBottomRightCoordinateOutOfRangeInX) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - (size.x - 1), resolution.y - 1 - (size.y - 1)};

    ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfBottomRightCoordinateOutOfRangeInY) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - 1 - (size.x - 1), resolution.y - (size.y - 1)};

    ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfPixelAreaHasSizeZero) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto topLeftCoordinate = Vec2D{10, 10};

    {
        const auto size = Vec2D{0, 3};
        Monochrome areaData[1];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }

    {
        const auto size = Vec2D{2, 0};
        Monochrome areaData[1];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }

    {
        const auto size = Vec2D{0, 0};
        Monochrome areaData[1];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsFalseIfDataIsNullptr) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    auto pixelArea = PixelArea<Monochrome>{size, nullptr};

    const auto topLeftCoordinate = Vec2D{10, 10};

    ASSERT_FALSE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, GetCurrentPixelArea_ReturnsAreaOfRequestedSize) {
    const auto resolution = hanoverOL037A->GetResolution();

    {
        const auto size = Vec2D{2, 3};
        Monochrome areaData[size.x * size.y];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        const auto topLeftCoordinate = Vec2D{resolution.x - 1 - (size.x - 1), resolution.y - 1 - (size.y - 1)};

        ASSERT_TRUE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }

    {
        const auto size = Vec2D{2, 3};
        Monochrome areaData[size.x * size.y];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        const auto topLeftCoordinate = Vec2D{0, 0};

        ASSERT_TRUE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }

    {
        const auto size = Vec2D{2, 3};
        Monochrome areaData[size.x * size.y];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        const auto topLeftCoordinate = Vec2D{0, resolution.y - 1 - (size.y - 1)};

        ASSERT_TRUE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }

    {
        const auto size = Vec2D{2, 3};
        Monochrome areaData[size.x * size.y];
        auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

        const auto topLeftCoordinate = Vec2D{resolution.x - 1 - (size.x - 1), 0};

        ASSERT_TRUE(hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelArea));
    }
}

//====================
// SetPixel()
//====================

TEST_F(FixtureHanoverOL037A, SetPixel_ReturnsFalseIfCoordinateOutOfRangeInX) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto coordinate = Vec2D{resolution.x, resolution.y - 1};
    ASSERT_FALSE(hanoverOL037A->SetPixel(coordinate, 16));
}

TEST_F(FixtureHanoverOL037A, SetPixel_ReturnsFalseIfCoordinateOutOfRangeInY) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto coordinate = Vec2D{resolution.x - 1, resolution.y};
    ASSERT_FALSE(hanoverOL037A->SetPixel(coordinate, 16));
}

TEST_F(FixtureHanoverOL037A, SetPixel_ReturnsTrueIfCoordinateInRange) {
    const auto resolution = hanoverOL037A->GetResolution();

    auto coordinate = Vec2D{resolution.x - 1, resolution.y - 1};
    ASSERT_TRUE(hanoverOL037A->SetPixel(coordinate, 16));

    coordinate = Vec2D{0, 0};
    ASSERT_TRUE(hanoverOL037A->SetPixel(coordinate, 16));

    coordinate = Vec2D{0, resolution.y - 1};
    ASSERT_TRUE(hanoverOL037A->SetPixel(coordinate, 16));

    coordinate = Vec2D{resolution.x - 1, 0};
    ASSERT_TRUE(hanoverOL037A->SetPixel(coordinate, 16));
}

//====================
// SetArea()
//====================

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfTopLeftCoordinateOutOfRangeInX) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    const auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x, resolution.y - 1};

    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfTopLeftCoordinateOutOfRangeInY) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    const auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - 1, resolution.y};

    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfBottomRightCoordinateOutOfRangeInX) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    const auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - (size.x - 1), resolution.y - 1 - (size.y - 1)};

    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfBottomRightCoordinateOutOfRangeInY) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    Monochrome areaData[size.x * size.y];
    const auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    const auto topLeftCoordinate = Vec2D{resolution.x - 1 - (size.x - 1), resolution.y - (size.y - 1)};

    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfAreaHasZeroSize) {
    // Note that topLeftCoordinate has larger values than the max in the `size` variable
    const auto topLeftCoordinate = Vec2D{10, 10};

    auto size = Vec2D{0, 3};
    Monochrome areaData[] = {1};
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};
    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    size = Vec2D{2, 0};
    pixelArea.size = size;
    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    size = Vec2D{0, 0};
    pixelArea.size = size;
    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsFalseIfDataIsNullptr) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto size = Vec2D{2, 3};
    const auto pixelArea = PixelArea<Monochrome>{size, nullptr};

    const auto topLeftCoordinate = Vec2D{10, 10};

    ASSERT_FALSE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

TEST_F(FixtureHanoverOL037A, SetArea_ReturnsTrueWithValidData) {
    const auto resolution = hanoverOL037A->GetResolution();

    auto size = Vec2D{1, 1};
    Monochrome areaData[size.x * size.y];
    auto pixelArea = PixelArea<Monochrome>{size, &areaData[0]};

    auto topLeftCoordinate = Vec2D{resolution.x - 1, resolution.y - 1};
    ASSERT_TRUE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    topLeftCoordinate = Vec2D{0, 0};
    ASSERT_TRUE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    topLeftCoordinate = Vec2D{0, resolution.y - 1};
    ASSERT_TRUE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    topLeftCoordinate = Vec2D{resolution.x - 1, 0};
    ASSERT_TRUE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));

    size = Vec2D{2, 3};
    Monochrome areaData2x3[size.x * size.y];
    pixelArea = PixelArea<Monochrome>{size, &areaData2x3[0]};
    topLeftCoordinate = Vec2D{resolution.x - 1 - (size.x - 1), resolution.y - 1 - (size.y - 1)};
    ASSERT_TRUE(hanoverOL037A->SetArea(topLeftCoordinate, pixelArea));
}

//====================
// SetPixel() + SetArea() + GetCurrentDisplay() + GetCurrentPixelArea()
//====================

TEST_F(FixtureHanoverOL037A, SetPixel_GetCurrentDisplay_SetsAndGetsCorrectly) {
    const auto resolution = hanoverOL037A->GetResolution();
    Monochrome pixelMemory[resolution.x * resolution.y];

    const Monochrome colorValue1 = 16;
    const auto coordinate1 = Vec2D{0, 0};
    {
        hanoverOL037A->SetPixel(coordinate1, colorValue1);

        auto pixelArea = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelArea);

        ASSERT_EQ(pixelArea.size, resolution);
        ASSERT_EQ(pixelArea.pixels, &pixelMemory[0]);

        ASSERT_EQ(pixelArea.at(coordinate1), colorValue1);
    }

    const Monochrome colorValue2 = 30;
    const auto coordinate2 = Vec2D{resolution.x - 1, resolution.y - 1};
    {
        hanoverOL037A->SetPixel(coordinate2, colorValue2);

        auto pixelArea = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelArea);

        ASSERT_EQ(pixelArea.at(coordinate1), colorValue1);
        ASSERT_EQ(pixelArea.at(coordinate2), colorValue2);
    }

    const Monochrome colorValue3 = std::numeric_limits<Monochrome>::max();
    const auto coordinate3 = Vec2D{resolution.x / 2, resolution.y / 2};
    {
        hanoverOL037A->SetPixel(coordinate3, colorValue3);

        auto pixelArea = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelArea);

        ASSERT_EQ(pixelArea.at(coordinate1), colorValue1);
        ASSERT_EQ(pixelArea.at(coordinate2), colorValue2);
        ASSERT_EQ(pixelArea.at(coordinate3), colorValue3);
    }

    const Monochrome colorValue2_1 = colorValue2 + 1;
    {
        hanoverOL037A->SetPixel(coordinate2, colorValue2_1);

        auto pixelArea = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelArea);

        ASSERT_EQ(pixelArea.at(coordinate1), colorValue1);
        ASSERT_EQ(pixelArea.at(coordinate2), colorValue2_1);
        ASSERT_EQ(pixelArea.at(coordinate3), colorValue3);
    }
}

TEST_F(FixtureHanoverOL037A, SetPixel_GetCurrentPixelArea_1x1_SetsAndGetsCorrectly) {
    const auto resolution = hanoverOL037A->GetResolution();

    const Monochrome colorValue1 = 16;
    const auto coordinate1 = Vec2D{0, 0};
    {
        hanoverOL037A->SetPixel(coordinate1, colorValue1);

        Monochrome pixel;
        const auto areaSize = Vec2D{1, 1};
        auto pixelArea = PixelArea<Monochrome>{areaSize, &pixel};
        hanoverOL037A->GetCurrentPixelArea(coordinate1, pixelArea);

        ASSERT_EQ(pixelArea.size, areaSize);
        ASSERT_EQ(pixelArea.pixels, &pixel);

        ASSERT_EQ(pixel, colorValue1);
    }

    const Monochrome colorValue2 = 30;
    const auto coordinate2 = Vec2D{resolution.x - 1, resolution.y - 1};
    {
        hanoverOL037A->SetPixel(coordinate2, colorValue2);

        Monochrome pixel;
        auto pixelArea = PixelArea<Monochrome>{{1, 1}, &pixel};
        hanoverOL037A->GetCurrentPixelArea(coordinate2, pixelArea);

        ASSERT_EQ(pixel, colorValue2);
    }

    const Monochrome colorValue3 = std::numeric_limits<Monochrome>::max();
    const auto coordinate3 = Vec2D{resolution.x / 2, resolution.y / 2};
    {
        hanoverOL037A->SetPixel(coordinate3, colorValue3);

        Monochrome pixel;
        auto pixelArea = PixelArea<Monochrome>{{1, 1}, &pixel};
        hanoverOL037A->GetCurrentPixelArea(coordinate3, pixelArea);

        ASSERT_EQ(pixel, colorValue3);
    }

    const Monochrome colorValue2_1 = colorValue2 + 1;
    {
        hanoverOL037A->SetPixel(coordinate2, colorValue2_1);

        Monochrome pixel;
        auto pixelArea = PixelArea<Monochrome>{{1, 1}, &pixel};
        hanoverOL037A->GetCurrentPixelArea(coordinate2, pixelArea);

        ASSERT_EQ(pixel, colorValue2_1);
    }
}

TEST_F(FixtureHanoverOL037A, SetArea_GetCurrentDisplay_SetsAndGetsCorrectly) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto pixelAreaSize = Vec2D{3, 2};
    Monochrome pixelDataToSet[] = {1, 3, 5, 20, 22, 24};
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, &pixelDataToSet[0]};

    {
        const auto topLeftCoordinate =
            Vec2D{resolution.x - 1 - (pixelAreaSize.x - 1), resolution.y - 1 - (pixelAreaSize.y - 1)};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemory[resolution.x * resolution.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto indexInSourceArea = y * pixelAreaSize.x + x;
                const auto indexInDisplay = ((y + topLeftCoordinate.y) * resolution.x) + topLeftCoordinate.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[indexInDisplay], pixelDataToSet[indexInSourceArea]);
            }
        }
    }

    {
        const auto topLeftCoordinate = Vec2D{resolution.x / 2, resolution.y / 2};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemory[resolution.x * resolution.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto indexInSourceArea = y * pixelAreaSize.x + x;
                const auto indexInDisplay = ((y + topLeftCoordinate.y) * resolution.x) + topLeftCoordinate.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[indexInDisplay], pixelDataToSet[indexInSourceArea]);
            }
        }
    }

    {
        const auto topLeftCoordinate = Vec2D{0, 0};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemory[resolution.x * resolution.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{resolution, &pixelMemory[0]};
        hanoverOL037A->GetCurrentDisplay(pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto indexInSourceArea = y * pixelAreaSize.x + x;
                const auto indexInDisplay = y * resolution.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[indexInDisplay], pixelDataToSet[indexInSourceArea]);
            }
        }
    }
}

TEST_F(FixtureHanoverOL037A, SetArea_GetCurrentPixelArea_3x2_SetsAndGetsCorrectly) {
    const auto resolution = hanoverOL037A->GetResolution();

    const auto pixelAreaSize = Vec2D{3, 2};
    Monochrome pixelDataToSet[] = {1, 3, 5, 20, 22, 24};
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, &pixelDataToSet[0]};

    {
        const auto topLeftCoordinate =
            Vec2D{resolution.x - 1 - (pixelAreaSize.x - 1), resolution.y - 1 - (pixelAreaSize.y - 1)};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemoryToGet[pixelAreaSize.x * pixelAreaSize.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{pixelAreaSize, &pixelMemoryToGet[0]};
        hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto index = y * pixelAreaSize.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[index], pixelDataToSet[index]);
            }
        }
    }

    {
        const auto topLeftCoordinate = Vec2D{resolution.x / 2, resolution.y / 2};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemoryToGet[pixelAreaSize.x * pixelAreaSize.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{pixelAreaSize, &pixelMemoryToGet[0]};
        hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto index = y * pixelAreaSize.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[index], pixelDataToSet[index]);
            }
        }
    }

    {
        const auto topLeftCoordinate = Vec2D{0, 0};

        hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

        Monochrome pixelMemoryToGet[pixelAreaSize.x * pixelAreaSize.y];
        auto pixelAreaReceived = PixelArea<Monochrome>{pixelAreaSize, &pixelMemoryToGet[0]};
        hanoverOL037A->GetCurrentPixelArea(topLeftCoordinate, pixelAreaReceived);

        for (auto y = 0; y < pixelAreaSize.y; y++) {
            for (auto x = 0; x < pixelAreaSize.x; x++) {
                const auto index = y * pixelAreaSize.x + x;
                ASSERT_EQ(pixelAreaReceived.pixels[index], pixelDataToSet[index]);
            }
        }
    }
}

//====================
// UpdateDisplay()
//====================

TEST_F(FixtureHanoverOL037A, UpdateDisplay_ReturnsTrueIfAllValid) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    ASSERT_TRUE(hanoverOL037A->UpdateDisplay());
}

TEST_F(FixtureHanoverOL037A, UpdateDisplay_PerformsCorrectIOSequence) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    {
        InSequence seq;

        EXPECT_CALL(*latch, SetState(0)).WillOnce(Return(true));

        const auto expectedSectionSelections =
            hanoverOL037A->GetNumberOfPanels() * hanoverOL037A->GetNumberOfSectionsPerPanel();
        for (auto i = 0; i < expectedSectionSelections; ++i) {
            EXPECT_CALL(*clk, SetState(1)).WillOnce(Return(true));
            EXPECT_CALL(*clk, SetState(0)).WillOnce(Return(true));
        }

        EXPECT_CALL(*latch, SetState(1)).WillOnce(Return(true));
        EXPECT_CALL(*latch, SetState(0)).WillOnce(Return(true));

        EXPECT_CALL(*ledOE, SetState(0)).WillOnce(Return(true));
    }

    ASSERT_TRUE(hanoverOL037A->UpdateDisplay());
}

//====================
// GetNumberOfPanels()
//====================

TEST_F(FixtureHanoverOL037A, GetNumberOfPanels_ReturnsCorrectValue) {
    ASSERT_EQ(hanoverOL037A->GetNumberOfPanels(), 4);
}

//====================
// GetNumberOfSectionsPerPanel()
//====================

TEST_F(FixtureHanoverOL037A, GetNumberOfSectionsPerPanel_ReturnsCorrectValue) {
    ASSERT_EQ(hanoverOL037A->GetNumberOfSectionsPerPanel(), 3);
}

//====================
// GetPanelAndSectionSelectionByte()
//====================

TEST_F(FixtureHanoverOL037A, GetPanelAndSectionSelectionByte_ReturnsZerosIfPanelOutsideOfRange) {
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(hanoverOL037A->GetNumberOfPanels(), 0), 0);
}

TEST_F(FixtureHanoverOL037A, GetPanelAndSectionSelectionByte_ReturnsZerosIfSectionOutsideOfRange) {
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(0, hanoverOL037A->GetNumberOfSectionsPerPanel()), 0);
}

TEST_F(FixtureHanoverOL037A, GetPanelAndSectionSelectionByte_ReturnsCorrectByte) {
    // Panel
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(0, 0), 0b01111100);
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(1, 0), 0b10111100);
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(2, 0), 0b11011100);
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(3, 0), 0b11101100);

    // Section
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(1, 1), 0b10111101);
    ASSERT_EQ(hanoverOL037A->GetPanelAndSectionSelectionByte(1, 2), 0b10111110);
}

//====================
// SelectPanelAndSection()
//====================

TEST_F(FixtureHanoverOL037A, SelectPanelAndSection_ReturnsFalseIfPanelOutsideOfRange) {
    ASSERT_FALSE(hanoverOL037A->SelectPanelAndSection(hanoverOL037A->GetNumberOfPanels(), 0));
}

TEST_F(FixtureHanoverOL037A, SelectPanelAndSection_ReturnsFalseIfSectionOutsideOfRange) {
    ASSERT_FALSE(hanoverOL037A->SelectPanelAndSection(0, hanoverOL037A->GetNumberOfSectionsPerPanel()));
}

TEST_F(FixtureHanoverOL037A, SelectPanelAndSection_ReturnsFalseIfGpioInterfaceNotSet) {
    ASSERT_FALSE(hanoverOL037A->SelectPanelAndSection(0, 0));
}

TEST_F(FixtureHanoverOL037A, SelectPanelAndSection_ReturnsTrueIfValidRanges) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    ASSERT_TRUE(hanoverOL037A->SelectPanelAndSection(0, 0));

    ASSERT_TRUE(hanoverOL037A->SelectPanelAndSection(hanoverOL037A->GetNumberOfPanels() - 1,
                                                     hanoverOL037A->GetNumberOfSectionsPerPanel() - 1));

    ASSERT_TRUE(hanoverOL037A->SelectPanelAndSection(hanoverOL037A->GetNumberOfPanels() / 2,
                                                     hanoverOL037A->GetNumberOfSectionsPerPanel() / 2));
}

TEST_F(FixtureHanoverOL037A, SelectPanelAndSection_PerformsCorrectIOSequence) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    InSequence seq;

    EXPECT_CALL(*clk, SetState(1)).WillOnce(Return(true));

    uint8_t expectedByte = 0b11011101;
    // Writes 0b11011101 from bit 0 till 7

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
    EXPECT_CALL(*data, SetState(1)).WillOnce(Return(true));
    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

    hanoverOL037A->SelectPanelAndSection(2, 1);
}

//====================
// WritePixels()
//====================

TEST_F(FixtureHanoverOL037A, WritePixels_ReturnsFalseIfPanelOutsideOfRange) {
    ASSERT_FALSE(hanoverOL037A->WritePixels(hanoverOL037A->GetNumberOfPanels(), 0));
}

TEST_F(FixtureHanoverOL037A, WritePixels_ReturnsFalseIfSectionOutsideOfRange) {
    ASSERT_FALSE(hanoverOL037A->WritePixels(0, hanoverOL037A->GetNumberOfSectionsPerPanel()));
}

TEST_F(FixtureHanoverOL037A, WritePixels_ReturnsFalseIfGpioInterfaceNotSet) {
    ASSERT_FALSE(hanoverOL037A->WritePixels(0, 0));
}

TEST_F(FixtureHanoverOL037A, WritePixels_ReturnsTrueIfValidRanges) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    ASSERT_TRUE(hanoverOL037A->WritePixels(0, 0));

    ASSERT_TRUE(hanoverOL037A->WritePixels(hanoverOL037A->GetNumberOfPanels() - 1,
                                           hanoverOL037A->GetNumberOfSectionsPerPanel() - 1));

    ASSERT_TRUE(hanoverOL037A->WritePixels(hanoverOL037A->GetNumberOfPanels() / 2,
                                           hanoverOL037A->GetNumberOfSectionsPerPanel() / 2));
}

TEST_F(FixtureHanoverOL037A, WritePixels_PerformsCorrectIOSequence_ForSectionZeroAndOne) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    const auto topLeftCoordinate = Vec2D{10, 3};
    const auto pixelAreaSize = Vec2D{1, 7};
    std::array<Monochrome, 1 * 7> pixelData = {1, 0, 97, 20, 0, 24, 45};
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, &pixelData[0]};
    hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

    for (auto s = 0; s < 2; ++s) {
        InSequence seq;

        EXPECT_CALL(*clk, SetState(0)).WillOnce(Return(true));

        constexpr auto columns = 40;
        constexpr auto rows = 8;

        for (auto c = 0; c < columns; ++c) {
            EXPECT_CALL(*clkSelEn, SetState(0)).WillOnce(Return(true));

            if (c == topLeftCoordinate.x) {
                if (s == 0) {
                    for (auto i = 0; i < 3; ++i) {
                        EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                        EXPECT_CALL(*data, SetState(false)).WillOnce(Return(true));
                        EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
                    }

                    // Expected data
                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(false)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(false)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
                } else {
                    // Expected data
                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

                    for (auto i = 0; i < 6; ++i) {
                        EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                        EXPECT_CALL(*data, SetState(false)).WillOnce(Return(true));
                        EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
                    }
                }
            } else {
                for (auto i = 0; i < rows; ++i) {
                    EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*data, SetState(0)).WillOnce(Return(true));
                    EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
                }
            }

            EXPECT_CALL(*clkSelEn, SetState(1)).WillOnce(Return(true));
        }

        hanoverOL037A->WritePixels(0, s);
    }
}

TEST_F(FixtureHanoverOL037A, WritePixels_PerformsCorrectIOSequence_ForSectionTwo) {
    hanoverOL037A->SetGPIOInterface(gpioInterface);

    const auto topLeftCoordinate = Vec2D{10, 16};
    const auto pixelAreaSize = Vec2D{1, 3};
    std::array<Monochrome, 1 * 3> pixelData = {23, 0, 45};
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, &pixelData[0]};
    hanoverOL037A->SetArea(topLeftCoordinate, pixelArea);

    InSequence seq;

    EXPECT_CALL(*clk, SetState(0)).WillOnce(Return(true));

    constexpr auto columns = 40;
    constexpr auto rows = 3;

    for (auto c = 0; c < columns; ++c) {
        EXPECT_CALL(*clkSelEn, SetState(0)).WillOnce(Return(true));

        if (c == topLeftCoordinate.x) {
            EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
            EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
            EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

            EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
            EXPECT_CALL(*data, SetState(false)).WillOnce(Return(true));
            EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

            EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
            EXPECT_CALL(*data, SetState(true)).WillOnce(Return(true));
            EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));

        } else {
            for (auto i = 0; i < rows; ++i) {
                EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
                EXPECT_CALL(*data, SetState(0)).WillOnce(Return(true));
                EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
            }
        }

        for (auto i = 0; i < 5; ++i) {
            EXPECT_CALL(*clkEn, SetState(0)).WillOnce(Return(true));
            EXPECT_CALL(*data, SetState(0)).WillOnce(Return(true));
            EXPECT_CALL(*clkEn, SetState(1)).WillOnce(Return(true));
        }

        EXPECT_CALL(*clkSelEn, SetState(1)).WillOnce(Return(true));
    }

    hanoverOL037A->WritePixels(0, 2);
}

// TODO: Add tests for WritePixels(...) where the panel is also non-zero

}  // namespace HanoverOL037ATesting