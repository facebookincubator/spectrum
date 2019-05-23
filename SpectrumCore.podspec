# Copyright (c) Facebook, Inc. and its affiliates.

compiler_flags = '-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1 -DSPECTRUM_TARGET_IOS -DSPECTRUM_EXTERNAL'

version = '1.1.0'

Pod::Spec.new do |spec|
  spec.name = 'SpectrumCore'
  spec.version = version
  spec.summary = 'A client-side image transcoding library.'
  spec.description = <<-DESC
Spectrum is a cross-platform image transcoding library that can easily be integrated into an iOS project to efficiently perform common image operations.
                       DESC

  spec.homepage = 'http://libspectrum.io'
  spec.license = { :type => 'MIT', :file => 'LICENSE' }
  spec.authors = 'Facebook'
  spec.source = { :git => 'https://github.com/facebookincubator/spectrum.git', :tag => "v#{version}" }
  spec.ios.deployment_target = '8.0'
  spec.default_subspecs = 'Plugins/Jpeg',
                          'Plugins/Png',
                          'Plugins/Webp'

  spec.dependency 'spectrum-folly', '~> 2019.01.21.00'
  spec.compiler_flags = compiler_flags

  spec.subspec 'Base' do |base_spec|
    base_spec.header_dir = 'spectrum'
    base_spec.header_mappings_dir = 'cpp/spectrum'
    base_spec.source_files = 'cpp/spectrum/**/*.{h,cpp}'
    base_spec.exclude_files = 'cpp/spectrum/plugins'
  end

  spec.subspec 'Plugins' do |plugins_spec|
    plugins_spec.subspec 'Jpeg' do |plugins_jpeg_spec|
      plugins_jpeg_spec.dependency 'SpectrumCore/Base', version
      plugins_jpeg_spec.dependency 'mozjpeg', '3.3.2'
      plugins_jpeg_spec.source_files = 'cpp/spectrum/plugins/jpeg/**/*.{h,cpp}'
      plugins_jpeg_spec.header_dir = 'spectrum/plugins/jpeg'
      plugins_jpeg_spec.header_mappings_dir = 'cpp/spectrum/plugins/jpeg'
    end

    plugins_spec.subspec 'Png' do |plugins_png_spec|
      plugins_png_spec.dependency 'SpectrumCore/Base', version
      plugins_png_spec.dependency 'libpng', '~> 1.6.35'
      plugins_png_spec.source_files = 'cpp/spectrum/plugins/png/**/*.{h,cpp}'
      plugins_png_spec.header_dir = 'spectrum/plugins/png'
      plugins_png_spec.header_mappings_dir = 'cpp/spectrum/plugins/png'
    end

    plugins_spec.subspec 'Webp' do |plugins_webp_spec|
      plugins_webp_spec.dependency 'SpectrumCore/Base', version
      plugins_webp_spec.dependency 'libwebp', '~> 1.0.2'
      plugins_webp_spec.source_files = 'cpp/spectrum/plugins/webp/**/*.{h,cpp}'
      plugins_webp_spec.header_dir = 'spectrum/plugins/webp'
      plugins_webp_spec.header_mappings_dir = 'cpp/spectrum/plugins/webp'
    end
  end
end
