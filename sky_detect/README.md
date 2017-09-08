# Sky detection

This implementation contains two algorithm for detecting sky in digital images.

1. Basic implementation based on color
2. Partial implementation based on paper "Sky Region Detection in a Single Image for Autonomous Ground Robot Navigation"

## Basic color based implementation
Images are loaded in BGR format.
Implementation based on the fact that blue channel intensity varies is in range 130 to 255 for sky.
Implementation provide sliders to adjust min-max intensity to see its effect.

## Based on paper
This is partial implementation as discribe in the paper mentioned above
http://journals.sagepub.com/doi/pdf/10.5772/56884

A slider is provided to adjust the threshold to see its effect.

