TrueType font convert tool
--------------------------
This program was written by Adafruit and I modified it to map characters
128 to 255 from Unicode to the Microsoft codepage 1252 set. This allows
you to support nearly all European/Western languages with direct use
of UTF-8 Unicode strings from your projects. It's not a perfect solution,
but it's better than not being able to use those symbols and accented
characters. See the Unicode_demo for an example of how to use them.

Build the tool with GCC and Make. I've tested this on MacOS and Linux,
but it should work on Windows too.

