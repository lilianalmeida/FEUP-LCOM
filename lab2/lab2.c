#include <lcom/lcf.h>

#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");
    
    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");
    
    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab2/output.txt");
    
    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;
    
    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();
    
    return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
    /* To be completed by the students */
    uint8_t st;
    
    int erro = timer_get_conf(timer, &st);
    if (erro != 0) {
        printf("Error in timer_get_conf", 0);
        return erro;
    }
    
    erro = timer_display_conf(timer, st, field);
    if (erro != 0) {
        printf("Error in timer_display_conf", 0);
        return erro;
    }
    
    return 0;
}

int(timer_test_time_base)(uint8_t UNUSED(timer), uint32_t UNUSED(freq)) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);
    
    return 1;
}

int(timer_test_int)(uint8_t UNUSED(time)) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);
    
    return 1;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    
    val = val << 8;
    val = val >> 8;

    *lsb = val;   
    
    
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
   
     val = val >> 8;
    val = val << 8;
    
    *msb = val;   
    
    
    return 0;
}
