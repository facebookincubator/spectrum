---
id: rotating_images
title: Rotating Images
---
To rotate an image, define a rotate requirement in the options' transformations. A rotate requirement has the following parameters:

- Rotate the image by the given degrees. This changes the final image orientation either by changing the metadata's orientation value or reordering the pixels.
- Flip the image, either horizontally or vertically.
- Force the up orientation. This ensures that the output does not have a non-default metadata orientation. Instead, the pixel data is rotated. Some image formats do not support a metadata encoded orientation. In this case forcing the up orientation will not have any effect.

## Code Sample

The following code rotates the image 90 degrees clockwise while forcing the up-orientation:

### Android

```java
final RotateRequirement rotateRequirement =
    new RotateRequirement(
        /* degrees */ 90,
        /* flipVertically */ false,
        /* flipHorizontally */ false,
        /* forceUpOrientation */ true);

// Pass rotate requirement to the options' transformations
```

### iOS

```objc
FSPRotateRequirement *rotateRequirement = [FSPRotateRequirement
    rotateRequirementWithDegrees:90
          shouldFlipHorizontally:NO
            shouldFlipVertically:NO
        shouldForceUpOrientation:YES];

// Pass crop requirement to the options' transformations
```
