#include "lsusb.h"

int parse_hex_value(const char *path, const char *filename) {
  char full_path[MAX_PATH_LEN];
  FILE *file;
  int value = -1;

  snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
  file = fopen(full_path, "r");
  if (file) {
    if (fscanf(file, "%x", &value) != 1) {
      value = -1;
    }
    fclose(file);
  }
  return value;
}

int read_string_value(const char *path, const char *filename, char *buffer, size_t size) {
  char full_path[MAX_PATH_LEN];
  FILE *file;

  snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
  file = fopen(full_path, "r");
  if (file) {
    if (fgets(buffer, size, file)) {
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
      }
      fclose(file);
      return 0;
    }
    fclose(file);
  }
  buffer[0] = '\0';
  return -1;
}

int read_device_info(const char *device_path, usb_device_t *device) {
  char path[MAX_PATH_LEN];
  struct stat st;

  snprintf(path, sizeof(path), "%s/%s", USB_SYS_PATH, device_path);

  if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
    return -1;
  }

  memset(device, 0, sizeof(usb_device_t));

  if (sscanf(device_path, "%d-%d", &device->bus_num, &device->dev_num) != 2) {
    char *dash = strchr(device_path, '-');
    if (dash && strchr(dash + 1, '.')) {
      return -1;
    }
    if (sscanf(device_path, "usb%d", &device->bus_num) == 1) {
      device->dev_num = 1;
    } else {
      return -1;
    }
  }

  device->vendor_id = parse_hex_value(path, "idVendor");
  device->product_id = parse_hex_value(path, "idProduct");
  device->device_class = parse_hex_value(path, "bDeviceClass");
  device->device_subclass = parse_hex_value(path, "bDeviceSubClass");
  device->device_protocol = parse_hex_value(path, "bDeviceProtocol");

  read_string_value(path, "manufacturer", device->manufacturer, sizeof(device->manufacturer));
  read_string_value(path, "product", device->product, sizeof(device->product));
  read_string_value(path, "serial", device->serial, sizeof(device->serial));
  read_string_value(path, "speed", device->speed, sizeof(device->speed));
  read_string_value(path, "version", device->version, sizeof(device->version));

  return 0;
}

int scan_usb_devices(usb_device_t devices[], int max_devices) {
  DIR *dir;
  struct dirent *entry;
  int count = 0;

  dir = opendir(USB_SYS_PATH);
  if (!dir) {
    fprintf(stderr, "Error: Unable to open %s: %s\n", USB_SYS_PATH, strerror(errno));
    return -1;
  }

  while ((entry = readdir(dir)) != NULL && count < max_devices) {
    if (entry->d_name[0] == '.') {
      continue;
    }

    if (strchr(entry->d_name, ':')) {
      continue;
    }

    if (read_device_info(entry->d_name, &devices[count]) == 0) {
      count++;
    }
  }

  closedir(dir);
  return count;
}
