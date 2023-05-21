#include <stdio.h>
#include <unistd.h> // for user id and file check
#include <stdlib.h> // for atoi

#define brightness_file "brightness"
#define max_brightness_file "max_brightness"

int user_is_sudo(void);
void set_backlight(int);
int get_max_backlight(void);
int file_exists(char *path) {
  int status = access(path, F_OK);
  if (status == 0){
    return 1;
  }

  return 0;

}

int main(int argc, char **argv) {
  // 1. Check if user is admin, exit if not
  __uid_t userid = geteuid();
  if (userid != 0){
    printf("Warning, you are not root, it might not work\n");
  }

  // 2. Check if arg, brightness in percent
  if (argc != 2){
    printf("Usage: brightness_control <level in percent>\n");
    return 1;
  }

  int user_value = atoi(argv[1]);
  if (user_value < 1 || user_value > 100){
    printf("Error, please enter a number between 1 and 100\n");
    return 2;
  }

  // x. Check if both files exist
  if (!file_exists(brightness_file)) {
    printf("Error, missing brightness file: %s\n", brightness_file);
    return 3;
  } 

  if (!file_exists(max_brightness_file)){
    printf("Error, missing max_brightness file: %s\n", max_brightness_file);
    return 4;
  }

  // 3. Load maximum brightness
  FILE *pFile = fopen(max_brightness_file, "r");
  if (pFile == 0) {
    printf("Error, could not open file\n");
    return 5;
  }

  int buff;

  fscanf(pFile, "%d", &buff);
  fclose(pFile);



  // 4. Calculate new brightness value
  int new_brightness = buff / 100 * user_value;

  // 5. Save brightness to file
  FILE *pOut = fopen(brightness_file, "w");
  fprintf(pOut, "%d", new_brightness);
  fclose(pOut);

  printf("End\n");

  return 0;
}