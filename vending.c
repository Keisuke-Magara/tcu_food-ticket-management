#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<wiringPi.h>

#define BlackLeft 21 //set GPIO pin for Black Button of left side.
#define Red 20 //set GPIO pin for Red Button.
#define Yellow 16 //set GPIO pin for Yellow Button.
#define Blue 26 //set GPIO pin for Blue Button.
#define Green 19 //set GPIO pin for Green Button.
#define BlackRight 13 //set GPIO pin for Black Button of right side.
#define LED 14 //set GPIO pin for status LED.

typedef struct {
  int no, value, current_tickets, prev_tickets, set_tickets, popular;
  int tickets_move[5];
  int* next;
  char name[256];
  int ave[10], times;
} Item;

void clear (char*, int);
void sendAWS (Item*, int);

struct tm *tm=NULL;
int n=0; //number of items.

int main (void)
{
  int init_flag=0, return_value=0, count=0;
  char buffer[1024];
  clear (buffer, 1024);
  FILE *setting=NULL;
  Item *item;
  clock_t start, stop;
  if (wiringPiSetupGpio() == 0){
    init_flag++;
  }
  pinMode (BlackLeft, INPUT);
  pinMode (Red, INPUT);
  pinMode (Yellow, INPUT);
  pinMode (Blue, INPUT);
  pinMode (Green, INPUT);
  pinMode (BlackRight, INPUT);
  pinMode (LED, OUTPUT);
  digitalWrite (LED, LOW);
  setting = fopen ("./setting.txt", "r");
  if (setting != NULL) init_flag++;
  fgets(buffer, 1024, setting); //skip 1line.
  fgets(buffer,6,setting);
  fscanf(setting, "%d", &n); //get number of items.
  for (int i=0;i<4;i++){
    fgets(buffer, 1024, setting); //skip 3lines.
  }
  printf("load settings...  ");
  //=====load settings.=====
  item = (Item*)calloc(n, (int)sizeof(Item));
  if (item != NULL) init_flag++;
  while (return_value != EOF){
    clear(buffer, 1024);
    return_value = fscanf(setting, "%d", &(item[count].no)); //get item no.
    if (return_value == EOF){
      break;
    }
    return_value = fscanf(setting, "%s", &(item[count].name)); //get item name.
    return_value = fscanf(setting, "%s", buffer); //get item value.
    for (int i=0;i<10;i++){ //delete "yen".
      if (buffer[i]<'0' || buffer[i]>'9'){
	buffer[i] = '\0';
	break;
      }
    }
    item[count].value = atoi(buffer);
    return_value = fscanf(setting, "%s", buffer); //get number of ticket of the item.
    for (int i=0;i<1024;i++){ //delete "mai".
      if (buffer[i]<'0' || buffer[i]>'9'){
	buffer[i] = '\0';
	break;
      }
    }
    item[count].set_tickets = atoi(buffer);
    item[count].current_tickets = item[count].set_tickets;
    count++;
  }
  fclose(setting);
  //========================
  printf("done.\n");
  for (int i=0;i<n;i++){
    item[i].times = 0;
    init_flag++;
  }
  printf("initializing ");
  if (init_flag == 3+n){
    printf("success.\n");
  }else{
    printf("failed.\n");
  }
  digitalWrite (LED, HIGH);
  printf("===============settings===============\n");
  for (int i=0;i<n;i++){
    printf("No.%d\t%d枚\t%d円\t\"%s\"\n", item[i].no, item[i].set_tickets, item[i].value, &(item[i].name));
  }
  printf("======================================\n");
  //==========================
  start = time(NULL);
  while (1) {
    stop = time(NULL);
    if (stop - start > 60){
      digitalWrite(LED, LOW);
      get_ave(item);
      start = time();
      digitalWrite(LED, HIGH);
    }
    if (digitalRead(BlackLeft) == 1){
      item[0].current_tickets--;
      sendAWS(item,n);
    }
    if (digitalRead(Red) == 1){
      item[1].current_tickets--;
      sendAWS(item,n);
    }
    if (digitalRead(Yellow) == 1){
      item[2].current_tickets--;
      sendAWS(item,n);
    }
    if (digitalRead(Blue) == 1){
      item[3].current_tickets--;
      sendAWS(item,n);
    }
    if (digitalRead(Green) == 1){
      item[4].current_tickets--;
      sendAWS(item,n);
    }
    if (digitalRead(BlackRight) == 1){
      item[5].current_tickets--;
      sendAWS(item,n);
    }
  }
  return 1;
}

void clear (char *buf, int size)
{
  for (int i=0;i<size;i++){
    buf[i] = 0;
  }
}

void sendAWS (Item *item, int n)
{
  digitalWrite (LED, LOW);
  time_t timer = time(NULL);
  tm = localtime(&timer);
  FILE *output = NULL;
  char buffer[128];
  printf("send\n");
  output = fopen ("./tickets.info", "w");
  if (output == NULL){
    printf("NULL!");
  }
  for (int i=0;i<n;i++){
    fprintf(output, "%d %d %d %d %s %d \n", item[i].no, item[i].current_tickets, item[i].set_tickets, item[i].popular, &(item[i].name), item[i].value);
  }
  fprintf(output, "\n最終更新: ");
  strftime (buffer, 128, "%Y/%m/%d %H:%M:%S", tm);
  fprintf(output, buffer);
  fclose(output);
  system("ftp -n < ftp.txt");
  usleep(400000);
  digitalWrite (LED, HIGH);
}

void get_ave (Item *item)
{
  int ct = item[0].times;
  int dif;
  for (int i=0;i<n;i++){
    dif = item[i].current_tickets - item[i].prev_tickets
      item[i].ave[ct%10] = dif;
      item[i].popular = 0;
      for (int j=0;j<10;j++){
        item[i].popular += item[i].ave[j];
      } 
      if (ct >= 10){
        item[i].popular /= 10;
      }else{
        item[i].popular /= ct+1
      }
    item[i].times++;
  }
}