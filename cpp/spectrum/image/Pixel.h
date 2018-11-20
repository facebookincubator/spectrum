// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/FixedString.h>
#include <folly/Optional.h>

#include <cstddef>
#include <cstdint>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {

/**
 * A color model defines the image's components and how they are interpreted to
 * recreate the intended color.
 */
struct ColorModel final {
  using Identifier = folly::FixedString<10>;

  /**
   * Human readable identifier of the color model (e.g. `RGB`).
   */
  Identifier identifier;

  /**
   * Number of independent color components in the color model (e.g. 3 for for
   * `RGB`).
   */
  std::uint8_t numberOfColorComponents;

  /**
   * True if the color model supports an alpha channel. For example, `RGB`
   * supports an alpha channel whereas `CMYK` does not.
   */
  bool supportsExtraAlphaChannel;

  constexpr explicit ColorModel(
      const Identifier& identifier,
      const std::uint8_t numberOfColorComponents,
      const bool supportsExtraAlphaChannel)
      : identifier(identifier),
        numberOfColorComponents(numberOfColorComponents),
        supportsExtraAlphaChannel(supportsExtraAlphaChannel) {}

  bool operator==(const ColorModel& rhs) const;
  bool operator<(const ColorModel& rhs) const;
};

namespace colormodels {

/** Color model consisting of a gray color channel */
static constexpr auto Gray = ColorModel{"gray", 1, true};

/**
 * Color model consisting of a red, green and blue color channel which work
 * additively.
 */
static constexpr auto RGB = ColorModel{"rgb", 3, true};

/**
 * Color model consisting of a cyan, magenta, yellow and key color channel
 * which work subtractively.
 */
static constexpr auto CMYK = ColorModel{"cmyk", 4, false};
static constexpr auto YCbCr = ColorModel{"ycbcr", 3, false};
static constexpr auto YCbCrK = ColorModel{"ycbcrk", 4, false};
static constexpr auto Unknown = ColorModel{"unknown", 0, false};
} // namespace colormodels

/**
 * The alpha info specifies the behaviour of the optional alpha channel.
 */
enum class AlphaInfo : std::uint8_t {
  /** No alpha channel */
  None = 0,

  /** Alpha channel comes before any other component */
  First,

  /** Alpha channel comes after any other component */
  Last,

  /** Alpha channel comes before any other component and must be ignored */
  SkipFirst,

  /** Alpha channel comes after any other component and must be ignored */
  SkipLast,

  /**
   * Alpha channel comes before any other component and the other components
   *   are premultiplied with its value
   */
  PremultipliedFirst,

  /**
   * Alpha channel comes after any other component and the other compoenents are
   * premultiplied with its value
   */
  PremultipliedLast,
};

std::string alphaInfoValueToString(const AlphaInfo alphaInfo);

AlphaInfo alphaInfoFromValue(const int value);

bool alphaInfoIsPremultiplied(const AlphaInfo alphaInfo);
bool alphaInfoIsFirst(const AlphaInfo alphaInfo);
bool alphaInfoIsLast(const AlphaInfo alphaInfo);
bool alphaInfoShouldInterpretAlphaValue(const AlphaInfo alphaInfo);

/** Order of the components in memory representation */
enum class ComponentsOrder : std::uint8_t { Natural = 0, Reversed = 1 };

ComponentsOrder makeComponentsOrderFromValue(const int value);

std::string componentsOrderValueToString(const ComponentsOrder componentsOrder);

/**
 * The specification explains how pixels and the color information is stored as
 * a continuous stream of bytes.
 */
class Specification {
 public:
  /** Number of color components and how to interpret them */
  ColorModel colorModel;

  /** The number of bytes per pixel (covering all its components). */
  std::uint8_t bytesPerPixel;

  /** Information about the optional alpha channel */
  AlphaInfo alphaInfo;

  /**
   * If true, the components are ordered reverse to the default of the color
   * model. E.g. BGR instead of RGB.
   */
  ComponentsOrder componentsOrder;

  constexpr Specification(
      const ColorModel& colorModel,
      const std::uint8_t bytesPerPixel,
      AlphaInfo alphaInfo,
      ComponentsOrder componentsOrder)
      : colorModel(colorModel),
        bytesPerPixel(bytesPerPixel),
        alphaInfo(alphaInfo),
        componentsOrder(componentsOrder) {}

  /**
   * The total number of components including an alpha channel (if any).
   */
  std::uint8_t numberOfComponents() const noexcept;

  bool hasAlpha() const noexcept;

  /**
   * Returns true if there is an alpha component and the alpha component is the
   * first byte of a pixel.
   */
  bool isAlphaLeadingComponent() const noexcept;

  std::string string() const;

  bool operator==(const Specification& rhs) const;
  bool operator!=(const Specification& rhs) const;
  bool operator<(const Specification& rhs) const noexcept;
};

namespace specifications {
static constexpr Specification Gray{colormodels::Gray,
                                    1,
                                    AlphaInfo::None,
                                    ComponentsOrder::Natural};
static constexpr Specification GrayA{colormodels::Gray,
                                     2,
                                     AlphaInfo::Last,
                                     ComponentsOrder::Natural};
static constexpr Specification AGray{colormodels::Gray,
                                     2,
                                     AlphaInfo::First,
                                     ComponentsOrder::Natural};
static constexpr Specification RGB{colormodels::RGB,
                                   3,
                                   AlphaInfo::None,
                                   ComponentsOrder::Natural};
static constexpr Specification RGBA{colormodels::RGB,
                                    4,
                                    AlphaInfo::Last,
                                    ComponentsOrder::Natural};
static constexpr Specification RGBX{colormodels::RGB,
                                    4,
                                    AlphaInfo::SkipLast,
                                    ComponentsOrder::Natural};
static constexpr Specification ARGB{colormodels::RGB,
                                    4,
                                    AlphaInfo::First,
                                    ComponentsOrder::Natural};
static constexpr Specification XRGB{colormodels::RGB,
                                    4,
                                    AlphaInfo::SkipFirst,
                                    ComponentsOrder::Natural};

static constexpr Specification BGR{colormodels::RGB,
                                   3,
                                   AlphaInfo::None,
                                   ComponentsOrder::Reversed};
static constexpr Specification BGRA{colormodels::RGB,
                                    4,
                                    AlphaInfo::First,
                                    ComponentsOrder::Reversed};
static constexpr Specification BGRX{colormodels::RGB,
                                    4,
                                    AlphaInfo::SkipFirst,
                                    ComponentsOrder::Reversed};

static constexpr Specification ABGR{colormodels::RGB,
                                    4,
                                    AlphaInfo::Last,
                                    ComponentsOrder::Reversed};
static constexpr Specification XBGR{colormodels::RGB,
                                    4,
                                    AlphaInfo::SkipLast,
                                    ComponentsOrder::Reversed};
static constexpr Specification yCbCr{colormodels::YCbCr,
                                     3,
                                     AlphaInfo::None,
                                     ComponentsOrder::Natural};
} // namespace specifications

} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
