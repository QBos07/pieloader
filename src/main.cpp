#include "elfloader.hpp"
#include "hhk.h"
#include "qrcodegen/qrcodegen.h"
#include "util.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>
#include <span>

std::list<void *> *dealloc_list;

int main() {
  dealloc_list = new std::list<void *>();
  atexit([]() {
    dealloc_list->unique();
    for (auto ptr : *dealloc_list)
      free(ptr);
    delete dealloc_list;
  });

  constexpr auto filename = "\\fls0\\pie.elf";

  std::printf("PIE Loader by QBos07\n");
  std::printf("Opening file %s... ", filename);
  std::FILE *file = std::fopen(filename, "rb");
  if (!file)
    error("Error!\n");
  std::printf("Done\n");
  std::printf("Getting length... ");
  std::fseek(file, 0, SEEK_END);
  size_t length = std::ftell(file);
  std::rewind(file);
  std::printf("%lu\n", length);

  std::printf("Getting memory... ");
  auto file_memory = new uint8_t[length];
  dealloc_list->push_back(file_memory);
  if (!file_memory)
    error("Error!\n");
  std::printf("Done\n");
  std::printf("Copying file... ");
  std::fread(file_memory, 1, length, file);
  std::printf("Done\n");
  std::printf("Closing file... ");
  std::fclose(file);
  std::printf("Done\n");

  auto mainPtr =
      reinterpret_cast<void (*)(void)>(load_elf(file_memory, length));
  dealloc_list->remove(file_memory);
  // Debug_WaitKey();
  delete[] file_memory;
  std::printf("Dumping memory... ");
  {
    std::string buffer;
    for (auto load : *file_loads) {
      buffer += std::to_string(reinterpret_cast<std::make_unsigned_t<std::intptr_t>>(load.second.first));
      buffer += " ";
      for (auto it : std::span(load.second.first, load.second.second)) {
        char tmp[2+1];
        std::snprintf(tmp, sizeof(tmp), "%02X", it);
        buffer += tmp;
      }
      buffer += ":";
    }
    for (auto i = 0ul; i < buffer.size(); i += 600) {
      auto sub = buffer.substr(i, 600);
      uint8_t temp_buffer[qrcodegen_BUFFER_LEN_FOR_VERSION(15)];
      uint8_t qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(15)];
      if (!qrcodegen_encodeText(sub.c_str(), temp_buffer, qrcode, qrcodegen_Ecc::qrcodegen_Ecc_LOW, qrcodegen_VERSION_MIN, 15, qrcodegen_Mask::qrcodegen_Mask_AUTO, true))
        error("Failed to generate QR code version 15 for string length %zu:\"%s\"\n", std::strlen(sub.c_str()), sub.c_str());
      std::printf("Data %zu/%zu\n", i/600+1, buffer.size()/600+1);
      std::fflush(stdout);
      volatile uint16_t *const FRAMEBUFFER = reinterpret_cast<uint16_t *>(0x8c000000);
      constexpr auto boarder = 1;
      const auto qrcode_size = qrcodegen_getSize(qrcode) + 2 * boarder;
      const auto pixel_size = 320 / (qrcode_size);
      for (auto i = 0; i < qrcode_size; i++) {
        for (auto j = 0; j < qrcode_size; j++) {
          const auto color = qrcodegen_getModule(qrcode, i-boarder, j-boarder);
          for (auto x = 0; x < pixel_size; x++) {
            for (auto y = 0; y < pixel_size; y++) {
              FRAMEBUFFER[(i * pixel_size + x) + (j * pixel_size + y) * 320] = color ? 0x0000 : 0xffff;
            }
          }
        }
      
      }
      LCD_Refresh();
      Debug_WaitKey();
    }
  }
  std::printf("Done\n");
  std::printf("Press any key to execute %#08lx... \n",
              reinterpret_cast<unsigned long>(mainPtr));
  std::fflush(stdout);
  Debug_WaitKey();
  mainPtr();
  std::printf("Done\n");
  std::fflush(stdout);
  Debug_WaitKey();
}