#include <lcom/lcf.h>
#include <stdio.h>
#include <math.h>

#include "pointSystem.h"
#include "initGame.h"
#include "spriteMovement.h"
#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "rtc_macros.h"
#include "rtc.h"
#include "mouse.h"

char path[100]; //partial path of all images
char path_temp[150];
static int32_t score = 0; //points of the player in the current game
static Numbers numbers; //struct with the bitmaps of the numbers to be drawn
static Time_RTC time_rtc; //struct with the date

void loadGameNumbers(){
  Bitmap* No0 = loadBitmap(appendPath("/No0.bmp",path_temp));
  Bitmap* No1 = loadBitmap(appendPath("/No1.bmp",path_temp));
  Bitmap* No2 = loadBitmap(appendPath("/No2.bmp",path_temp));
  Bitmap* No3 = loadBitmap(appendPath("/No3.bmp",path_temp));
  Bitmap* No4 = loadBitmap(appendPath("/No4.bmp",path_temp));
  Bitmap* No5 = loadBitmap(appendPath("/No5.bmp",path_temp));
  Bitmap* No6 = loadBitmap(appendPath("/No6.bmp",path_temp));
  Bitmap* No7 = loadBitmap(appendPath("/No7.bmp",path_temp));
  Bitmap* No8 = loadBitmap(appendPath("/No8.bmp",path_temp));
  Bitmap* No9 = loadBitmap(appendPath("/No9.bmp",path_temp));

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

  score = 0;
}
void loadHighScoreNumbers(){
  Bitmap* No0 = loadBitmap(appendPath("/No0Y.bmp",path_temp));
  Bitmap* No1 = loadBitmap(appendPath("/No1Y.bmp",path_temp));
  Bitmap* No2 = loadBitmap(appendPath("/No2Y.bmp",path_temp));
  Bitmap* No3 = loadBitmap(appendPath("/No3Y.bmp",path_temp));
  Bitmap* No4 = loadBitmap(appendPath("/No4Y.bmp",path_temp));
  Bitmap* No5 = loadBitmap(appendPath("/No5Y.bmp",path_temp));
  Bitmap* No6 = loadBitmap(appendPath("/No6Y.bmp",path_temp));
  Bitmap* No7 = loadBitmap(appendPath("/No7Y.bmp",path_temp));
  Bitmap* No8 = loadBitmap(appendPath("/No8Y.bmp",path_temp));
  Bitmap* No9 = loadBitmap(appendPath("/No9Y.bmp",path_temp));
  Bitmap* NoDash = loadBitmap(appendPath("/NoDashY.bmp",path_temp));
  Bitmap* NoSlash = loadBitmap(appendPath("/NoSlashY.bmp",path_temp));

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



bool pointHandler(Sprite *aim, int xLeft, int xRight){
  int aimx = getAimx();
  int aimy = getAimy();

  if(aimx > xLeft && aimx < xRight - (int)aim->width && aimy > 236 && aimy < 591 -(int)aim->height){
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

bool pointHandlerMulti(Sprite *ball, int player, int xLimit){

  if (player == 1){
    if(ball->x > xLimit- (int)ball->width){
     return true;
    }
    else{
      return false;
    }
  }else{
    if(ball->x < xLimit - (int)ball->width){
     return true;
    }
    else{
      return false;
    }
  }
}


void printPoints(bool isMulti, int score1, int score2){
  if(!isMulti){
    int units = score % 10;
    int dozens = (score/10) % 10;
    int hundreds = (score/100)%10;

    int32_t unitsX =110;
    int32_t unitsY = 42;
    int32_t dozensX =98;
    int32_t dozensY = 42;
    int32_t hundredsX = 86;
    int32_t hundredsY = 42;

    print_numbers (hundreds, hundredsX, hundredsY);
    print_numbers (dozens, dozensX, dozensY);
    print_numbers (units, unitsX, unitsY);
  }else {
    print_numbers (score1, 442, 28);
    print_numbers (score2, 580, 28);
    }
}

void update_date (){

  rtc_ih_asm();

  time_rtc.day = get_day_asm();
  time_rtc.month = get_month_asm();
  time_rtc.year = get_year_asm();
  time_rtc.hour = get_hour_asm();
  time_rtc.minutes = get_minutes_asm();

  uint32_t temp;
  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &temp);

  if (!(temp & BIT(2))) {
    BCD_to_binary(&time_rtc.hour);
    BCD_to_binary(&time_rtc.minutes);
    BCD_to_binary(&time_rtc.year);
    BCD_to_binary(&time_rtc.month);
    BCD_to_binary(&time_rtc.day);
  }
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
  uint32_t mouse_irq_set = getMOUSE_IRQ();

  uint8_t nbyte = 0; //scanByte's number of bytes
  bool wait = false;

  loadHighScoreNumbers();

  Bitmap* background = loadBitmap(appendPath("/HighscoreBackground.bmp",path_temp));

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
        if (msg.m_notify.interrupts & mouse_irq_set) {

          OB_cleaner(); //ignores any mouse interrupts
        }
        if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
          kbc_asm_ih();
          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);

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
  deleteNumbers();
  deleteBitmap(background);
}


int compare( const void* a, const void* b)
{
  const HighscoreLine *HighscoreLine_a =  (HighscoreLine *) a;
  const HighscoreLine *HighscoreLine_b =  (HighscoreLine *) b;

  return (HighscoreLine_a->points < HighscoreLine_b->points) - (HighscoreLine_a->points > HighscoreLine_b->points);
}

void setHighscores(int score){

  FILE *ptr_file;
  ptr_file = fopen("/home/lcom/labs/proj/src/Highscores.txt", "r+");

  if (ptr_file== NULL){
    printf("Failed to open Highscores.txt\n" );
    return;
  } else{


    char point[5];
    char date[15];
    sprintf(date,"%02x/%02x/%02x",(int)get_day_asm(),(int)get_month_asm(),(int)get_year_asm());
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
      sprintf(buf, "%03d %s\n", h[i].points,h[i].date);
      fwrite(buf, strlen(buf), 1, ptr_file);
    }

    fclose(ptr_file);
  }
}


void readHighscores(){
  FILE *ptr_file;
  ptr_file = fopen("/home/lcom/labs/proj/src/Highscores.txt", "r");

  if (ptr_file== NULL){
    printf("Failed to open Highscores.txt\n" );
    return;
  } else{
    char str[20];
    int scoreX[] = {705,720,735}, dateX[]= {785,800,830,845,875,890};
    int yPos[] = {322,389,459,527};
    char temp[15];

    for(int i =0; i<4; i++){ //repeat for every line
      if(  fgets (str, 20, ptr_file) == NULL){
        printf("Failed to read Highscores.txt\n" );
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

      }

    }

  }
}


char* appendPath (char* nameFile, char* abs_path){
  char str1[150];
  strcpy (str1, nameFile);
  strcpy (abs_path, path);
  strcat(abs_path, str1);
  return abs_path;
}

