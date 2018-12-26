#include <lcom/lcf.h>
#include <stdio.h>
#include <math.h>

#include "pointSystem.h"
#include "initGame.h"
#include "playerMovement.h"
#include "bitmap.h"
#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "rtc_macros.h"
#include "rtc.h"


static int32_t score = 0;
static Numbers numbers;
static Time_RTC time_rtc;

void loadGameNumbers(){
  Bitmap* No0 = loadBitmap("/home/lcom/labs/proj/bmp/No0.bmp");
  Bitmap* No1 = loadBitmap("/home/lcom/labs/proj/bmp/No1.bmp");
  Bitmap* No2 = loadBitmap("/home/lcom/labs/proj/bmp/No2.bmp");
  Bitmap* No3 = loadBitmap("/home/lcom/labs/proj/bmp/No3.bmp");
  Bitmap* No4 = loadBitmap("/home/lcom/labs/proj/bmp/No4.bmp");
  Bitmap* No5 = loadBitmap("/home/lcom/labs/proj/bmp/No5.bmp");
  Bitmap* No6 = loadBitmap("/home/lcom/labs/proj/bmp/No6.bmp");
  Bitmap* No7 = loadBitmap("/home/lcom/labs/proj/bmp/No7.bmp");
  Bitmap* No8 = loadBitmap("/home/lcom/labs/proj/bmp/No8.bmp");
  Bitmap* No9 = loadBitmap("/home/lcom/labs/proj/bmp/No9.bmp");

  numbers.No0 = No0;
  numbers.No1 = No1;
  numbers.No2 = No2;
  numbers.No3 = No3;
  numbers.No4 = No4;
  numbers.No5 = No5;
  numbers.No6 = No6;
  numbers.No7 = No7;
  numbers.No8 = No8;
  numbers.No9 = No9;
}
void loadHighScoreNumbers(){
  Bitmap* No0 = loadBitmap("/home/lcom/labs/proj/bmp/No0Y.bmp");
  Bitmap* No1 = loadBitmap("/home/lcom/labs/proj/bmp/No1Y.bmp");
  Bitmap* No2 = loadBitmap("/home/lcom/labs/proj/bmp/No2Y.bmp");
  Bitmap* No3 = loadBitmap("/home/lcom/labs/proj/bmp/No3Y.bmp");
  Bitmap* No4 = loadBitmap("/home/lcom/labs/proj/bmp/No4Y.bmp");
  Bitmap* No5 = loadBitmap("/home/lcom/labs/proj/bmp/No5Y.bmp");
  Bitmap* No6 = loadBitmap("/home/lcom/labs/proj/bmp/No6Y.bmp");
  Bitmap* No7 = loadBitmap("/home/lcom/labs/proj/bmp/No7Y.bmp");
  Bitmap* No8 = loadBitmap("/home/lcom/labs/proj/bmp/No8Y.bmp");
  Bitmap* No9 = loadBitmap("/home/lcom/labs/proj/bmp/No9Y.bmp");
  Bitmap* NoDash = loadBitmap("/home/lcom/labs/proj/bmp/NoDashY.bmp");
  Bitmap* NoSlash = loadBitmap("/home/lcom/labs/proj/bmp/NoSlashY.bmp");

  numbers.No0 = No0;
  numbers.No1 = No1;
  numbers.No2 = No2;
  numbers.No3 = No3;
  numbers.No4 = No4;
  numbers.No5 = No5;
  numbers.No6 = No6;
  numbers.No7 = No7;
  numbers.No8 = No8;
  numbers.No9 = No9;
  numbers.NoDash = NoDash;
  numbers.NoSlash = NoSlash;
}
void deleteNumbers(){
  deleteBitmap(numbers.No0);
  deleteBitmap(numbers.No1);
  deleteBitmap(numbers.No2);
  deleteBitmap(numbers.No3);
  deleteBitmap(numbers.No4);
  deleteBitmap(numbers.No5);
  deleteBitmap(numbers.No6);
  deleteBitmap(numbers.No7);
  deleteBitmap(numbers.No8);
  deleteBitmap(numbers.No9);
  deleteBitmap(numbers.NoDash);
  deleteBitmap(numbers.NoSlash);
}

bool pointHandler(Sprite*UNUSED(ball), Sprite *aim){
  int aimx = getAimx();
  int aimy = getAimy();

  if(aimx > getHorResolution()/2 && aimx < 1010- (int)aim->width && aimy > 236 && aimy < 591 -(int)aim->height){
    score++;
    resetAim();
    return true;
  }
  else{
    setHighscores(score);
    score = 0;
    return false;
  }
}

void printPoints(){

  int units = score % 10;
  int dozens = (score/10) % 10;
  int hundreds = (score/100)%10;

  int32_t unitsX =110;
  int32_t unitsY = 42;
  int32_t dozensX =98;
  int32_t dozensY=42;
  int32_t hundredsX=86;
  int32_t hundredsY=42;

  print_numbers (hundreds, hundredsX, hundredsY);
  print_numbers (dozens, dozensX, dozensY);
  print_numbers (units, unitsX, unitsY);
}

void update_date (){

  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  time_rtc.day = get_day_rtc();
  time_rtc.month = get_month_rtc();
  time_rtc.year = get_year_rtc();
  time_rtc.hour = get_hour_rtc();
  time_rtc.minutes = get_minutes_rtc();

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  uint32_t temp;
  sys_inb(RTC_DATA_REG, &temp);


  if (!(temp & BIT(2))) {
    BCD_to_binary(&time_rtc.hour);
    BCD_to_binary(&time_rtc.minutes);
    BCD_to_binary(&time_rtc.year);
    BCD_to_binary(&time_rtc.month);
    BCD_to_binary(&time_rtc.day);
  }
  sys_outb(RTC_ADDR_REG, RTC_REG_C);

}

void print_date (){
  ////////////day////////////
  int32_t data;
  data = time_rtc.day;

  int units = data % 10;
  int dozens = (data/10) % 10;

  int32_t unitsX = 904;
  int32_t DateY = 17;
  int32_t dozensX = 892;
  int32_t TimeY = 44;


  print_numbers (dozens, dozensX, DateY);
  print_numbers (units, unitsX, DateY);

  ////////////month////////////
  data = time_rtc.month;

  units = data % 10;
  dozens = (data/10) % 10;

  unitsX = 944;
  dozensX = 932;

  print_numbers (dozens, dozensX, DateY);
  print_numbers (units, unitsX, DateY);

  ////////////year////////////
  data = time_rtc.year;

  units = data % 10;
  dozens = (data/10) % 10;

  unitsX = 986;
  dozensX = 974;

  print_numbers (dozens, dozensX, DateY);
  print_numbers (units, unitsX, DateY);

  ////////////hour////////////
  data = time_rtc.hour;

  units = data % 10;
  dozens = (data/10) % 10;

  unitsX = 926;
  dozensX = 914;

  print_numbers (dozens, dozensX,TimeY);
  print_numbers (units, unitsX, TimeY);

  ////////////minutes////////////
  data = time_rtc.minutes;

  units = data % 10;
  dozens = (data/10) % 10;

  unitsX = 960;
  dozensX = 948;

  print_numbers (dozens, dozensX, TimeY);
  print_numbers (units, unitsX, TimeY);
}

void print_numbers (int digit, int32_t pos_x, int32_t pos_y){
  switch(digit){
    case 0:
    drawBitmap(numbers.No0,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 1:
    drawBitmap(numbers.No1,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 2:
    drawBitmap(numbers.No2,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 3:
    drawBitmap(numbers.No3,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 4:
    drawBitmap(numbers.No4,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 5:
    drawBitmap(numbers.No5,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 6:
    drawBitmap(numbers.No6,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 7:
    drawBitmap(numbers.No7,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 8:
    drawBitmap(numbers.No8,pos_x,pos_y,ALIGN_LEFT);
    break;
    case 9:
    drawBitmap(numbers.No9,pos_x,pos_y,ALIGN_LEFT);
    break;
  }
}

void BCD_to_binary(uint32_t * number){
  uint32_t temp;
  temp = (*number & 0xF0) >> 4;
  *number = temp * 10 + (*number & 0x0F);

}

void highscoreScreen(){

  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  //uint32_t timer_irq_set = getTIMER_IRQ();

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

  loadHighScoreNumbers();

  Bitmap* background = loadBitmap("/home/lcom/labs/proj/bmp/HighscoreBackground.bmp");

  drawBitmap(background,0,0, ALIGN_LEFT);
  readHighscores();//Displays the highscoreScreen

  doubleBuffCall();

  while (scanByte != ESC_CODE) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
          kbc_ih();
          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);
          if (wait == false) {
            scancode_parse(scanByte, nbyte);
          }
          tickdelay(micros_to_ticks(DELAY_US));

        }

        break;
        default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
}


int compare( const void* a, const void* b)
{
  const HighscoreLine *HighscoreLine_a =  (HighscoreLine *) a;
  const HighscoreLine *HighscoreLine_b =  (HighscoreLine *) b;

  return (HighscoreLine_a->points < HighscoreLine_b->points) - (HighscoreLine_a->points > HighscoreLine_b->points);
}
void setHighscores(int score){

  FILE *ptr_file;
  ptr_file = fopen("/home/lcom/labs/proj/Highscores.txt", "r+");

  if (ptr_file== NULL){
    printf("Failed to open Highscore.txt\n" );
    return;
  } else{


    char point[5];
    char date[15];
    sprintf(date,"%x/%x/%x",(int)get_day_rtc(),(int)get_month_rtc(),(int)get_year_rtc());
    char temp[20];
    HighscoreLine h[5];
    memset(temp, 0, 15);

    for(int i =0; i<4; i++){ //reads the highscores ans stores them in the array
      if(  fgets (temp, 20, ptr_file) == NULL){
        printf("Failed to read Highscore.txt\n" );
        return;
      }else{
        h[i].points = atoi(memcpy(point,temp,4));
        memcpy(h[i].date,&temp[4],8);
        memset(temp, 0, 15);
      }
    }
    h[4].points = score;
    memcpy(h[4].date,date,8);

    qsort( h, 5, sizeof(HighscoreLine), compare ); //sorts the array (Bubble sort)

    rewind(ptr_file);

    char buf[20];

    for(int i =0; i<4; i++){
      sprintf(buf, "%03d %s\n", h[i].points,h[i].date); //inserir a data depois
      fwrite(buf, strlen(buf), 1, ptr_file);
    }

    fclose(ptr_file);
  }
}


void readHighscores(){
  FILE *ptr_file;
  ptr_file = fopen("/home/lcom/labs/proj/Highscores.txt", "r");

  if (ptr_file== NULL){
    printf("Failed to open Highscore.txt\n" );
    return;
  } else{
    char str[20];
    int scoreX[] = {705,720,735}, dateX[]= {785,800,830,845,875,890};
    int yPos[] = {322,389,459,527};
    char temp[15];

    for(int i =0; i<4; i++){ //repeat for every line
      if(  fgets (str, 20, ptr_file) == NULL){
        printf("Failed to read Highscore.txt\n" );
        return;
      }else{
        int points = atoi(memcpy(temp,str,4));
        memset(temp, 0, 15);
        int day = atoi(memcpy(temp,&str[4],2));
        memset(temp, 0, 15);
        int month = atoi(memcpy(temp,&str[7],2));
        memset(temp, 0, 15);
        int year = atoi(memcpy(temp,&str[10],2));

        int units = points % 10;
        int dozens = (points/10) % 10;
        int hundreds = (points/100)%10;
        int day1 = day % 10;
        int day2 = (day/10)%10;
        int month1 = month % 10;
        int month2 = (month/10)%10;
        int year1 = year % 10;
        int year2 = (year/10)%10;

        print_numbers (hundreds, scoreX[0], yPos[i]); //SCORE
        print_numbers (dozens, scoreX[1], yPos[i]);
        print_numbers (units, scoreX[2], yPos[i]);

        print_numbers (day2, dateX[0], yPos[i]);//DAY
        print_numbers (day1, dateX[1], yPos[i]);

        print_numbers (month2, dateX[2], yPos[i]);//MONTH
        print_numbers (month1, dateX[3], yPos[i]);

        print_numbers (year2, dateX[4], yPos[i]);//YEAR
        print_numbers (year1, dateX[5], yPos[i]);

        /*printf("%d\n",points );
        printf("%d\n",day );
        printf("%d\n",month );
        printf("%d\n",year );*/

      }

    }

  }
}
