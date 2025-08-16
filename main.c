#include "lsusb.h"

int main(int argc, char *argv[]) {
  options_t opts;
  usb_device_t devices[MAX_DEVICES];
  int device_count;
  int parse_result;

  parse_result = parse_arguments(argc, argv, &opts);
  if (parse_result != 0) {
    return (parse_result < 0) ? 1 : 0;
  }

  device_count = scan_usb_devices(devices, MAX_DEVICES);
  if (device_count < 0) {
    fprintf(stderr, "Error: Failed to scan USB devices\n");
    return 1;
  }

  if (device_count == 0) {
    printf("No USB devices found\n");
    return 0;
  }

  if (opts.tree) {
    print_device_tree(devices, device_count);
  } else {
    for (int i = 0; i < device_count; i++) {
      int show_device = 1;

      if (opts.vendor_id >= 0 && devices[i].vendor_id != opts.vendor_id) {
        show_device = 0;
      }

      if (opts.product_id >= 0 && devices[i].product_id != opts.product_id) {
        show_device = 0;
      }

      if (opts.bus_specified && opts.bus_num >= 0 && 
        devices[i].bus_num != opts.bus_num) {
        show_device = 0;
      }

      if (opts.device_specified && opts.dev_num >= 0 && 
        devices[i].dev_num != opts.dev_num) {
        show_device = 0;
      }

      if (show_device) {
        print_device_info(&devices[i], &opts);
      }
    }
  }

  return 0;
}
