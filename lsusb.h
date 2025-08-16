#ifndef LSUSB_H
#define LSUSB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#define USB_SYS_PATH "/sys/bus/usb/devices"
#define MAX_PATH_LEN 256
#define MAX_STRING_LEN 128
#define MAX_DEVICES 256

typedef struct {
  int bus_num;
  int dev_num;
  int vendor_id;
  int product_id;
  int device_class;
  int device_subclass;
  int device_protocol;
  char manufacturer[MAX_STRING_LEN];
  char product[MAX_STRING_LEN];
  char serial[MAX_STRING_LEN];
  char speed[16];
  char version[16];
} usb_device_t;

typedef struct {
  int verbose;
  int tree;
  int device_specified;
  int bus_specified;
  int vendor_id;
  int product_id;
  int bus_num;
  int dev_num;
} options_t;

int scan_usb_devices(usb_device_t devices[], int max_devices);
int read_device_info(const char *device_path, usb_device_t *device);
void print_device_info(const usb_device_t *device, const options_t *opts);
void print_device_tree(usb_device_t devices[], int count);
int parse_hex_value(const char *path, const char *filename);
int read_string_value(const char *path, const char *filename, char *buffer, size_t size);
void print_usage(const char *program_name);
int parse_arguments(int argc, char *argv[], options_t *opts);

#endif
