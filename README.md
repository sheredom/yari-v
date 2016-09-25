# YARI-V #

[![Build status](https://ci.appveyor.com/api/projects/status/njqewxtgg59mkjo6?svg=true)](https://ci.appveyor.com/project/sheredom/yari-v)

[![Build status](https://api.travis-ci.org/repositories/sheredom/yari-v.svg)](https://travis-ci.org/sheredom/yari-v)


yariv.h is a single C/C++ header to encode and decode SPIR-V shaders into a more compressed form I've called YARI-V.

YARI-V is an alternative encoding of the SPIR-V standard that seeks to reduce the bytes required to encode shaders/kernels. In its default mode, it will losslessly convert a SPIR-V shader module into a corresponding YARI-V encoding with an average compression ratio of 34.42%.

# Usage #

Simply include the yariv.h header in your code, and then to encode, first call:

```
size_t yariv_encode_size(
  uint32_t encode_flags,
  const void *spirv,
  size_t spirv_size);
```

To get the size (in bytes) of memory required to hold the encoded YARI-V shader module.

Then call:

```
int yariv_encode(
  uint32_t encode_flags,
  void *out_yariv,
  size_t yariv_size,
  const void *spirv,
  size_t spirv_size);
```

Which will return 0 on success, and `out_yariv` will contain the YARI-V shader module.

Then, to decode, first call:

```
size_t yariv_decode_size(
  const void *yariv,
  size_t yariv_size);
```

To get the size (in bytes) of memory required to hold the decoded SPIR-V shader module.

Then call:

```
int yariv_decode(
  void *out_spirv,
  size_t spirv_size,
  const void *yariv,
  size_t yariv_size);
```

Which will return 0 on success, and `out_spirv` will contain the SPIR-V shader module.

# Results #

|         Approach          | Size (kilobytes) | Compression (%) |
| :------------------------ | ---------------: | --------------: |
| SPIR-V                    |    4868.468750   |   100.000000%   |
| SPIR-V + zstd20           |     590.573242   |    12.130575%   |
| SMOL-V                    |    1837.881836   |    37.750717%   |
| SMOL-V + zstd20           |     386.879883   |     7.946644%   |
| YARI-V                    |    1675.956055   |    34.424706%   |
| YARI-V + zstd20           |     390.577148   |     8.022587%   |
| SMOL-V(stripped)          |    1629.115234   |    33.462580%   |
| SMOL-V(stripped) + zstd20 |     348.073242   |     7.149542%   |
| YARI-V(stripped)          |    1498.666016   |    30.783108%   |
| YARI-V(stripped) + zstd20 |     364.057617   |     7.477867%   |

# License #

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

## Exceptions ##

The following folders are held under their own licenses (used for testing purposes only):

* test/external/dota2 - Copyright (c) Valve Corporation, all rights reserved.
* test/external/shadertoy - several most popular shaders from Shadertoy, converted to Vulkan SPIR-V via glslang. Copyrights by their individual authors (filename matches last component of shadertoy URL).
* test/external/talos - Copyright (c) 2002-2016 Croteam, all rights reserved.
tests/external/unity - various Unity shaders, produced through a HLSL -> DX11 bytecode -> HLSLcc -> glslang toolchain.
