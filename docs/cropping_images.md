---
id: cropping_images
title: Cropping Images
---

To crop an image, define a crop requirement in the options' transformations. A crop requirement can be defined in multiple ways:

- _Relatively_: Specify relative values which should be evaluated against the image dimensions. Each value starts from the top/left axis. For example, `top: 0.25, left: 0.25, bottom: 0.75, right: 0.75` crops the centre of an image.
- _Absolutely_: Specify absolute values measured in pixel of the original image. Each value starts from the top/left axis. For example, `top: 100, left: 100, bottom: 200, right: 250` crops an image of size `150x100`.

Finally, a crop requirement can also be approximate (controlled via `mustBeExact`) which may in some cases be more efficient.

## Code Sample

The following code exactly crops the centre point of the image:

### Android

```java
final CropRequirement cropRequirement =
    CropRequirement.makeRelativeToOrigin(
        /* top */ 0.25,
        /* left */ 0.25,
        /* bottom */ 0.75,
        /* right */ 0.75,
        /* mustBeExact */ true);

// Pass crop requirement to the options' transformations
```

### iOS

```objc
FSPRelativeToOriginCropRequirement *cropRequirement =
    [FSPRelativeToOriginCropRequirement relativeToOriginCropRequirementWithValues:{
      .top = 0.25f,
      .left = 0.25f,
      .bottom = 0.75f,
      .right = 0.75f,
    } mustBeExact:YES];

// Pass crop requirement to the options' transformations
```
