/**************************************************************************/
/* main.cpp                                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                                Knoodle                                 */
/*                        https://knoodlegraph.org                        */
/**************************************************************************/
/* Copyright (c) 2025 Knoodle contributors (vide AUTHORS.md)              */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include <cstdint>
#include <filesystem>
#include <set>
#include "log/log.hpp"
#include "string_utils.hpp"

namespace kn {
extern bool compile();
}

using namespace kn;

void print_help() {
  KN_LOG(LogShaderCompiler, Info, "Usage: shader_compiler [options] <input_file>");
  KN_LOG(LogShaderCompiler, Info, "Options:");
  KN_LOG(LogShaderCompiler, Info, "  -h, --help, ?  Display this help message");
}

int32_t main(int32_t argc, char** argv) {
#ifdef KN_WITH_LOG
  auto& Logger = Log::getInstance();

  auto& StdSink = Logger.addSink<Log::StdoutSink>();
  StdSink.ShowTimestamp = false;
  StdSink.ShowCategory = false;
  StdSink.ShowVerbosity = false;
  StdSink.ShowFile = false;
#endif

  constexpr auto noInputError = []() {
    KN_LOG(LogShaderCompiler, Error, "No input file provided");
    print_help();
  };

  if (argc < 2) {
    noInputError();
    return 1;
  }

  if (argc == 2) {
    if (strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0 || argv[1][0] == '?') {
      print_help();
      return 0;
    }
  }

  std::set<std::filesystem::path> InputFiles;

  for (int32_t i = 1; i < argc; ++i) {
    char* Arg = argv[i];
#ifdef KN_WITH_LOG
    if (strcmp("--log", Arg) == 0) {
      std::filesystem::path LogPath = "shader-compiler.log";
      if ((i + 1) < argc && argv[i + 1][0] != '-') {
        LogPath = argv[i + 1];
        ++i;
      }
      auto& FileSink = Logger.addSink<Log::FileSink>(LogPath);
      StdSink.ShowFile = false;
      StdSink.ShowTimestamp = false;
    }
#endif

    if (std::filesystem::is_regular_file(Arg)) {
      KN_LOG(LogShaderCompiler, Verbose, "Adding input file: {}", Arg);
      InputFiles.insert(Arg);
    }
  }

  if (InputFiles.empty()) {
    noInputError();
    return 1;
  }

  return 0;
}
