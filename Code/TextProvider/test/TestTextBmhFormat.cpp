#include "TestFonts/Bitstream_Vera_Sans_Mono_12.h"
#include "TestFonts/Bitstream_Vera_Sans_Mono_8.h"
#include "TextBmhFormat.h"
//----------
#include "fuzztest/fuzztest.h"
#include "gmock/gmock.h"

using namespace testing;
using namespace fuzztest;

namespace TextBmhFormatTesting {

class FixtureTextBmhFormat : public Test {
   public:
    FixtureTextBmhFormat() : m_textBmhFormat(std::make_unique<TextBmhFormat>()) {}

    std::unique_ptr<TextBmhFormat> m_textBmhFormat;
};

//---------------
// ITextProvider
//---------------

//====================
// GetRequiredSizeCharacter()
//====================

TEST_F(FixtureTextBmhFormat, GetRequiredSizeCharacter_ReturnsZeroZeroIfNoFontSet) {
    TextBmhFormat localTextBmhFormat;

    const auto result = localTextBmhFormat.GetRequiredSizeCharacter('a');
    ASSERT_EQ(result, Vec2D(0, 0));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeCharacter_ReturnsZeroZeroIfCharacterNotAvailableInFontHeight8) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_8::font_data);

    const auto result = m_textBmhFormat->GetRequiredSizeCharacter('c');
    ASSERT_EQ(result, Vec2D(0, 0));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeCharacter_ReturnsZeroZeroIfCharacterNotAvailableInFontHeight12) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    const auto result = m_textBmhFormat->GetRequiredSizeCharacter('c');
    ASSERT_EQ(result, Vec2D(0, 0));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeCharacter_ReturnsCorrectSizeIfCharacterAvailableInFontHeight8) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_8::font_data);

    auto result = m_textBmhFormat->GetRequiredSizeCharacter('A');
    ASSERT_EQ(result, Vec2D(4, 8));

    result = m_textBmhFormat->GetRequiredSizeCharacter('!');
    ASSERT_EQ(result, Vec2D(1, 8));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeCharacter_ReturnsCorrectSizeIfCharacterAvailableInFontHeight12) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    auto result = m_textBmhFormat->GetRequiredSizeCharacter('A');
    ASSERT_EQ(result, Vec2D(7, 12));

    result = m_textBmhFormat->GetRequiredSizeCharacter('!');
    ASSERT_EQ(result, Vec2D(1, 12));
}

//====================
// GetRequiredSizeString()
//====================

TEST_F(FixtureTextBmhFormat, GetRequiredSizeString_ReturnsZeroZeroIfNoFontSet) {
    TextBmhFormat localTextBmhFormat;

    const auto result = localTextBmhFormat.GetRequiredSizeString("ab3!", 1);
    ASSERT_EQ(result, Vec2D(0, 0));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeString_ReturnsZeroZeroIfCharacterInTextNotAvailable) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    // 'C' is not available
    const auto result = m_textBmhFormat->GetRequiredSizeString("abC3!", 1);
    ASSERT_EQ(result, Vec2D(0, 0));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeString_ReturnsCorrectSizeIfSingleCharacterHeight8) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_8::font_data);

    const auto result = m_textBmhFormat->GetRequiredSizeString("A", 1);
    ASSERT_EQ(result, Vec2D(4, 8));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeString_ReturnsCorrectSizeIfSingleCharacterHeight12) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    const auto result = m_textBmhFormat->GetRequiredSizeString("A", 1);
    ASSERT_EQ(result, Vec2D(7, 12));
}

TEST_F(FixtureTextBmhFormat, GetRequiredSizeString_ReturnsCorrectSizeWithStringHeight12) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    const auto result = m_textBmhFormat->GetRequiredSizeString("a2 b!1", 1);

    // a: 6, 2: 7, ' ': 1, b: 6, !: 1, 1: 5
    // 6 + 7 + 1 + 6 + 1 + 5 = 26
    // 5 * 1 spacing = 5
    // 26 + 5 = 31
    ASSERT_EQ(result, Vec2D(31, 12));
}

//====================
// SetString()
//====================

TEST_F(FixtureTextBmhFormat, SetString_ReturnsFalseIfFontNotSet) {
    TextBmhFormat localTextBmhFormat;

    Monochrome pixels[10 * 10];
    auto pixelArea = PixelArea<Monochrome>{Vec2D{10, 10}, pixels};

    const auto result = localTextBmhFormat.SetString("a", 1, Vec2D{0, 0}, pixelArea);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetString_ReturnsTrueIfFunctionFinishedNormally) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    Monochrome pixels[100 * 20];
    auto pixelArea = PixelArea<Monochrome>{Vec2D{100, 20}, pixels};

    const auto result = m_textBmhFormat->SetString("a", 1, Vec2D{0, 0}, pixelArea);
    ASSERT_TRUE(result);
}

TEST_F(FixtureTextBmhFormat, SetString_ReturnsFalseIfCharacterNotAvailableInFont) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    Monochrome pixels[100 * 20];
    auto pixelArea = PixelArea<Monochrome>{Vec2D{100, 20}, pixels};

    // 'C' is not available
    const auto result = m_textBmhFormat->SetString("abC3", 1, Vec2D{0, 0}, pixelArea);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetString_SetsStringInPixelAreaIfCompleteTextFitsInPixelArea) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    const auto pixelAreaSize = Vec2D{30, 20};
    Monochrome pixels[pixelAreaSize.x * pixelAreaSize.y];
    std::memset(pixels, 0, sizeof(pixels));
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, pixels};

    const auto result = m_textBmhFormat->SetString("a1 b!", 1, Vec2D{6, 2}, pixelArea);

    // Required length:
    // a: 6, 1: 5, ' ': 1, b: 6, !: 1
    // 6 + 5 + 1 + 6 + 1 = 19
    // 4 * 1 spacing = 4
    // 19 + 4 = 23

    // clang-format off
    Monochrome expectedPixels[] = {
    //  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 01
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, // 02
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, // 03
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, // 04
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, // 05
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, // 06
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, // 07
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, // 08
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, // 09
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, // 10
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, // 11
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 12
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 13
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 14
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 15
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 17
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 18
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 19
    };
    // clang-format on

    const auto expectedPixelArea = PixelArea<Monochrome>{pixelAreaSize, expectedPixels};

    ASSERT_TRUE(result);
    ASSERT_TRUE(pixelArea.HasSameData(expectedPixelArea));
}

TEST_F(FixtureTextBmhFormat, S_IFMT) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_12::font_data);

    const auto pixelAreaSize = Vec2D{30, 20};
    Monochrome pixels[pixelAreaSize.x * pixelAreaSize.y];
    std::memset(pixels, 0, sizeof(pixels));
    auto pixelArea = PixelArea<Monochrome>{pixelAreaSize, pixels};

    const auto result = m_textBmhFormat->SetString("a1 b!", 1, Vec2D{10, 12}, pixelArea);

    // Required length:
    // a: 6, 1: 5, ' ': 1, b: 6, !: 1
    // 6 + 5 + 1 + 6 + 1 = 19
    // 4 * 1 spacing = 4
    // 19 + 4 = 23

    // clang-format off
    Monochrome expectedPixels[] = {
    //  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 01
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 02
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 03
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 04
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 05
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 06
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 07
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 08
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 09
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 11
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 12
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 13
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 14
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, // 15
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, // 16
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 17
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, // 18
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0  // 19
    };
    // clang-format on

    const auto expectedPixelArea = PixelArea<Monochrome>{pixelAreaSize, expectedPixels};

    ASSERT_TRUE(result);
    ASSERT_TRUE(pixelArea.HasSameData(expectedPixelArea));
}

// TODO: Test with negative offset once Vec2D is templated

//---------------
// TextBmhFormat
//---------------

//====================
// SetFont()
//====================

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsTrueWithValidFont) {
    const auto result = m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_8::font_data);
    ASSERT_TRUE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfFontIsNullptr) {
    const auto result = m_textBmhFormat->SetFont(nullptr);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfStructCharWidthIsNullptr) {
    BmhFont localFont = Bitstream_Vera_Sans_Mono_8::font_data;
    localFont.char_width = nullptr;

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfStructAvailableCharsIsNullptr) {
    BmhFont localFont = Bitstream_Vera_Sans_Mono_8::font_data;
    localFont.available_chars = nullptr;

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfStructCharAddrIsNullptr) {
    BmhFont localFont = Bitstream_Vera_Sans_Mono_8::font_data;
    localFont.char_addr = nullptr;

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfStructCharAddrContainsNullptr) {
    const auto charCount = Bitstream_Vera_Sans_Mono_8::font_data.char_count;
    const uint8_t* localCharAddressArray[charCount];

    const auto nullptrIndex = 2;
    for (uint8_t i = 0; i < charCount; ++i) {
        if (i == nullptrIndex) {
            localCharAddressArray[i] = Bitstream_Vera_Sans_Mono_8::font_data.char_addr[i];
        } else {
            localCharAddressArray[i] = nullptr;
        }
    }

    BmhFont localFont = {.char_width = Bitstream_Vera_Sans_Mono_8::font_data.char_width,
                         .char_addr = &localCharAddressArray[0],
                         .available_chars = Bitstream_Vera_Sans_Mono_8::font_data.available_chars,
                         .char_count = Bitstream_Vera_Sans_Mono_8::font_data.char_count,
                         .font_size = Bitstream_Vera_Sans_Mono_8::font_data.font_size,
                         .top_padding = Bitstream_Vera_Sans_Mono_8::font_data.top_padding};

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfStructCharCountIsZero) {
    BmhFont localFont = {.char_width = Bitstream_Vera_Sans_Mono_8::font_data.char_width,
                         .char_addr = Bitstream_Vera_Sans_Mono_8::font_data.char_addr,
                         .available_chars = Bitstream_Vera_Sans_Mono_8::font_data.available_chars,
                         .char_count = 0,
                         .font_size = Bitstream_Vera_Sans_Mono_8::font_data.font_size,
                         .top_padding = Bitstream_Vera_Sans_Mono_8::font_data.top_padding};

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

TEST_F(FixtureTextBmhFormat, SetFont_ReturnsFalseIfPaddingIsGreaterThanFontSize) {
    BmhFont localFont = {.char_width = Bitstream_Vera_Sans_Mono_8::font_data.char_width,
                         .char_addr = Bitstream_Vera_Sans_Mono_8::font_data.char_addr,
                         .available_chars = Bitstream_Vera_Sans_Mono_8::font_data.available_chars,
                         .char_count = Bitstream_Vera_Sans_Mono_8::font_data.char_count,
                         .font_size = 8,
                         .top_padding = 9};

    const auto result = m_textBmhFormat->SetFont(&localFont);
    ASSERT_FALSE(result);
}

//====================
// SetFont() + GetFont()
//====================

TEST_F(FixtureTextBmhFormat, GetFont_ReturnsNullptrIfNoFontSet) {
    TextBmhFormat localTextBmhFormat;

    const auto result = localTextBmhFormat.GetFont();
    ASSERT_EQ(result, nullptr);
}

TEST_F(FixtureTextBmhFormat, GetFont_ReturnsFontIfFontSet) {
    m_textBmhFormat->SetFont(&Bitstream_Vera_Sans_Mono_8::font_data);

    const auto result = m_textBmhFormat->GetFont();
    ASSERT_EQ(result, &Bitstream_Vera_Sans_Mono_8::font_data);
}

};  // namespace TextBmhFormatTesting