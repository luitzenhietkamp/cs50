# Questions

## What's `stdint.h`?

`stdint.h` is a header file in the C standard library, that, looking at its name and whatever else is going on in `bmp.h`, introduces a set of integers of specific width.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

It allows you to use small or specific ints which lets you store information in a more concise way. It's also useful when a header needs to be a certain size. Using a uint32 to store a uint8 might otherwise cause problems.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

`BYTE = 1 byte`, `DWORD = 4 byte`, `LONG = 4 byte`, `WORD = 2 byte`.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM

## What's the difference between `bfSize` and `biSize`?

`bfSize` refers to the size of the bitmpa file, `biSize` refers to the size of the BITMAPINFOHEADER structure

## What does it mean if `biHeight` is negative?

It means that the image is stored as a top-down DIB and the origin is the top left corner (in the same way as our eyes see it).

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If program cannot open the file (e.g. network location not available) or create the file (e.g. no write access).

## Why is the third argument to `fread` always `1` in our code?

Because we read only 1 "item" at once.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

Sets the position indicator associated with the stream to a new position. In the case of `fseek(inptr, padding, SEEK_CUR)` advances inptr from the current position with `padding` to `SEEK_CUR + padding`.
Or in other words, skips the file pointer `inptr` beyond `padding`.

## What is `SEEK_CUR`?

The current position of the file pointer.

## Whodunit?

It was Professor Plum with the candlestick in the library.
