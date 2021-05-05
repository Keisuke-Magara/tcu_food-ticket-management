#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<wiringPi.h>

#define BlackLeft 21
#define Red 20
#define Yellow 16
#define Blue 26
#define Green 19
#define BlackRight 13

typedef struct {
  int no, value, current_tickets, set_tickets, popular;
  char name[256];
} Item;

void clear (char*, int);
void sendAWS (Item*, int);

struct tm *tm=NULL;

int main (void)
{
  int init_flag=0, return_value=0, n=0, count=0; //number of items .
  char buffer[1024];
  clear (buffer, 1024);
  FILE *setting=NULL, *temp=NULL;
  Item *item;
  clock_t start, stop;
  if (wiringPiSetupGpio() == 0){
    init_flag++;
  }
  setting = fopen ("./setting.txt", "r");
  if (setting != NULL) init_flag++;
  fgets(buffer, 1024, setting); //skip 1line.
  fgets(buffer,6,setting);
  fscanf(setting, "%d", &n);
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
  printf("initializing ");
  if (init_flag == 3){
    printf("success.\n");
  }else{
    printf("failed.\n");
  }
  printf("===============settings===============\n");
  for (int i=0;i<n;i++){
    printf("%d\t%d枚\t%d円\t\"%s\"\n", item[i].no, item[i].set_tickets, item[i].value, &(item[i].name));
  }
  printf("======================================\n");
  //==========================
  while (1) {
    if (digitalRead(BlackLeft) == 1){
      item[0].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
    }
    if (digitalRead(Red) == 1){
      item[1].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
    }
    if (digitalRead(Yellow) == 1){
      item[2].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
    }
    if (digitalRead(Blue) == 1){
      item[3].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
    }
    if (digitalRead(Green) == 1){
      item[4].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
    }
    if (digitalRead(BlackRight) == 1){
      item[5].current_tickets--;
      sendAWS(item,n);
      usleep(400000);
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
    fprintf(output, "%d %d %d %d %s %d\n", item[i].no, item[i].current_tickets, item[i].set_tickets, item[i].popular, &(item[i].name), item[i].value);
  }
  fprintf(output, "\ndate ");
  strftime (buffer, 128, "%Y %m %d %H %M %S", tm);
  fprintf(output, buffer);
  fclose(output);
  system("ftp -n < ftp.txt");
}
