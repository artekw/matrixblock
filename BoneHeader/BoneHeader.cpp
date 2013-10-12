#include "BoneHeader.h"

/****************************************************************
 * export_gpio
 ****************************************************************/

int export_gpio(int gpio){
    FILE *fp;

    //open the export file
    if((fp = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Cannot open export file. \n");
        return 1;
    }

    //write specified gpio to export file
    fprintf(fp, "%d\n", gpio);
    fflush(fp);
    fclose(fp);

    //return 0 if everything runs correctly
    return 0;
}

/****************************************************************
 * is_exported_gpio
 ****************************************************************/

int is_exported_gpio(int gpio){

    char s[200];
    sprintf(s, "test -e /sys/class/gpio/gpio%d/value", gpio);
    if (system(s) == 0){
        return 1;
    }else{
        return 0;
    }
}
/****************************************************************
 * unexport_gpio
 ****************************************************************/

int unexport_gpio(int gpio){
    FILE *fp;

    //open the unexport file
    if((fp = fopen("/sys/class/gpio/unexport", "ab")) == NULL){
        printf("Cannot open unexport file. \n");
        return 1;
    }

    //write specified gpio to unexport file
    fprintf(fp, "%d\n", gpio);
    fflush(fp);
    fclose(fp);

    return 0;
}

/****************************************************************
 * set_gpio_direction
 ****************************************************************/

int set_gpio_direction(int gpio, const char* direction){
    FILE *fp;
    char path[MAX_BUF];

    //create path using specified gpio
    snprintf(path, sizeof path,"/sys/class/gpio/gpio%d/direction", gpio);
    //open direction file
    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open specified direction file. Is gpio%d exported?\n", gpio);
        return 1;
    }

    //write "in" or "out" to direction file
    rewind(fp);
    fprintf(fp, "%s\n", direction);
    fflush(fp);
    fclose(fp);
}

/****************************************************************
 * set_gpio_value
 ****************************************************************/

int set_gpio_value(int gpio, int value){
    FILE *fp;
    char path[MAX_BUF];

    //set value only if direction is out
    snprintf(path, sizeof path, "/sys/class/gpio/gpio%d/value", gpio);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open specified value file.\n", gpio);
        return 1;
    }

    //write 1 or 0 to value file
    rewind(fp);
    fprintf(fp, "%d\n", value);
    fflush(fp);
    fclose(fp);
}

/****************************************************************
 * set_gpio_edge
 ****************************************************************/

int set_gpio_edge(int gpio, char* edge){
    FILE *fp;
    char path[MAX_BUF];

    //create path using specified gpio
    snprintf(path, sizeof path, "/sys/class/gpio/gpio%d/edge", gpio);
    //open edge file
    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open specified edge file. Is gpio%d exported?\n", gpio);
        return 1;
    }

    //write "rising", "falling", or "both" to edge file
    rewind(fp);
    fprintf(fp, "%s\n", edge);
    fflush(fp);
    fclose(fp);

}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY | O_NONBLOCK );
    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
    return close(fd);
}

/****************************************************************
 * set_mux_value
 ****************************************************************/
int set_mux_value(char* mux, int value){
    FILE *fp;
    char path[MAX_BUF];

    snprintf(path, sizeof path, "/sys/kernel/debug/omap_mux/%s", mux);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open specified mux, %s\n", mux);
        return 1;
    }

    rewind(fp);
    fprintf(fp, "%d\n", value);
    fflush(fp);
    fclose(fp);

}

/****************************************************************
 * read_ain
 ****************************************************************/
int read_ain(char* ain){
    FILE *fp;
    char path[MAX_BUF];
    char buf[MAX_BUF];

    snprintf(path, sizeof path, "/sys/devices/platform/omap/tsc/%s", ain);

    if((fp = fopen(path, "r")) == NULL){
        printf("Cannot open specified ain pin, %s\n", ain);
        return 1;
    }

    if(fgets(buf, MAX_BUF, fp) == NULL){
        printf("Cannot read specified ain pin, %s\n", ain);
    }

    fclose(fp);
    return atoi(buf);
}

/****************************************************************
 * set_pwm
 ****************************************************************/
int set_pwm(char* pwm, int period_freq, int duty_percent){
    FILE *fp;
    char path[MAX_BUF];

    snprintf(path, sizeof path, "/sys/class/pwm/%s/run", pwm);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open pwm run file, %s\n", path);
        return 1;
    }

    rewind(fp);
    fprintf(fp, "1\n");
    fflush(fp);
    fclose(fp);

    snprintf(path, sizeof path, "/sys/class/pwm/%s/duty_ns", pwm);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open pwm duty_ns file, %s\n", path);
    }

    rewind(fp);
    fprintf(fp, "0\n");
    fflush(fp);
    fclose(fp);

    snprintf(path, sizeof path, "/sys/class/pwm/%s/period_freq", pwm);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open pwm period_freq file, %s\n", path);
    }

    rewind(fp);
    fprintf(fp, "%d\n", period_freq);
    fflush(fp);
    fclose(fp);

    snprintf(path, sizeof path, "/sys/class/pwm/%s/duty_percent", pwm);

    if((fp = fopen(path, "w")) == NULL){
        printf("Cannot open duty_percent file, %s\n", path);
    }

    rewind(fp);
    fprintf(fp, "%d\n", duty_percent);
    fflush(fp);
    fclose(fp);
}

/****************************************************************
 * unset_pwm
 ****************************************************************/
int unset_pwm(char* pwm){
    FILE *fp;
    char path[MAX_BUF];

    snprintf(path, sizeof path, "/sys/class/pwm/%s/run", pwm);

    if((fp = fopen(path, "w")) == NULL) {
        printf("Cannot open pwm run file, %s\n", path);
        return 1;
    }

    rewind(fp);
    fprintf(fp, "0\n");
    fflush(fp);
    fclose(fp);

    return 0;

}