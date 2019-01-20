// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

protocol ConfigurationViewModelDelegate: class {
  func viewModelDidUpdate(_ viewModel: ConfigurationViewModel)
}

protocol ConfigurationViewModelInternalDelegate: class {
  func internalViewModelDidUpdate(_ viewModel: Any)
}

final class ConfigurationViewModel: ConfigurationViewModelInternalDelegate {
  // MARK: Public Types

  class General {
    // MARK: - Public Properties

    weak var delegate: ConfigurationViewModelInternalDelegate?

    var defaultBackgroundColor: DefaultBackgroundColor {
      get { return DefaultBackgroundColor(color: self.configuration.defaultBackgroundColor) }
      set {
        self.configuration.defaultBackgroundColor = newValue.color
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var interpretMetadata: Bool {
      get { return self.configuration.interpretMetadata }
      set {
        self.configuration.interpretMetadata = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var propagateChromaSamplingModeFromSource: Bool {
      get { return self.configuration.propagateChromaSamplingModeFromSource }
      set {
        self.configuration.propagateChromaSamplingModeFromSource = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var chromaSamplingModeOverride: ConfigurationChromaSamplingMode {
      get { return self.configuration.chromaSamplingModeOverride }
      set {
        self.configuration.chromaSamplingModeOverride = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    // MARK: - Private Properties

    private let configuration: ConfigurationGeneral

    // MARK: - Public Methods

    init(configuration: ConfigurationGeneral) {
      self.configuration = configuration
    }
  }

  class Jpeg {
    // MARK: - Public Properties

    weak var delegate: ConfigurationViewModelInternalDelegate?

    var useTrellis: Bool {
      get { return self.configuration.useTrellis }
      set {
        self.configuration.useTrellis = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var useProgressive: Bool {
      get { return self.configuration.useProgressive }
      set {
        self.configuration.useProgressive = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var useOptimizeScan: Bool {
      get { return self.configuration.useOptimizeScan }
      set {
        self.configuration.useOptimizeScan = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var useCompatibleDCScanOption: Bool {
      get { return self.configuration.useCompatibleDCScanOption }
      set {
        self.configuration.useCompatibleDCScanOption = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var usePSNRQuantTable: Bool {
      get { return self.configuration.usePSNRQuantTable }
      set {
        self.configuration.usePSNRQuantTable = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    // MARK: - Private Properties

    private let configuration: ConfigurationJpeg

    // MARK: - Public Methods

    init(configuration: ConfigurationJpeg) {
      self.configuration = configuration
    }
  }

  class Png {
    // MARK: - Public Properties

    weak var delegate: ConfigurationViewModelInternalDelegate?

    var useInterlacing: Bool {
      get { return self.configuration.useInterlacing }
      set {
        self.configuration.useInterlacing = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }
    
    var compressionLevel: CompressionLevel {
      get { return CompressionLevel(rawValue: self.configuration.compressionLevel)! }
      set {
        self.configuration.compressionLevel = newValue.rawValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    // MARK: - Private Properties

    private let configuration: ConfigurationPng

    // MARK: - Public Methods

    init(configuration: ConfigurationPng) {
      self.configuration = configuration
    }
  }

  class Webp {
    // MARK: - Public Properties

    weak var delegate: ConfigurationViewModelInternalDelegate?

    var method: WebpMethod {
      get { return WebpMethod(rawValue: self.configuration.method)! }
      set {
        self.configuration.method = newValue.rawValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    var imageHint: ConfigurationWebpImageHint {
      get { return self.configuration.imageHint }
      set {
        self.configuration.imageHint = newValue
        self.delegate?.internalViewModelDidUpdate(self)
      }
    }

    // MARK: - Private Properties

    private let configuration: ConfigurationWebp

    // MARK: - Public Methods

    init(configuration: ConfigurationWebp) {
      self.configuration = configuration
    }
  }

  // MARK: Public Properties

  let general: General
  let jpeg: Jpeg
  let png: Png
  let webp: Webp

  weak var delegate: ConfigurationViewModelDelegate?

  // MARK: Public Methods

  init(configuration: Configuration) {
    self.general = General(configuration: configuration.general)
    self.jpeg = Jpeg(configuration: configuration.jpeg)
    self.png = Png(configuration: configuration.png)
    self.webp = Webp(configuration: configuration.webp)

    self.general.delegate = self
    self.jpeg.delegate = self
    self.png.delegate = self
    self.webp.delegate = self
  }

  // MARK: - ConfigurationViewModelInternalDelegate

  func internalViewModelDidUpdate(_: Any) {
    self.delegate?.viewModelDidUpdate(self)
  }
}
