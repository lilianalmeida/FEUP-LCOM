PROG=proj

SRCS = proj.c bitmap.c keyboard.c mouse.c video_gr.c timer.c game.c menus.c initGame.c spriteMovement.c pointSystem.c rtc.c serial_port.c kbd_Assembly.S rtc_Assembly.S

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF} ${LIBLM}
LDADD += -llcf -llm

.include <minix.lcom.mk>
