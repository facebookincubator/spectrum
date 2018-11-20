// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class ImageDifferenceView: UIView, UIScrollViewDelegate {
  // MARK: - Public Properties

  var viewModel: ImageDifferenceViewModel? {
    didSet {
      guard self.viewModel !== oldValue else { return }
      self.viewModel.flatMap(self.updateView(with:))
    }
  }

  // MARK: - Private Properties

  // MARK: IBOutlets

  @IBOutlet private var scrollView: ZoomableScrollView!
  @IBOutlet private var sourceImageView: UIImageView!
  @IBOutlet private var targetImageView: UIImageView!
  @IBOutlet private var sliderView: UISlider!

  // MARK: - NSObject

  override func awakeFromNib() {
    super.awakeFromNib()

    if #available(iOS 11.0, *) {
      self.scrollView.contentInsetAdjustmentBehavior = .never
    }

    self.targetImageView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
    self.targetImageView.translatesAutoresizingMaskIntoConstraints = true
    self.targetImageView.frame = self.sourceImageView.bounds
    self.sourceImageView.addSubview(self.targetImageView)
  }

  // MARK: - UIScrollViewDelegate

  func viewForZooming(in _: UIScrollView) -> UIView? {
    return self.sourceImageView
  }

  func scrollViewDidZoom(_: UIScrollView) {
    self.scrollView.centreContentView()
    self.scrollView.layoutIfNeeded()
  }

  // MARK: - Public Methods

  func updateView(with viewModel: ImageDifferenceViewModel) {
    self.sliderView.value = viewModel.ratio
    self.targetImageView.alpha = viewModel.transcodedImageViewAlpha

    if self.sourceImageView.image != viewModel.sourceImage {
      self.sourceImageView.image = viewModel.sourceImage
      self.scrollView.updateZoomScaleIfNeeded(force: true)
    }

    if self.targetImageView.image != viewModel.transcodedImage {
      self.targetImageView.image = viewModel.transcodedImage
    }
  }

  // MARK: - Private Methods

  // MARK: IBActions

  @IBAction private func sliderValueDidChange(_ sender: UISlider) {
    self.viewModel!.ratio = sender.value
  }
}
