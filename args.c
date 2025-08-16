#include "lsusb.h"

void print_usage(const char *program_name) {
  printf("Usage: %s [options]\n", program_name);
  printf("List USB devices\n\n");
  printf("Options:\n");
  printf("  -v, --verbose           Show detailed device information\n");
  printf("  -t, --tree              Show device tree\n");
  printf("  -s [[bus]:][devnum]     Show only devices with specified bus and/or device number\n");
  printf("  -d [vendor]:[product]   Show only devices with specified vendor and product ID\n");
  printf("  -h, --help              Show this help message\n");
  printf("  -V, --version           Show version information\n\n");
  printf("Examples:\n");
  printf("  %s                      List all USB devices\n", program_name);
  printf("  %s -v                   List all devices with detailed info\n", program_name);
  printf("  %s -t                   Show device tree\n", program_name);
  printf("  %s -s 001:002           Show device 002 on bus 001\n", program_name);
  printf("  %s -d 1d6b:0002         Show devices with vendor:product 1d6b:0002\n", program_name);
}

int parse_bus_dev(const char *str, int *bus, int *dev) {
  char *colon = strchr(str, ':');

  if (colon) {
    *colon = '\0';
    if (strlen(str) > 0) {
      *bus = (int)strtol(str, NULL, 10);
    }
    if (strlen(colon + 1) > 0) {
      *dev = (int)strtol(colon + 1, NULL, 10);
    }
    *colon = ':';
  } else {
    if (strlen(str) > 0) {
      *dev = (int)strtol(str, NULL, 10);
    }
  }

  return 0;
}

int parse_vendor_product(const char *str, int *vendor, int *product) {
  char *colon = strchr(str, ':');

  if (colon) {
    *colon = '\0';
    if (strlen(str) > 0) {
      *vendor = (int)strtol(str, NULL, 16);
    }
    if (strlen(colon + 1) > 0) {
      *product = (int)strtol(colon + 1, NULL, 16);
    }
    *colon = ':';
  } else {
    if (strlen(str) > 0) {
      *vendor = (int)strtol(str, NULL, 16);
    }
  }

  return 0;
}

int parse_arguments(int argc, char *argv[], options_t *opts) {
  int opt;
  static struct option long_options[] = {
    {"verbose", no_argument, 0, 'v'},
    {"tree", no_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {0, 0, 0, 0}
  };

  memset(opts, 0, sizeof(options_t));
  opts->vendor_id = -1;
  opts->product_id = -1;
  opts->bus_num = -1;
  opts->dev_num = -1;

  while ((opt = getopt_long(argc, argv, "vts:d:hV", long_options, NULL)) != -1) {
    switch (opt) {
      case 'v':
        opts->verbose = 1;
        break;

      case 't':
        opts->tree = 1;
        break;

      case 's':
        opts->device_specified = 1;
        parse_bus_dev(optarg, &opts->bus_num, &opts->dev_num);
        break;

      case 'd':
        parse_vendor_product(optarg, &opts->vendor_id, &opts->product_id);
        break;

      case 'h':
        print_usage(argv[0]);
        return 1;

      case 'V':
        printf("lsusb version 1.0\n");
        printf("USB device listing utility\n");
        return 1;

      case '?':
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return -1;

      default:
        return -1;
    }
  }

  return 0;
}
