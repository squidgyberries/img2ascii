#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

const char *ramp = "$@B%&M#*oahbpwmZOLCJUYXzcvuxrf/|(1[?-+~<i!lI:,\"'. ";
const float ratio = (float)(49.0 / 255.0);

const char *help = "Usage\n"
                   "\n"
                   "  img2ascii [options] <path-to-image>\n"
                   "\n"
                   "Options\n"
                   "\n"
                   "  --scale-w, --scale-width <value>  = Amount to scale width (default = 1)\n"
                   "  --scale-h, --scale-height <value> = Amount to scale height (default = 1)\n"
                   "\n"
                   "  -h, --help                        = Print usage information and exit\n"
                   "  --version                         = Print version number and exit\n"
                   "\n"
                   "Image Formats\n"
                   "\n"
                   "  JPEG\n"
                   "  PNG\n"
                   "  BMP\n"
                   "  PSD\n"
                   "  TGA\n"
                   "  GIF\n"
                   "  PIC\n"
                   "  PNM (PPM and PGM)";
const char *version = "0.1.0";

int main(int argc, char **argv) {
  uintmax_t scalew = 1, scaleh = 1;
  char *imgf = NULL;

  if (argc < 2) {
    printf("%s\n", help);
    exit(EXIT_SUCCESS);
  }

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      printf("%s\n", help);
      exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[i], "--version")) {
      printf("%s\n", version);
      exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[i], "--scale-w") || !strcmp(argv[i], "--scale-width")) {
      if (!(argc > i + 1)) {
        printf("error: %s: No value passed", argv[i]);
        exit(EXIT_FAILURE);
      }
      errno = 0;
      scalew = strtoumax(argv[++i], NULL, 10);
      if (errno) {
        char errmsg[128];
        strerror_s(errmsg, 128, errno);
        printf("error: %s: %s\n", argv[i], errmsg);
        exit(EXIT_FAILURE);
      }
    } else if (!strcmp(argv[i], "--scale-h") || !strcmp(argv[i], "--scale-height")) {
      if (!(argc > i + 1)) {
        printf("error: %s: No value passed", argv[i]);
        exit(EXIT_FAILURE);
      }
      errno = 0;
      scaleh = strtoumax(argv[++i], NULL, 10);
      if (errno) {
        char errmsg[128];
        strerror_s(errmsg, 128, errno);
        printf("error: %s: %s\n", argv[i], errmsg);
        exit(EXIT_FAILURE);
      }
    } else {
      imgf = argv[i];
      break;
    }
  }

  if (imgf == NULL) {
    printf("error: No image file passed\n");
    exit(EXIT_FAILURE);
  }

  int fcomp = 3; // Force 3 components, rgb
  int x, y;
  unsigned char *image = stbi_load(imgf, &x, &y, NULL, fcomp); // NULL for comp because no need
  if (image == NULL) {
    printf("error: %s\n", stbi_failure_reason());
    exit(EXIT_FAILURE);
  }

  // Heap array to void chkstk error on Windows
  char *out = (char *)malloc(
      sizeof(char) * ((size_t)scalew * scaleh * x * y + scaleh * y +
                         1)); // scaleh * y for newline characters, 1 for null terminate
  if (out == NULL) {
    printf("error: Malloc failed");
    stbi_image_free(image);
    exit(EXIT_FAILURE);
  }

  for (int i = 0, outi = 0; i < y; i++) { // outi is out string index
    for (uintmax_t h = 0; h < scaleh; h++) {
      for (int j = 0; j < x; j++) {
        int index = (i * x + j) * 3; // Image index
        int brightness = (image[index] + image[index + 1] + image[index + 2]) / 3;
        int chari = (int)(brightness * ratio);
        char c = ramp[chari];
        for (uintmax_t w = 0; w < scalew; w++) {
          out[outi] = c;
          outi++;
        }
      }
      out[outi] = '\n';
      outi++;
    }
  }
  out[scalew * scaleh * x * y + scaleh * y] = '\0';
  printf("%s", out);

  free(out);
  stbi_image_free(image);
  return EXIT_SUCCESS;
}