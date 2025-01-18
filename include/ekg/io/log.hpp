#ifndef EKG_IO_LOG_HPP
#define EKG_IO_LOG_HPP

#include <iostream>
#include <sstream>

namespace ekg {
  class log {
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static bool tracked;
  public:
    static void flush() {
      if (ekg::log::buffered) {
        std::string p_log {ekg::log::buffer.str()};

        #if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_VERBOSE, "EKG", "%s", p_log.c_str());
        #else
        std::cout << p_log;
        #endif

        ekg::log::buffer = std::ostringstream {};
        ekg::log::buffered = false;
      }
    }

    static void print(std::string_view msg) {
      std::cout << "[EKG-PRINT] " << msg << std::endl;
    }

    template<typename t>
    static void trace(bool should, t trace, bool interrupt_runtime = false) {
      if (!should) {
        return;
      }

      std::cout << "[EKG-TRACE] " << trace << std::endl;
      if (interrupt_runtime) {
        std::terminate();
      }
    }

    explicit log() {
      ekg::log::buffered = true;
      ekg::log::buffer << "[EKG-INFO] ";
    }

    ~log() {
      ekg::log::buffer << '\n';
      //#define EKG_LOG_DEBUG
      #ifdef EKG_LOG_DEBUG
      ekg::log::buffered = true;
      ekg::log::flush();
      ekg_log("");
      #endif
    }

    template<typename t>
    log &operator<<(const t &value) {
      ekg::log::buffer << value;
      return *this;
    }
  };
}

#endif