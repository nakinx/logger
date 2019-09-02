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

#ifndef LOG_SETTING_
#define LOG_SETTING_

#include <string>
#include <mutex>


/**
 * Struct with log settings with informations about the log.
 */
typedef struct _LogSetting {

private:
    std::string _name; ///< Logger name and file.
    std::string _path; ///< Path where the log will be stored.
    bool _isEnable; ///< Enable or disable the logger.
    std::string _infoFormat; ///< Header with informations about the log record.
    int _activeSeverity; ///< Severitys allowed to log.

public:
    _LogSetting(const std::string name,
                const std::string path,
                const bool isEnable = true)
        : _name(name),
          _path(path),
          _isEnable(isEnable),
          _activeSeverity(0) {
    }

    void setEnable(const bool isEnable) {
        _isEnable = isEnable;
    }

    void setInfo(const std::string infoFormat) {
        _infoFormat = infoFormat;
    }

    bool isEnable() {
        return _isEnable;
    }

    void setActiveSeverity(const int activeSeverity) {
        _activeSeverity = activeSeverity;
    }

    std::string getInfo() {
        return _infoFormat;
    }

    std::string getName() {
        return _name;
    }

    std::string getPath() {
        return _path;
    }

    int getActiveSeverity() {
        return _activeSeverity;
    }
} LogSetting;

#endif // LOG_SETTING_
