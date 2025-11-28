#include <source_location>
#include <cstdint>
#include <numeric>
#include <string>
#include <cstring>
#include <format>
#include <algorithm>
#include <iostream>


namespace vobj {
  class LocInfo {
    const uint64_t INF = std::numeric_limits<uint64_t>::max();

  public:
  
    const char *file;
    uint64_t line = INF;
    uint64_t offset = INF;

    LocInfo() {}
    LocInfo(std::source_location sloc) : line(sloc.line()), offset(sloc.column()) {
      // find basename of file
      file = sloc.file_name();
      const char *lastFSlash = std::strrchr(file, '/');
      const char *lastBSlash = std::strrchr(file, '\\');
      const char *end = file + std::strlen(file);
      lastFSlash = lastFSlash ? lastFSlash + 1 : file;
      lastBSlash = lastBSlash ? lastBSlash + 1 : file;
      file = std::max(lastFSlash, lastBSlash);
      std::cout << file << std::endl;
    }
    LocInfo(const LocInfo &loc) : file(loc.file), line(loc.line), offset(loc.offset) {}
    LocInfo(LocInfo &&loc) : LocInfo(loc) {}

    LocInfo &operator=(const LocInfo &other) {
      file = other.file;
      line = other.line;
      offset = other.offset;
      return *this;
    }
    LocInfo &operator=(LocInfo &&other) {
      return *this = other;
    }

    bool operator==(LocInfo other) {
      return line == other.line && offset == other.offset && std::strcmp(file, other.file) == 0;
    }

    std::string pretty() {
      return std::format("{}:{}:{}", file, line == INF ? "?" : std::to_string(line), offset == INF ? "?" : std::to_string(offset));
    }

    std::string prettyCombine(LocInfo other) {
      if (*this == other) return pretty();

      std::string a = pretty();
      std::string b = other.pretty();
      size_t ia = a.find(':');
      size_t ib = b.find(':');
      size_t pia = SIZE_MAX;
      size_t pib = SIZE_MAX;
      while (ia != pia && a.substr(0, ia) == b.substr(0, ib)) {
        pia = ia;
        pib = ib;
        ia = std::distance(a.begin(), std::find(a.begin() + ia, a.end(), ':'));
        ib = std::distance(b.begin(), std::find(b.begin() + ib, b.end(), ':'));
      }
      
      return a.substr(0, pia+1) + a.substr(pia+1) + "-" + b.substr(pib+1); 
    }
  };
}