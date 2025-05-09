#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  CP_1251,
  KOI_8_R,
  ISO_8859_5
} Encoding;

unsigned short koi8r_to_unicode[128] = {
  0x2500, 0x2502, 0x250C, 0x2510, 0x2514, 0x2518, 0x251C, 0x2524,
  0x252C, 0x2534, 0x253C, 0x2580, 0x2584, 0x2588, 0x258C, 0x2590,
  0x2591, 0x2592, 0x2593, 0x2320, 0x25A0, 0x2219, 0x221A, 0x2248,
  0x2264, 0x2265, 0x00A0, 0x2321, 0x00B0, 0x00B2, 0x00B7, 0x00F7,
  0x2550, 0x2551, 0x2552, 0x0451, 0x2553, 0x2554, 0x2555, 0x2556,
  0x2557, 0x2558, 0x2559, 0x255A, 0x255B, 0x255C, 0x255D, 0x255E,
  0x255F, 0x2560, 0x2561, 0x0401, 0x2562, 0x2563, 0x2564, 0x2565,
  0x2566, 0x2567, 0x2568, 0x2569, 0x256A, 0x256B, 0x256C, 0x00A9,
  0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433,
  0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
  0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432,
  0x044C, 0x044B, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044A,
  0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413,
  0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
  0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412,
  0x042C, 0x042B, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042A
};

unsigned short iso8859_5_to_unicode[128] = {
  0x00A0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407,
  0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x00AD, 0x040E, 0x040F,
  0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
  0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
  0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
  0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
  0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
  0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
  0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
  0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457,
  0x0458, 0x0459, 0x045A, 0x045B, 0x045C, 0x00A7, 0x045E, 0x045F
};

unsigned short cp1251_to_unicode(unsigned char c) {
  if (c < 0x80) return c;
  static const unsigned short cp1251_table[128] = {
    0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021,
    0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
    0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x00,   0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
    0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7,
    0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
    0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7,
    0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
    0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
    0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
    0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
    0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F
  };
  return cp1251_table[c - 0x80];
}

void encode(unsigned short codepoint, FILE *out) 
{
  if(codepoint < 0x80) {
    fputc(codepoint, out);
  } else if(codepoint < 0x800) {
    fputc(0xC0 | (codepoint >> 6), out);
    fputc(0x80 | (codepoint & 0x3F), out);
  } else {
    fputc(0xE0 | (codepoint >> 12), out);
    fputc(0x80 | ((codepoint >> 6) & 0x3F), out);
    fputc(0x80 | (codepoint & 0x3F), out);
  }
}

Encoding parse_encoding(const char *format) {
  if(strcmp(format, "CP-1251") == 0) return CP_1251;
  if(strcmp(format, "KOI8-R") == 0) return KOI_8_R;
  if(strcmp(format, "ISO-8859-5") == 0) return ISO_8859_5;
  fprintf(stderr, "Unknown format: %s\n", format);
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <input> <encoding> <output>\n", argv[0]);
    return 1;
  }

  // аргумент командной строка 1
  const char *input_path = argv[1];
  // аргумент командной строка 2
  const char *encoding = argv[2];
  // аргумент командной строка 3
  const char *output_path = argv[3];

  FILE *fin = fopen(input_path, "rb");
  if(fin == NULL) 
  {
    perror("fopen error");
    return 1;
  }

  FILE *fout = fopen(output_path, "wb");
  if (!fout) {
    perror("fopen output");
    fclose(fin);
    return 1;
  }

  Encoding enc = parse_encoding(encoding);

  int ch;
  
  while((ch = fgetc(fin)) != EOF) {
    unsigned short codepoint;
    if(ch < 0x80) {
      codepoint = ch;
    } else {
      switch (enc) {
        case CP_1251:
          codepoint = cp1251_to_unicode(ch);
          break;
        case KOI_8_R:
          codepoint = koi8r_to_unicode[ch - 0x80];
          break;
        case ISO_8859_5:
          codepoint = iso8859_5_to_unicode[ch - 0x80];
          break;
      }
    }
    encode(codepoint, fout);
  }
  
  fclose(fin);
  fclose(fout);
  return 0;
}
