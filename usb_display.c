#include "lsusb.h"

const char* get_class_name(int class_code) {
  switch (class_code) {
    case 0x00: return "Per Interface";
    case 0x01: return "Audio";
    case 0x02: return "Communications";
    case 0x03: return "Human Interface Device";
    case 0x05: return "Physical";
    case 0x06: return "Image";
    case 0x07: return "Printer";
    case 0x08: return "Mass Storage";
    case 0x09: return "Hub";
    case 0x0a: return "CDC Data";
    case 0x0b: return "Smart Card";
    case 0x0d: return "Content Security";
    case 0x0e: return "Video";
    case 0x0f: return "Personal Healthcare";
    case 0x10: return "Audio/Video";
    case 0x11: return "Billboard";
    case 0xdc: return "Diagnostic";
    case 0xe0: return "Wireless";
    case 0xef: return "Miscellaneous";
    case 0xfe: return "Application Specific";
    case 0xff: return "Vendor Specific";
    default: return "Unknown";
  }
}

void print_device_info(const usb_device_t *device, const options_t *opts) {
  if (!device) return;

  if (opts->device_specified) {
    if (device->bus_num != opts->bus_num || device->dev_num != opts->dev_num) {
      return;
    }
  }

  if (opts->verbose) {
    printf("\nBus %03d Device %03d: ID %04x:%04x", 
           device->bus_num, device->dev_num, 
           device->vendor_id, device->product_id);

    if (strlen(device->manufacturer) > 0 || strlen(device->product) > 0) {
      printf(" %s %s", device->manufacturer, device->product);
    }
    printf("\n");

    if (device->device_class >= 0) {
      printf("  Device Class: %02x (%s)\n", 
             device->device_class, get_class_name(device->device_class));
    }

    if (device->device_subclass >= 0) {
      printf("  Device Subclass: %02x\n", device->device_subclass);
    }

    if (device->device_protocol >= 0) {
      printf("  Device Protocol: %02x\n", device->device_protocol);
    }

    if (strlen(device->speed) > 0) {
      printf("  Speed: %s\n", device->speed);
    }

    if (strlen(device->version) > 0) {
      printf("  USB Version: %s\n", device->version);
    }

    if (strlen(device->serial) > 0) {
      printf("  Serial Number: %s\n", device->serial);
    }

  } else {
    printf("Bus %03d Device %03d: ID %04x:%04x", 
           device->bus_num, device->dev_num, 
           device->vendor_id, device->product_id);

    if (strlen(device->manufacturer) > 0 || strlen(device->product) > 0) {
      printf(" %s %s", device->manufacturer, device->product);
    }
    printf("\n");
  }
}

static int compare_devices(const void *a, const void *b) {
  const usb_device_t *dev_a = (const usb_device_t *)a;
  const usb_device_t *dev_b = (const usb_device_t *)b;

  if (dev_a->bus_num != dev_b->bus_num) {
    return dev_a->bus_num - dev_b->bus_num;
  }
  return dev_a->dev_num - dev_b->dev_num;
}

void print_device_tree(usb_device_t devices[], int count) {
  int current_bus = -1;

  qsort(devices, count, sizeof(usb_device_t), compare_devices);

  for (int i = 0; i < count; i++) {
    if (devices[i].bus_num != current_bus) {
      current_bus = devices[i].bus_num;
      printf("/:  Bus %02d.Port 1: Dev 1, Class=root_hub, Driver=hub\n", 
             current_bus);
    }

    if (devices[i].dev_num > 1) {
      printf("    |__ Port %d: Dev %d, If 0, Class=%s, Driver=%s\n",
             devices[i].dev_num, devices[i].dev_num,
             get_class_name(devices[i].device_class),
             (devices[i].device_class == 0x09) ? "hub" : "usb");
    }
  }
}
