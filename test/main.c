// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>

#include <stdio.h>

#include "utest.h"

#include <yariv.h>

#include <shaders.h>

struct yariv {
  void *spirv;
  size_t spirv_size;
};

UTEST_I_SETUP(yariv) {
  FILE *f = fopen(shaders[utest_index], "rb");

  ASSERT_TRUE(f);

  ASSERT_EQ(0, fseek(f, 0, SEEK_END));

  utest_fixture->spirv_size = ftell(f);

  ASSERT_NE(-1L, utest_fixture->spirv_size);

  ASSERT_EQ(0, fseek(f, 0, SEEK_SET));

  utest_fixture->spirv = malloc(utest_fixture->spirv_size);

  ASSERT_TRUE(utest_fixture->spirv);

  ASSERT_EQ(utest_fixture->spirv_size,
            fread(utest_fixture->spirv, 1, utest_fixture->spirv_size, f));

  fclose(f);
}

UTEST_I_TEARDOWN(yariv) { free(utest_fixture->spirv); }

UTEST_I(yariv, default, SHADERS_LENGTH) {
  void *out_yariv;
  size_t out_yariv_size;
  void *out_spirv;
  size_t out_spirv_size;
  size_t index;

  out_yariv_size =
      yariv_encode_size(yariv_encode_flags_default, utest_fixture->spirv,
                        utest_fixture->spirv_size);
  ASSERT_LT(0, out_yariv_size);

  out_yariv = malloc(out_yariv_size);
  ASSERT_TRUE(out_yariv);

  ASSERT_EQ(0,
            yariv_encode(yariv_encode_flags_default, out_yariv, out_yariv_size,
                         utest_fixture->spirv, utest_fixture->spirv_size));

  ASSERT_GE(utest_fixture->spirv_size, out_yariv_size);

  out_spirv_size = yariv_decode_size(out_yariv, out_yariv_size);
  ASSERT_EQ(utest_fixture->spirv_size, out_spirv_size);

  out_spirv = malloc(out_spirv_size);

  ASSERT_EQ(0,
            yariv_decode(out_spirv, out_spirv_size, out_yariv, out_yariv_size));

  for (index = 0; index < out_spirv_size; index++) {
    ASSERT_EQ(((char *)utest_fixture->spirv)[index],
              ((char *)out_spirv)[index]);
  }

  free(out_yariv);
  free(out_spirv);
}

UTEST_I(yariv, strip, SHADERS_LENGTH) {
  void *out_yariv;
  size_t out_yariv_size;
  void *out_spirv;
  size_t out_spirv_size;
  size_t i, k;

  out_yariv_size =
      yariv_encode_size(yariv_encode_flags_strip, utest_fixture->spirv,
                        utest_fixture->spirv_size);
  ASSERT_LT(0, out_yariv_size);

  out_yariv = malloc(out_yariv_size);
  ASSERT_TRUE(out_yariv);

  ASSERT_EQ(0, yariv_encode(yariv_encode_flags_strip, out_yariv, out_yariv_size,
                            utest_fixture->spirv, utest_fixture->spirv_size));

  ASSERT_GE(utest_fixture->spirv_size, out_yariv_size);

  out_spirv_size = yariv_decode_size(out_yariv, out_yariv_size);
  ASSERT_GE(utest_fixture->spirv_size, out_spirv_size);

  out_spirv = malloc(out_spirv_size);

  ASSERT_EQ(0,
            yariv_decode(out_spirv, out_spirv_size, out_yariv, out_yariv_size));

  for (i = 0; i < 5; i++) {
    ASSERT_EQ(((uint32_t*)utest_fixture->spirv)[i], ((uint32_t*)out_spirv)[i]);
  }

  for (i = 5, k = 5; k < (out_spirv_size / sizeof(uint32_t));) {
    const uint32_t word = ((uint32_t*)utest_fixture->spirv)[i];
    const uint32_t word_count = word >> 16;
    const uint32_t opcode = word & 0xFFFFu;

    if (word == ((uint32_t*)out_spirv)[k]) {
      // the opcodes matched, so we skip them both!
      i += word_count;
      k += word_count;
    } else {
      switch (opcode) {
        default:
          // we reordered or malformed the output SPIR-V! Not good :(
          ASSERT_TRUE(0);
        case YarivOpNop:
        case YarivOpSourceContinued:
        case YarivOpSource:
        case YarivOpSourceExtension:
        case YarivOpName:
        case YarivOpMemberName:
        case YarivOpString:
        case YarivOpLine:
        case YarivOpNoLine:
          // all of these opcodes are allowed to be stripped from the module

          // skip opcode (only in the original SPIR-V)
          i += word_count;
          break;
      }
    }
  }

  free(out_yariv);
  free(out_spirv);
}

UTEST_MAIN();
