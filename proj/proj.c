// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>
// Any header files included below this line should have been created by you
#include "i8254.h"
#include "macro.h"
#include "macros.h"
#include "keyboard.h"
#include "mouse.h"
#include "video_gr.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  /* To be completed */
  printf("%s(%d, %p): under construction\n", __func__, argc, argv);

  return 0;
}
