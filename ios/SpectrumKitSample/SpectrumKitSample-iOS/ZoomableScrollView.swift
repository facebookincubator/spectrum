// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class ZoomableScrollView: UIScrollView {
  // MARK: - Private Properties

  @IBOutlet private var contentViewTopConstraint: NSLayoutConstraint!
  @IBOutlet private var contentViewLeadingConstraint: NSLayoutConstraint!
  @IBOutlet private var contentView: UIView!

  private var lastUpdateSizes: (bounds: CGSize, intrinsicContentSize: CGSize)?

  private var zoomScaleRange: ClosedRange<CGFloat>? {
    guard self.minimumZoomScale < self.maximumZoomScale else { return nil }
    return self.minimumZoomScale ... self.maximumZoomScale
  }

  private var sizeChangedSinceLastUpdate: Bool {
    return self.lastUpdateSizes?.bounds != self.bounds.size ||
      self.lastUpdateSizes?.intrinsicContentSize != self.contentView.intrinsicContentSize
  }

  // MARK: - Public Methods

  func centreContentView() {
    let constants = (top: (self.frame.height - self.contentView.frame.height) / 2,
                     leading: (self.frame.width - self.contentView.frame.width) / 2)

    self.contentViewTopConstraint.constant = max(round(constants.top), 0)
    self.contentViewLeadingConstraint.constant = max(round(constants.leading), 0)
  }

  func updateZoomScaleIfNeeded(force: Bool = false) {
    guard self.sizeChangedSinceLastUpdate || force else { return }

    self.lastUpdateSizes = (bounds: self.frame.size, intrinsicContentSize: self.contentView.intrinsicContentSize)

    let wasMinimumZoomScale = self.zoomScale == self.minimumZoomScale || force
    self.minimumZoomScale = min(self.frame.width / self.contentView.intrinsicContentSize.width,
                                self.frame.height / self.contentView.intrinsicContentSize.height)
    self.maximumZoomScale = self.minimumZoomScale * 5

    if wasMinimumZoomScale || self.zoomScaleRange?.contains(self.zoomScale) != true {
      self.zoomScale = self.minimumZoomScale
    }

    self.centreContentView()
  }

  // MARK: - Private Methods

  private func toggleZoomLevel() {
    self.setZoomScale(self.zoomScale == self.minimumZoomScale ? self.maximumZoomScale : self.minimumZoomScale,
                      animated: true)
  }

  // MARK: IBActions

  @IBAction private func didDoubleTap(_: UITapGestureRecognizer) {
    self.toggleZoomLevel()
  }
}
