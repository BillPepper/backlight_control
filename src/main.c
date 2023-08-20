#include <stdio.h>
#include <unistd.h> // for user id and file check
#include <stdlib.h> // for atoi

#define brightness_file "/sys/class/backlight/intel_backlight/brightness"
#define max_brightness_file "/sys/class/backlight/intel_backlight/max_brightness"

// return 1 if user is root
void warn_no_root(void) {
  __uid_t uid;

  uid = geteuid();
  if (uid != 0){
    printf("Warning, you are not root, it might not work\n"); 
  }
}

int file_exists(char *path) {
  int status;
  
  status = access(path, F_OK);
  if (status == 0){
    return 1;
  }

  return 0;
}

int file_writable(char *path) {
  int status;
  
  status = access(path, W_OK);
  if (status == 0){
    return 1;
  }

  return 0;
}

int set_backlight(int value) {
  FILE *pFile;

  pFile = fopen(brightness_file, "w");
  if (pFile == 0) {
    printf("Error, could not open file\n");
    return 0;
  }

  if (!file_writable(brightness_file)) {
    printf("Error, can't write brightness file\n");
    return 0;
  }

  fprintf(pFile, "%d", value);
  fclose(pFile);

  return 1;
};

int get_max_brightness(void) {
  FILE *pFile;
  int max_brightness;

  pFile = fopen(max_brightness_file, "r");
  if (pFile == 0) {
    printf("Error, could not open file\n");
    return 0;
  }

  fscanf(pFile, "%d", &max_brightness);
  fclose(pFile);

  return max_brightness;
};

int main(int argc, char **argv) {
  warn_no_root();

  int percent;
  int max;
  int new_brightness;
  int status;

  if (argc != 2){
    printf("Usage: brightness_control <level in percent>\n");
    exit(1);
  }

  percent = atoi(argv[1]);
  if (percent < 1 || percent > 100){
    printf("Error, please enter a number between 1 and 100\n");
    exit(2);
  }

  if (!file_exists(brightness_file)) {
    printf("Error, missing brightness file: %s\n", brightness_file);
    exit(3);
  } 

  if (!file_exists(max_brightness_file)){
    printf("Error, missing max_brightness file: %s\n", max_brightness_file);
    exit(4);
  }

  max = get_max_brightness();
  if (max == 0) {
    printf("Error, no max brightness found in file\n");
    exit(5);
  }

  new_brightness = max / 100 * percent;
  status = set_backlight(new_brightness);
  if (status == 0) {
    printf("Error, could not set backlight\n");
    exit(6);
  }

  printf("Set screen brightness to %d%% (%d)\n", percent, new_brightness);

  return 0;
}
