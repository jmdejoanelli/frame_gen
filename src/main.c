/* Josef de Joanelli (jmdejoanelli@gmail.com) */

#include "frame_gen.h"
#include "fill_patterns.h"
#include "pack.h"

void print_usage(void);

int main(int argc, char *argv[]){
  if (argc != 10){
    printf("%s : ERROR : Require 9 arguments, only %d provided...\n", argv[0], argc-1);
    print_usage();
    return 1;
  }

  printf("Frame Gen v0.1\n");

  packed_frame_t *packed_frame;
  packing_t packing = packing_decode_str(argv[3]);
  uint8_t cdepth = cdepth_from_packing(packing);
  colour_t colour = {atoi(argv[5]), atoi(argv[6]), atoi(argv[7])};

  /* Always RGB444 and 16 bit for raw frame. */
  frame_t *frame = frame_create(atoi(argv[1]), atoi(argv[2]), 16, RGB444);

  if (frame->n_pixels % 8 != 0){
    printf("%s : ERROR : Total number of pixels must be divisable by 8...\n", argv[0]);
    return 2;
  }

  printf("Frame width     : %d\n", frame->width);
  printf("Frame height    : %d\n", frame->height);
  printf("Colour depth    : %d bits\n", cdepth);
  printf("Pixels          : %d\n", frame->n_pixels);
  printf("Packing         : %s\n", argv[3]);
  printf("RGB             : (%d %d %d)\n", colour.r, colour.g, colour.b);


  switch (atoi(argv[4])) {
    case 0:
      printf("Pattern         : solid\n");
      break;
    case 1:
      printf("Pattern         : checker\n");
      break;
    case 2:
      printf("Pattern         : horizontal ramp\n");
      break;
    case 3:
      printf("Pattern         : vertical ramp\n");
      break;
    case 4:
      printf("Pattern         : diagonal ramp\n");
      break;
    case 5:
      printf("Pattern         : prbs15\n");
      break;
    default:
      break;
  }

  printf("File size       : %d bytes (%0.3f MB)\n", frame->n_pixels*6, (float)frame->n_pixels*6/1024/1024);

  switch (atoi(argv[4])) {
    case 0:
      fill_solid(frame, colour);
      break;
    case 1:
      fill_checker(frame, colour);
      break;
    case 2:
      fill_ramp_h(frame, colour, ((1 << cdepth) - 1));
      break;
    case 3:
      fill_ramp_v(frame, colour, ((1 << cdepth) - 1));
      break;
    case 4:
      fill_ramp_d(frame, colour, ((1 << cdepth) - 1));
      break;
    case 5:
      fill_prbs15(frame, colour);
      break;
    default:
      break;
  }

  frame_clamp(frame, 16, ((1 << cdepth) - 1)-16);

  if (strcmp(argv[8], "--") != 0){
    write_binary(frame, argv[8]);
  }

  switch (packing){
    case V210:
      // packed_frame = pack_V210(frame);
      printf("%s : ERROR : YCbCr colour space not currently supported...\n", argv[0]);
      return 3;
      break;
    case ARGB:
      packed_frame = pack_ARGB(frame);
      break;
    case BGRA:
      packed_frame = pack_BGRA(frame);
      break;
    case R210:
      packed_frame = pack_R210(frame);
      break;
    case R12L:
      packed_frame = pack_R12L(frame);
      break;
    case R12B:
      packed_frame = pack_R12B(frame);
      break;
    default:
      break;
  }

  printf("Packed file size: %d bytes (%0.3f MB)\n", packed_frame->len_32*4, (float)packed_frame->len_32*4/1024/1024);

  frame_destroy(frame);

  if (strcmp(argv[9], "--") != 0){
    write_packed_binary(packed_frame, argv[9]);
  }

  packed_frame_destroy(packed_frame);

  return 0;
}

void print_usage(void){
  printf("Usage: ./frame_gen width height depth pattern r g b frame_path packed_frame_path\n");
  printf("  width               frame width\n");
  printf("  height              frame height\n");
  printf("  depth               frame bit depth - 10 and 12 bits are valid\n");
  printf("  pattern             frame pattern:\n");
  printf("                        0 solid fill\n");
  printf("                        1 checker fill\n");
  printf("                        2 horizontal ramp\n");
  printf("                        3 vertical ramp\n");
  printf("                        4 diagonal ramp\n");
  printf("                        5 prbs15\n");
  printf("  r g b               RGB values used to fill the patterns\n");
  printf("  frame_path          path to output unpacked frame to\n");
  printf("  packed_frame_path   path to output packed frame to\n");
};
