# Copyright (c) Facebook, Inc. and its affiliates.

compiler_flags = '-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1'

version = '0.9.0'
pinned_version = '~>' + version

Pod::Spec.new do |spec|
  spec.name = 'SpectrumKit'
  spec.version = version
  spec.summary = 'A client-side image transcoding library.'
  spec.description = <<-DESC
Spectrum is a cross-platform image transcoding library that can easily be integrated into an  iOS project to efficiently perform common image operations.
                       DESC

  spec.homepage = 'http://libspectrum.io'
  spec.license = { :type => 'MIT', :file => 'LICENSE' }
  spec.authors = 'Facebook'
  spec.source = { :git => 'https://github.com/facebookincubator/spectrum.git', :tag => "v#{version}" }
  spec.ios.deployment_target = '8.0'
  spec.default_subspecs = 'Plugins/Default'
  spec.dependency 'spectrum-folly', "~>2018.11.12.00"
  spec.compiler_flags = compiler_flags

  spec.subspec 'Base' do |base_spec|
    base_spec.header_dir = 'SpectrumKit'
    base_spec.source_files = 'ios/SpectrumKit/SpectrumKit/**/*.{h,m,mm}'
    base_spec.dependency 'SpectrumCore/Base'
    base_spec.exclude_files = 'ios/SpectrumKit/SpectrumKit/Internal'
    base_spec.public_header_files = 'ios/SpectrumKit/SpectrumKit/Configuration/**/*.h',
                                    'ios/SpectrumKit/SpectrumKit/*.h',
                                    'ios/SpectrumKit/SpectrumKit/Requirements/*.h',
                                    'ios/SpectrumKit/SpectrumKit/Misc/*.h',
                                    'ios/SpectrumKit/SpectrumKit/Image/*.h'
  end

  spec.subspec 'Plugins' do |plugins_spec|
    plugins_spec.subspec 'Default' do |default_spec|
      default_spec.dependency 'SpectrumKit/Plugins/Jpeg'
      default_spec.dependency 'SpectrumKit/Plugins/Png'
      default_spec.dependency 'SpectrumKit/Plugins/Webp'
      default_spec.source_files = 'ios/SpectrumKit/SpectrumKitPlugins/Default/**/*'
    end

    plugins_spec.subspec 'Jpeg' do |plugins_jpeg_spec|
      plugins_jpeg_spec.dependency 'SpectrumKit/Base', pinned_version
      plugins_jpeg_spec.dependency 'SpectrumCore/Plugins/Jpeg'
      plugins_jpeg_spec.source_files = 'ios/SpectrumKit/SpectrumKitPlugins/Jpeg/**/*'
    end

    plugins_spec.subspec 'Png' do |plugins_png_spec|
      plugins_png_spec.dependency 'SpectrumKit/Base', pinned_version
      plugins_png_spec.dependency 'SpectrumCore/Plugins/Png'
      plugins_png_spec.source_files = 'ios/SpectrumKit/SpectrumKitPlugins/Png/**/*'
    end

    plugins_spec.subspec 'Webp' do |plugins_webp_spec|
      plugins_webp_spec.dependency 'SpectrumKit/Base', pinned_version
      plugins_webp_spec.dependency 'SpectrumCore/Plugins/Webp'
      plugins_webp_spec.source_files = 'ios/SpectrumKit/SpectrumKitPlugins/Webp/**/*'
    end
  end
end
