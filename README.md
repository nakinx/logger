Logger
======

Logger it's a library for log purpose, implemented with C++ language. In the open source world we have a lot of librarys that manager logs like boost, log4cxx, g3log and many others, but logger was built to be a easy code to understand, making possible customize according to your needs.

Usage
=====

It's really easy to use logger, first you need to get a instance of the builder, second create a instance of the logger based on the settings you wish and finally create a log record.

Example:

    LogBuilder::getInstance().buildLogger("logger", "/tmp/");

    LOG_DEBUG("logger", "Simple record example!");

Result:

    Simple record example!

To create the log header with informations about data/time, function, file, line and severity, during the creation of logger or even after you need set format info.

Example:

    LogBuilder::getInstance().buildLogger("logger", "/tmp/");

    LogBuilder::getInstance().getLogger(logName)->setInfoFormat("[%D{%Y-%m-%d %H:%M:%S:%q}][%S] - ");

    LOG_DEBUG("logger", "Simple record example!");

Result:

    [2019-09-01 21:23:49:817][debug] - Simple record test!

To record a combination of string and numbers you may use the following syntax:

    LOG_DEBUG("logger", "Writing number " << 10);

For more information about all logger abilities you should check the logger_test.