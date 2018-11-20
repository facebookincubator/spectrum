// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Photos
import UIKit.UIImage

final class SpectrumController {
  struct Options {
    let encodeRequirement: EncodeRequirement?
    let rotateRequirement: RotateRequirement?
    let cropRequirement: CropRequirementProviding?
    let resizeRequirement: ResizeRequirement?
    let outputImagePixelSpecification: ImagePixelSpecification?
    let configuration: Configuration?
    let metadata: ImageMetadata?

    func makeEncodeOptions() -> EncodeOptions {
      return EncodeOptions(encodeRequirement: self.encodeRequirement!,
                           transformations: self.makeTransformations(),
                           metadata: self.metadata,
                           configuration: self.configuration,
                           outputPixelSpecificationRequirement: self.outputImagePixelSpecification)
    }

    func makeTranscodeOptions() -> TranscodeOptions {
      return TranscodeOptions(encodeRequirement: self.encodeRequirement!,
                              transformations: self.makeTransformations(),
                              metadata: self.metadata,
                              configuration: self.configuration,
                              outputPixelSpecificationRequirement: self.outputImagePixelSpecification)
    }

    private func makeTransformations() -> Transformations {
      return Transformations(resizeRequirement: self.resizeRequirement,
                             rotateRequirement: self.rotateRequirement,
                             cropRequirement: self.cropRequirement)
    }
  }

  typealias Completion = (Result<ImageRepresentation>) -> Void

  // MARK: Public Methods

  static func transcode(options: Options,
                        source: ImageSource,
                        inputType: IOType,
                        outputType: IOType,
                        completion: @escaping Completion) {
    DispatchQueue.global().async {
      let result: Result<ImageRepresentation>

      do {
        let imageRepresentation = try SpectrumController.makeTranscodeResult(options: options,
                                                                             source: source,
                                                                             inputType: inputType,
                                                                             outputType: outputType)

        result = .success(imageRepresentation)
      } catch {
        SpectrumController.printError(error)
        result = .error(error)
      }

      DispatchQueue.main.async {
        completion(result)
      }
    }
  }

  // MARK: Private Methods

  private static func makeTranscodeResult(options: Options,
                                          source: ImageSource,
                                          inputType: IOType,
                                          outputType: IOType) throws -> ImageRepresentation {
    let dateStarted = Date()
    var error: NSError?
    var resultData: ResultData?

    switch (inputType, outputType) {
    case (.bitmap, .file):
      resultData = Spectrum.shared.encodeImage(source.image,
                                               options: options.makeEncodeOptions(),
                                               error: &error)
    case (.file, .file):
      if let sourceUrl = source.url {
        resultData = Spectrum.shared.transcodeImage(fromFileAt: sourceUrl,
                                                    options: options.makeTranscodeOptions(),
                                                    error: &error)
      } else {
        throw Error.noImageInInfo
      }
    case (_, .bitmap):
      // This will be tacked as soon as FSPSpectrum supports decoding / transforming.
      throw Error.unsupportedBitmapOutput
    }

    if let resultData = resultData, let data = resultData.data, resultData.result.didSucceed {
      SpectrumController.printResult(resultData, dateStarted: dateStarted)
      return .bytes(data)
    } else {
      throw error ?? Error.unknown
    }
  }

  private static func printResult(_ resultData: ResultData, dateStarted: Date) {
    print("# Operation completed")
    print("# Operation duration: \(-dateStarted.timeIntervalSinceNow)")
    print("# Did succeed: \(resultData.result.didSucceed)")
    print("# Rule: \(resultData.result.ruleName ?? "none")")

    if
      let data = resultData.data,
      let source = CGImageSourceCreateWithData(data as CFData, nil),
      let properties = CGImageSourceCopyPropertiesAtIndex(source, 0, nil) {
      print("# Exif: \(properties)")
    }
  }

  private static func printError(_ error: Swift.Error) {
    print("# Operation failed")
    print("# Error: \(error.localizedDescription)")

    let nsError = error as NSError
    if nsError.userInfo.isEmpty == false {
      print("# Error Info: \(nsError.userInfo)")
    }
  }
}
