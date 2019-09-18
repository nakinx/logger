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

#include "logbuilder.h"

#include "logger.h"
#include "logexception.h"

LogBuilder & LogBuilder::getInstance() {
    static LogBuilder lb;
    return lb;
}

void LogBuilder::buildLogger(const std::string & name,
                             const std::string & path) {
    buildLogger(LogSetting(name, path));
}

void LogBuilder::buildLogger(LogSetting ls) {
    if (!(ls.getName().length() > 0) || !(ls.getName().length() < 255))
        throw LoggerException(1, "Invalid logger name.");

    auto ret = _loggers.insert(std::make_pair(ls.getName(), new Logger(ls)));

    // Logger name already exist on the map?
    if (ret.second == false)
        throw LoggerException(2, "Logger name already exist.");
}

void LogBuilder::destroyLogger(const std::string & name) {
    if (_loggers.find(name) == _loggers.end())
        throw LoggerException(1, "Logger (" + name + ") doesn't exist.");

    _loggers.erase(name);
}

std::shared_ptr<Logger> LogBuilder::getLogger(const std::string & name) {
    if (_loggers.find(name) == _loggers.end())
        throw LoggerException(1, "Logger (" + name + ") doesn't exist.");

    return (_loggers[name]);
}
