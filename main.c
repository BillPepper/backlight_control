#include <stdio.h>
#include <unistd.h> // for user id and file check
#include <stdlib.h> // for atoi

#define brightness_file "brightness"
#define max_brightness_file "max_brightness"

// return 1 if user is root
void warn_no_root(void) {
  __uid_t uid;

  uid = geteuid();

  if (uid != 0){
    printf("Warning, you are not root, it might not work\n"); 
  }
}

void set_backlight(int percent);

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

int file_exists(char *path) {
  int status = access(path, F_OK);
  if (status == 0){
    return 1;
  }

  return 0;

}

int main(int argc, char **argv) {
  warn_no_root();

  // 2. Check if arg, brightness in percent
  if (argc != 2){
    printf("Usage: brightness_control <level in percent>\n");
    exit(1);
  }

  int user_value = atoi(argv[1]);
  if (user_value < 1 || user_value > 100){
    printf("Error, please enter a number between 1 and 100\n");
    exit(2);
  }

  // x. Check if both files exist
  if (!file_exists(brightness_file)) {
    printf("Error, missing brightness file: %s\n", brightness_file);
    exit(3);
  } 

  if (!file_exists(max_brightness_file)){
    printf("Error, missing max_brightness file: %s\n", max_brightness_file);
    exit(4);
  }

  // 3. Load maximum brightness
  int max = get_max_brightness();
  if (max == 0) {
    printf("Error, no max brightness found in file\n");
    exit(5);
  }

  // 4. Calculate new brightness value
  int new_brightness = max / 100 * user_value;

  // 5. Save brightness to file
  FILE *pOut = fopen(brightness_file, "w");
  fprintf(pOut, "%d", new_brightness);
  fclose(pOut);

  printf("Set screen brightness to %d%% (%d)\n", user_value, new_brightness);

  return 0;
}