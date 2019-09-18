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

#ifndef LOG_H_
#define LOG_H_

#include "logbuilder.h"
#include "logsetting.h"
#include "logexception.h"

#include <string>
#include <sstream>

#define LOG(severity, name, msg) LogBuilder::getInstance().getLogger(name)->write(severity, __FILE__, __PRETTY_FUNCTION__, __LINE__, msg)

#define LOG_DEBUG(name, msg) { \
    std::stringstream ss; \
    ss << msg; \
    LOG(SeverityLevel::Debug, name, ss.str()); \
}

#define LOG_FATAL(name, msg) { \
    std::stringstream ss; \
    ss << msg; \
    LOG(SeverityLevel::Fatal, name, ss.str()); \
}

#define LOG_ERROR(name, msg) { \
    std::stringstream ss; \
    ss << msg; \
    LOG(SeverityLevel::Error, name, ss.str()); \
}

#define LOG_WARNING(name, msg) { \
    std::stringstream ss; \
    ss << msg; \
    LOG(SeverityLevel::Warning, name, ss.str()); \
}

#define LOG_INFO(name, msg) { \
    std::stringstream ss; \
    ss << msg; \
    LOG(SeverityLevel::Info, name, ss.str()); \
}

/**
 * All types of severity level available to classify the log record.
 */
enum class SeverityLevel {
    Debug = 0x01, ///< Informational events most useful for developers to debug application.
    Fatal = 0x02, ///< Severe error information that will presumably abort application.
    Error = 0x04, ///< Information representing errors in application but application will keep running.
    Warning = 0x08, ///< Useful when application has potentially harmful situtaions.
    Info = 0x16, ///< Mainly useful to represent current progress of application.
    Unknown = 0x64 ///< Represents unknown level.
};

/**
 * This class is reponsible to control flow to the log file based on the
 * settings previously defined.
 */
class Logger {

public:
    /**
     * Constructor
     *
     * @param logSetting Log settings.
     */
    Logger(const LogSetting & logSetting);

    /**
     * Destructor.
     */
    ~Logger();

    /**
     * Check if the given severity is enable to log.
     *
     * @param sl Severiy to be checked.
     *
     * @return True if severity is enabled to log and false otherwise.
     */
    bool checkActiveSeverity(const SeverityLevel & sl);

    /**
     * Enable/Disable the functionality to log.
     *
     * @param isEnable True to enable and false to disable.
     */
    void setEnable(const bool & isEnable);

    /**
     * Set which severity will be enable to log.
     *
     * @param as Bitwise argument to represent active severity.
     *           Example of value: Serverity::Debug & Severity::Info are enable.
     */
    void setActiveSeverity(const int & as);

    /**
     * Add severitys to be log.
     *
     * @param as Bitwise argument to represent active severity.
     *           Example of value: Serverity::Debug & Severity::Info are enable.
     */
    void addActiveSeverity(const int & as);

    /**
     * Remove severitys to be log.
     *
     * @param as Bitwise argument to represent active severity.
     *           Example of value: Serverity::Debug & Severity::Info are enable.
     */
    void rmActiveSeverity(const int & as);

    /**
     * Enable severitys to be log.
     */
    void enableAllSeverity();

    /**
     * Enable debug severity to be log.
     *
     * @param isEnable True if is enable and false otherwise.
     *
     */
    void setDebugSeverityEnable(const bool & isEnable);

    /**
     * Set specifiers where error has ocurred in the log record, with
     * optional information like date/time, file, function, line and severity.
     *
     * @param infoFormat String containing specifiers with log informations.
     *
     *     %D{<format>}  The date format is based on strftime().
     *                   %a – Abbreviated weekday name
     *                   %A – Full weekday name
     *                   %b – Abbreviated month name
     *                   %B – Full month name
     *                   %c – Standard date and time string
     *                   %d – Day of month as a decimal(1-31)
     *                   %H – Hour(0-23)
     *                   %I – Hour(1-12)
     *                   %j – Day of year as a decimal(1-366)
     *                   %m – Month as decimal(1-12)
     *                   %M – Minute as decimal(0-59)
     *                   %p – Locale's equivalent of AM or PM
     *                   %q – Milliseconds as decimal(0-999) – Special for the application.
     *                   %S – Second as decimal(0-59)
     *                   %U – Week of year, Sunday being first day(0-53)
     *                   %w – Weekday as a decimal(0-6, Sunday being 0)
     *                   %W – Week of year, Monday being first day(0-53)
     *                   %x – Standard date string
     *                   %X – Standard time string
     *                   %y – Year in decimal without century(0-99)
     *                   %Y – Year including century as decimal
     *                   %Z – Time zone name
     *
     *     %F            File where method was invoked.
     *     %M            Method where method was invoked.
     *     %L            Line where method was invoked.
     *     %S            Log severity.
     */
    void setInfoFormat(const std::string & infoFormat);

    /**
     * Write the log record based on the settings used to build the logger.
     *
     * @param sl Severity of the log record.
     * @param file File where log was invoked.
     * @param function Function where log was invoked.
     * @line Line where log was invoked.
     * @msg Log message to be recorded.
     *
     * @return True if everything is ok and false otherwise.
     *
     * @throws LoggerException
     *         Error while opening the file.
     *         Error while writing in the file.
     *
     */
    bool write(const SeverityLevel sl,
               const std::string file,
               const std::string function,
               const int line,
               const std::string msg);

    /**
     * Based on severity code it's returns the severity name.
     *
     * @param sl Severity code.
     *
     * @return Name of severity code.
     */
    std::string getServerityName(const SeverityLevel & sl);

    /**
     * Based on severity code it's returns the details about the severity.
     *
     * @param sl Severity code.
     *
     * @return Details of severity code.
     */
    std::string getServerityDescription(const SeverityLevel & sl);

private:

    /**
     * Build the header based on specifiers where error has ocurred in the log
     * record, with optional information like date/time, file, function, line
     * and severity.
     *
     * @param format String containing specifiers with log informations.
     *
     *     %D{<format>}  The date format is based on strftime().
     *                   %a – Abbreviated weekday name
     *                   %A – Full weekday name
     *                   %b – Abbreviated month name
     *                   %B – Full month name
     *                   %c – Standard date and time string
     *                   %d – Day of month as a decimal(1-31)
     *                   %H – Hour(0-23)
     *                   %I – Hour(1-12)
     *                   %j – Day of year as a decimal(1-366)
     *                   %m – Month as decimal(1-12)
     *                   %M – Minute as decimal(0-59)
     *                   %p – Locale's equivalent of AM or PM
     *                   %q – Milliseconds as decimal(0-999) – Special for the application.
     *                   %S – Second as decimal(0-59)
     *                   %U – Week of year, Sunday being first day(0-53)
     *                   %w – Weekday as a decimal(0-6, Sunday being 0)
     *                   %W – Week of year, Monday being first day(0-53)
     *                   %x – Standard date string
     *                   %X – Standard time string
     *                   %y – Year in decimal without century(0-99)
     *                   %Y – Year including century as decimal
     *                   %Z – Time zone name
     *
     *     %F            File where method was invoked.
     *     %M            Method where method was invoked.
     *     %L            Line where method was invoked.
     *     %S            Log severity.
     * @param file File where logger was invoked.
     * @param function Function where logger was invoked.
     * @param line Line where logger was invoked.
     * @param severity Severity of the log content.
     *
     * @return String containing the header of log based on the format
     *         specifiers.
     */
    std::string buildInfo(const std::string & format,
                          const std::string & file,
                          const std::string & function,
                          const int & line,
                          const SeverityLevel & sl);

    LogSetting _logSetting; ///< All log behaviour settings.
    std::mutex _mtxLog; ///< Protection for multiple threads trying to write a log.
};

#endif // LOG_H_
