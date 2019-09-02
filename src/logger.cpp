/*
 * MIT License
 *
 * Copyright (c) 2019 Ismael Filipe Mesquita Ribeiro - nakinx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "logger.h"

#include "logsetting.h"
#include "logbuilder.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <cstring>

Logger::Logger(LogSetting logSetting)
    : _logSetting(logSetting) {
    enableAllSeverity();
}

Logger::~Logger() {
}

bool Logger::checkActiveSeverity(SeverityLevel sl) {
    if (_logSetting.getActiveSeverity() & static_cast<int>(sl))
        return true;

    return false;
}

void Logger::setActiveSeverity(const int as) {
    _logSetting.setActiveSeverity(as);
}

void Logger::addActiveSeverity(const int as) {
    _logSetting.setActiveSeverity(_logSetting.getActiveSeverity() | as);
}

void Logger::rmActiveSeverity(const int as) {
    _logSetting.setActiveSeverity(_logSetting.getActiveSeverity() ^ as);
}

void Logger::enableAllSeverity() {
    setActiveSeverity(static_cast<int>(SeverityLevel::Debug) |
                      static_cast<int>(SeverityLevel::Fatal) |
                      static_cast<int>(SeverityLevel::Error) |
                      static_cast<int>(SeverityLevel::Warning) |
                      static_cast<int>(SeverityLevel::Info));
}

void Logger::setDebugSeverityEnable(const bool isEnable) {
    if (isEnable)
        addActiveSeverity(static_cast<int>(SeverityLevel::Debug));
    else
        rmActiveSeverity(static_cast<int>(SeverityLevel::Debug));
}

void Logger::setEnable(const bool isEnable) {
    _logSetting.setEnable(isEnable);
}

void Logger::setInfoFormat(const std::string infoFormat) {
    _logSetting.setInfo(infoFormat);
}

bool Logger::write(const SeverityLevel sl,
                   const std::string file,
                   const std::string function,
                   const int line,
                   const std::string msg) {
    if (!_logSetting.isEnable())
        return false; // Do not throw exception to avoid exit application.

    if (!checkActiveSeverity(sl))
        return false; // Do not throw exception to avoid exit application.

    std::lock_guard<std::mutex> lk(_mtxLog);

    std::fstream fs;

    fs.open((_logSetting.getPath() + _logSetting.getName()),
             std::fstream::in | std::fstream::out | std::fstream::app);

    if (fs.good() != true)
        throw LoggerException(2, "Error while opening the file.");

    fs << buildInfo(_logSetting.getInfo(), file, function, line, sl) << msg << std::endl;

    if (fs.good() != true)
        throw LoggerException(3, "Error while writing in the file.");

    fs.close();

    return true;
}

std::string Logger::buildInfo(const std::string & format,
                              const std::string & file,
                              const std::string & function,
                              const int & line,
                              const SeverityLevel & sl) {
    bool found_date = false;
    bool found_date_key = false;
    bool found_specifier = false;
    char sv[64];
    std::stringstream ssr;

    auto cur_time = std::chrono::system_clock::now();
    std::time_t cur_time_tt = std::chrono::system_clock::to_time_t(cur_time);
    std::tm * tm = std::localtime(&cur_time_tt);

    for (unsigned int i = 0; i < format.length(); i++) {
        if (format[i] == '{') {
            // Begin of date/time.
            found_date_key = true;
            continue;
        } else if (format[i] == '}') {
            // End of date/time.
            found_date = false;
            found_date_key = false;
            continue;
        } else if (format[i] == '%') {
            found_specifier = true;
            continue;
        } else if (format[i] == 'D') {
            // Date/time specifier.
            found_date = true;
            found_specifier = false;
            continue;
        } else if ((found_date == true) &&
                   (found_date_key == true) &&
                   (found_specifier == true) &&
                   (format[i] == 'q')) {
            // Milliseconds specifier.
            ssr << (cur_time.time_since_epoch().count() / 1000000) % 1000;
            found_specifier = false;
            continue;
        } else if ((found_date == true) &&
                   (found_date_key == true) &&
                   (found_specifier == true)) {
            memset(sv, 0, sizeof(sv));
            std::string tmp("%");
            tmp += format[i];
            std::strftime(sv, (sizeof(sv) - sizeof(char)), tmp.c_str(), tm);
            ssr << sv;
            found_specifier = false;
            continue;
        } else if (format[i] == 'F') {
            // File specifier.
            std::cout << "Found file specifier!\n";
            ssr << file;
            found_specifier = false;
            continue;
        } else if (format[i] == 'M') {
            // Function specifier.
            ssr << function;
            found_specifier = false;
            continue;
        } else if (format[i] == 'L') {
            // Line specifier.
            ssr << line;
            found_specifier = false;
            continue;
        }
        else if (format[i] == 'S') {
            // Severity.
            ssr << getServerityName(sl);
            found_specifier = false;
            continue;
        }
        ssr << format[i];
    }

    return ssr.str();
}

std::string Logger::getServerityName(const SeverityLevel & sl) {
    switch(sl){
        case SeverityLevel::Debug : return ("debug");
        case SeverityLevel::Fatal : return ("fatal");
        case SeverityLevel::Error : return ("error");
        case SeverityLevel::Warning : return ("warning");
        case SeverityLevel::Info : return ("info");
        case SeverityLevel::Unknown : return ("unknown");
        default : return ("unknown");
    }
}

std::string Logger::getServerityDescription(const SeverityLevel & sl) {
    switch(sl){
        case SeverityLevel::Debug : return ("Informational events most useful for developers to debug application.");
        case SeverityLevel::Fatal : return ("Severe error information that will presumably abort application.");
        case SeverityLevel::Error : return ("Information representing errors in application but application will keep running.");
        case SeverityLevel::Warning : return ("Useful when application has potentially harmful situtaions. ");
        case SeverityLevel::Info : return ("Mainly useful to represent current progress of application.");
        case SeverityLevel::Unknown : return ("Unknown level.");
        default : return ("Unknown level.");
    }
}
