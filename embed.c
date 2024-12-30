#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *basename(char const *path) {
  char *s = strrchr(path, '/');
  if (!s)
    return strdup(path);
  else
    return strdup(s + 1);
}

int embed_file(FILE *out_file, const char *prefix, const char *in_path) {
  char *name_str = basename(in_path);
  const int name_len = strlen(name_str);

  for (int i = 0; i < name_len; ++i) {
    switch (name_str[i]) {
    case '.':
    case '/':
    case '-':
      name_str[i] = '_';
      break;
    default:
      break;
    }
  }

  FILE *in_file = fopen(in_path, "r");
  if (in_file == NULL) {
    free(name_str);
    return 1;
  }

  fprintf(out_file, "#ifdef EMBED_IMPL\n");
  fprintf(out_file, "const unsigned char _embed_%s_%s[] = {", prefix, name_str);
  long embed_len = 0;
  while (1) {
    unsigned char b;
    int count = fread(&b, 1, 1, in_file);
    if (count == 0)
      break;
    fprintf(out_file, "0x%02x,", b);
    embed_len += 1;
  }

  fprintf(out_file, "};\nconst unsigned long _embed_%s_%s_len = %lu;\n", prefix,
          name_str, embed_len);

  fprintf(out_file, "#else\n");

  fprintf(out_file,
          "const unsigned char _embed_%s_%s[];\nconst unsigned long "
          "_embed_%s_%s_len;\n",
          prefix, name_str, prefix, name_str);

  fprintf(out_file, "#endif\n");

  free(name_str);
  fclose(in_file);
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 4)
    return 1;

  FILE *out_file = fopen(argv[1], "w");
  if (out_file == NULL) {
    return 1;
  }

  fprintf(out_file, "#ifndef EMBED_IMPL\n#pragma once\n#endif\n");
  fprintf(out_file, "#define LOAD_DATA(NAME) _embed_##NAME\n");
  fprintf(out_file, "#define LOAD_LEN(NAME) _embed_##NAME##_len\n");

  for (int i = 3; i < argc; ++i) {
    if (embed_file(out_file, argv[2], argv[i]) != 0) {
      fclose(out_file);
      return 1;
    }
  }

  fclose(out_file);
  return 0;
}
