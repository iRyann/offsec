# John Locke Steganography Challenge

Flag format: `pyr-ctf{FLAG}`

## Recovery steps

1. Parse `john.png` and stop at the `IEND` chunk. There are 10,766 extra bytes after that marker that start with a second PNG header.
2. Extract those trailing bytes to a new file (e.g., `john2.png`) to reveal a hidden 603x60 RGBA image.
3. Invert and autocontrast the hidden image, then run OCR to read the text. `tesseract john2_proc.png stdout` returns the flag.

## Flag

`pyr-ctf{h1d1ing_is_@_thing}`
