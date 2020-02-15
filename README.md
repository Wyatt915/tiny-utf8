tiny-utf8
=========

Takes a hexadecimal codepoint from stdin and prints the resulting character to stdout. That's it.

Usage
-----

To build:
```
$ gcc tinyutf8.c -o tiny-utf8
```

To run:
```
$ echo 203d | tiny-utf8
‽
```

Helper Script
-------------

I use the script unicode-select.sh to input characters for which I can't remember the compose key
sequence. It works with dmenu and rofi, meaning you can search for a character by its description.
Once selected, it will be inserted wherever the cursor is active. On my system, I have the ~/scripts
directory in my PATH, and have copied both UnicodeNamesList.txt and unicode-select.sh into it. I
also have the ~/bin directory in my PATH, into which I have placed the tiny-utf8 executable. The
most up-to-date version of the NamesList can always be found
[here](https://unicode.org/Public/UNIDATA/NamesList.txt). I am aware that the file explicitly states
that it is not to be parsed for machine-readable information. I do not care and have done it anyway.

License
-------

Except where otherwise stated (i.e. the Unicode NamesList), all code is released under the MIT license.

```
MIT License

Copyright (c) 2020 Wyatt Sheffield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
