// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

protocol ImageDifferenceViewControllerDelegate: class {
  func imageDifferenceViewControllerDidTapCloseButton(_ imageDifferenceViewController: ImageDifferenceViewController)
}

final class ImageDifferenceViewController: UIViewController, ImageDifferenceViewModelDelegate {
  // MARK: - Public Properties

  weak var delegate: ImageDifferenceViewControllerDelegate?

  var viewModel: ImageDifferenceViewModel? {
    didSet {
      guard self.viewModel !== oldValue else { return }

      if oldValue?.delegate === self {
        oldValue?.delegate = nil
      }

      self.viewModel?.delegate = self

      if self.isViewLoaded {
        self.mView.viewModel = self.viewModel
      }
    }
  }

  // MARK: - Private Properties

  private var mView: ImageDifferenceView {
    return self.view as! ImageDifferenceView
  }

  // MARK: IBOutlets

  @IBOutlet private var scrollView: ZoomableScrollView!

  // MARK: - UIViewController

  override var preferredStatusBarStyle: UIStatusBarStyle {
    return .lightContent
  }

  override func viewDidLoad() {
    super.viewDidLoad()

    guard let viewModel = self.viewModel else {
      fatalError("View model should be set by viewDidLoad()")
    }

    self.mView.viewModel = viewModel
  }

  override func viewDidLayoutSubviews() {
    super.viewDidLayoutSubviews()

    self.scrollView.updateZoomScaleIfNeeded()
  }

  // MARK: - ImageDifferenceViewModelDelegate

  func imageDifferenceViewModelDidChange(_ viewModel: ImageDifferenceViewModel) {
    self.mView.updateView(with: viewModel)
  }

  // MARK: - Private Methods

  // MARK: IBActions

  @IBAction private func didTapCloseButton(_: UIButton) {
    self.delegate?.imageDifferenceViewControllerDidTapCloseButton(self)
  }
}
