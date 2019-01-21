Pod::Spec.new do |spec|
  spec.name = "spectrum-folly"
  spec.version = "2019.01.21.00"
  spec.license = { :type => "Apache License, Version 2.0" }
  spec.homepage = "https://github.com/facebook/folly"
  spec.summary = "An open-source C++ library developed and used at Facebook."
  spec.authors = "Facebook"
  spec.source = { :git => "https://github.com/facebook/folly.git",
                  :tag => "v#{spec.version}" }
  spec.module_name = "folly"
  spec.prepare_command = <<-CMD
    sed -i -e '/#include <glog/d' folly/detail/RangeCommon.h
    sed -i -e '/DCHECK_/d' folly/detail/RangeCommon.h
  CMD

  spec.header_mappings_dir = "folly"
  spec.source_files = "folly/Optional.h",
                      "folly/Portability.h",
                      "folly/CPortability.h",
                      "folly/Traits.h",
                      "folly/Utility.h",
                      "folly/FixedString.h",
                      "folly/CpuId.h",
                      "folly/Likely.h",
                      "folly/detail/RangeCommon.h",
                      "folly/detail/RangeSse42.h",
                      "folly/lang/Exception.h",
                      "folly/lang/Ordering.h",
                      "folly/portability/Config.h",
                      "folly/portability/String.h",
                      "folly/portability/Constexpr.h",
                      "folly/hash/SpookyHashV2.h",
                      "folly/CppAttributes.h",
                      "folly/ConstexprMath.h",
                      "folly/Range.h"

  spec.header_dir = "folly"
  spec.preserve_paths = "folly/*.h",
                        "folly/portability/*.h",
                        "folly/lang/*.h",
                        "folly/detail/*.h",
                        "folly/hash/*.h"

  spec.libraries = "stdc++"
  spec.platforms = { :ios => "8.0" }
end
