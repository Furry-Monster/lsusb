# lsusb - USB设备列表工具

一个用C语言编写的轻量级USB设备列表工具，类似于Linux系统自带的lsusb命令。

## 功能特性

- 枚举系统中的所有USB设备
- 显示设备的总线号、设备号、厂商ID和产品ID
- 支持详细信息显示模式
- 支持设备树形显示
- 支持按总线号、设备号过滤
- 支持按厂商ID、产品ID过滤
- 完整的命令行参数支持

## 编译

### 系统要求
- Linux操作系统（需要`/sys/bus/usb/devices`目录）
- GCC编译器
- Make工具

### 编译步骤
```bash
make
```

### 其他编译选项
```bash
make debug     # 调试版本
make static    # 静态链接版本
make clean     # 清理编译文件
```

## 使用方法

### 基本用法
```bash
./lsusb                    # 列出所有USB设备
./lsusb -v                 # 显示详细信息
./lsusb -t                 # 显示设备树
./lsusb -h                 # 显示帮助信息
./lsusb -V                 # 显示版本信息
```

### 设备过滤
```bash
./lsusb -s 001:002         # 显示总线001上的设备002
./lsusb -s 001:            # 显示总线001上的所有设备
./lsusb -s :002            # 显示所有总线上的设备002
./lsusb -d 1d6b:0002       # 显示厂商ID为1d6b，产品ID为0002的设备
./lsusb -d 1d6b:           # 显示厂商ID为1d6b的所有设备
```

## 命令行选项

| 选项 | 长选项 | 描述 |
|------|--------|------|
| `-v` | `--verbose` | 显示详细的设备信息 |
| `-t` | `--tree` | 以树形结构显示设备 |
| `-s` | | 按总线号和设备号过滤 `[[bus]:][devnum]` |
| `-d` | | 按厂商ID和产品ID过滤 `[vendor]:[product]` |
| `-h` | `--help` | 显示帮助信息 |
| `-V` | `--version` | 显示版本信息 |

## 输出格式

### 基本格式
```
Bus 001 Device 002: ID 1d6b:0002 Linux Foundation 2.0 root hub
```

### 详细格式（-v选项）
```
Bus 001 Device 002: ID 1d6b:0002 Linux Foundation 2.0 root hub
  Device Class: 09 (Hub)
  Device Subclass: 00
  Device Protocol: 01
  Speed: 480
  USB Version: 2.00
  Serial Number: 0000:00:14.0
```

### 树形格式（-t选项）
```
/:  Bus 01.Port 1: Dev 1, Class=root_hub, Driver=hub
    |__ Port 2: Dev 2, If 0, Class=Hub, Driver=hub
```

## 文件结构

```
lsusb/
├── README.md           # 项目说明文档
├── Makefile           # 编译配置文件
├── lsusb.h            # 主头文件
├── main.c             # 主程序入口
├── usb_scan.c         # USB设备扫描功能
├── usb_display.c      # 设备信息显示功能
└── args.c             # 命令行参数处理
```

## 技术实现

- **设备发现**: 通过读取`/sys/bus/usb/devices`目录获取USB设备信息
- **信息解析**: 解析sysfs中的设备属性文件（如idVendor、idProduct等）
- **参数处理**: 使用`getopt_long`处理命令行参数
- **内存管理**: 静态数组管理设备列表，避免动态内存分配

## 注意事项

1. 程序需要读取`/sys/bus/usb/devices`目录，通常需要适当的权限
2. 某些设备信息可能因权限不足而无法读取
3. 程序设计为单次执行，不支持实时监控

## 安装

```bash
make install           # 安装到/usr/local/bin/
make uninstall         # 卸载
```

## 测试

```bash
make test              # 运行基本功能测试
```

## 许可证

MIT License

## 作者

FurryMonster
