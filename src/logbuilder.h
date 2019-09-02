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

#ifndef LOG_BUILDER_
#define LOG_BUILDER_

#include "logsetting.h"

#include <iostream>
#include <memory>
#include <map>

class Logger;

/**
 * Singleton class responsable to create, store and manager the loggers
 * instances.
 */
class LogBuilder {

public:
    /**
     * Get instance of logger builder allowing to create, store and manager
     * the loggers instances.
     *
     * @return Returns log builder instance.
     */
    static LogBuilder & getInstance();

    /**
     * Build a logger instance based on settings previous defined.
     *
     * @param ls Logger settings.
     *
     * @throws LoggerException
     *         Invalid logger name.
     *         Logger name already exist.
     */
    void buildLogger(LogSetting ls);

    /**
     * Build a logger instance.
     *
     * @param name Logger name.
     * @param name Path where logs will be saved.
     *
     * @throws LoggerException
     *         Invalid logger name.
     *         Logger name already exist.
     */
    void buildLogger(const std::string name,
                     const std::string path);

    /**
     * Destroy logger instance.
     *
     * @param name Logger name.
     *
     * @throws LoggerException
     *         Logger name doesn't exist.
     */
    void destroyLogger(const std::string name);

    /**
     * Returns the logger instance based on the provide name.
     *
     * @param name Logger name.
     *
     * @return Returns logger instance.
     *
     * @throws LoggerException
     *         Logger name doesn't exist.
     */
    std::shared_ptr<Logger> getLogger(const std::string name);

private:
    /**
     * Implementation as private to build a singleton class.
     */
    LogBuilder() {}

    /**
     * Implementation as private to build a singleton class.
     */
    LogBuilder(LogBuilder const &) {}

    /**
     * Implementation as private to build a singleton class.
     */
    void operator=(LogBuilder const &) {}

    std::map<std::string, std::shared_ptr<Logger>> _loggers; ///< Map with all loggers instances.

};

#endif // LOG_BUILDER_
