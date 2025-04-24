#include <stdio.h>
#include <iconv.h>
#include <errno.h>

#define BUF_SIZE 4096

int main(const int argc, char **argv) {
    // Если количество аргументов для командной строки не равно 4ем
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

    //открывает входной файл, который передали через аргумент командной строки
    //в режиме rb - readbin
    FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        perror("fopen input");
        return 1;
    }

    //открывает выходной файл, который передали через аргумент командной строки
    //в режиме wb - writebin
    FILE *fout = fopen(output_path, "wb");
    if (!fout) {
        perror("fopen output");
        fclose(fin);
        return 1;
    }

    char from_enc[32];

    // безопасный способ форматировать строку в буфер ограниченного размера
    // from_enc - указатель на буфер,куда будет записана строка
    // maxlen - максимальный размер буфера включая нулевой терминатор \0
    // format - формат
    // encoding - аргумент из командной строки
    snprintf(from_enc, sizeof(from_enc), "%s", encoding);

    iconv_t convert = iconv_open("UTF-8", from_enc);

    if (convert == (iconv_t)(-1)) {
        perror("iconv_open");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    char inbuf[BUF_SIZE];
    char outbuf[BUF_SIZE * 2];
    size_t inbytes, outbytes;

    while ((inbytes = fread(inbuf, 1, BUF_SIZE, fin)) > 0) {
        char *inptr = inbuf;

        while (inbytes > 0) {
            char *outptr = outbuf;
            outbytes = sizeof(outbuf);

            size_t res = iconv(convert, &inptr, &inbytes, &outptr, &outbytes);
            if (res == (size_t)(-1)) {
                if (errno == EILSEQ) {
                    fprintf(stderr, "Invalid multibyte sequence\n");
                } else if (errno == EINVAL) {
                    fprintf(stderr, "Incomplete multibyte sequence\n");
                } else if (errno == E2BIG) {
                    // output buffer full — handle gracefully
                } else {
                    perror("iconv");
                }
                iconv_close(convert);
                fclose(fin);
                fclose(fout);
                return 1;
            }

            fwrite(outbuf, 1, outptr - outbuf, fout);
        }
    }

    iconv_close(convert);
    fclose(fin);
    fclose(fout);

    printf("Conversion complete: %s → UTF-8 → %s\n", encoding, output_path);
    return 0;
}

