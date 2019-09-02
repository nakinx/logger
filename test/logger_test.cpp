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

#include "src/logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdio>
#include <exception>


const static std::string logName = "logger";
const static std::string logPath = "/tmp/";
static int logRecordsCount = 0;

#define THREAD_RECORDS 10000

int startTest();
int findRecordInFile(const std::string & file,
                     const std::string & str);
bool creatingSimpleLogRecord(const std::string & file);
void threadLoop();
bool threadTest(const std::string & file);
bool loggerWhithoutPermissionTest();
bool loggerDetroyingTest();
bool loggerGettingTest();
bool loggerBuilderTest();
bool loggerEnableTest(const std::string & file);
bool loggerActiveSeverityTest(const std::string & file);

int main(int argc,
         char * argv[]) {
    int result = startTest();

    std::cout << "\n===Test finished with " << result << " of 8 approved.===\n";

    return (0);
}

int startTest() {
    int rc = 0;
    int qtyApprovedTest = 0;

    logRecordsCount = 1;

    std::string absPath = std::string(logPath) + std::string(logName);

    std::cout << "Initializing test application! \n";

    rc = std::remove(absPath.c_str());

    if (rc != 0) {
        std::cout << "[FAIL] Error while deleting file: (" << absPath << "). RC: (" << rc << ")\n";
    } else {
        std::cout << "[OK] Old log file removed sucessfully! File: (" << absPath << ")\n";
    }

    LogBuilder::getInstance().buildLogger(logName, logPath);

    LogBuilder::getInstance().getLogger(logName)->setInfoFormat("[%D{%Y-%m-%d %H:%M:%S:%q}][%S] - ");

    if (creatingSimpleLogRecord(absPath) == true)
        qtyApprovedTest++;

    if (threadTest(absPath) == true)
        qtyApprovedTest++;

    if (loggerWhithoutPermissionTest() == true)
        qtyApprovedTest++;

    if (loggerDetroyingTest() == true)
        qtyApprovedTest++;

    if (loggerGettingTest() == true)
        qtyApprovedTest++;

    if (loggerBuilderTest() == true)
        qtyApprovedTest++;

    if (loggerEnableTest(absPath) == true)
        qtyApprovedTest++;

    if (loggerActiveSeverityTest(absPath) == true)
        qtyApprovedTest++;


    return qtyApprovedTest;
}

int findRecordInFile(const std::string & file,
                     const std::string & str) {
    int lineCount = 0;
    std::string line = "";
    std::ifstream inFile;

    inFile.open(file, std::ifstream::in);

    if (inFile.good() == false) {
        std::cout << "[FAIL] Error while trying to open (" << file << ") to find a record with (" << str << ")\n";
        return -1;
    }

    while (std::getline(inFile, line)) {
        if (line.find(str) != std::string::npos) {
            lineCount++;
        }
    }

    inFile.close();

    return lineCount;
}

bool creatingSimpleLogRecord(const std::string & file) {
    std::cout << "===> Creating simple record log test!\n";

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Simple record test!");
    logRecordsCount++;

    if (findRecordInFile(file, "Simple record test!") > 0) {
        std::cout << "[OK] Simple log record inserted with basic sytanx call.\n";
    } else {
        std::cout << "[FAIL] Simple log record inserted with basic sytanx call.\n";
        return false;
    }

    return true;
}

void threadLoop() {
    for (int i = 1; i <= THREAD_RECORDS; i++) {
        LOG_DEBUG(logName, "Thread test - " << "(" << logRecordsCount << ") Writing record (" << i << ") of " << THREAD_RECORDS << " with thread id (" << std::this_thread::get_id() << ")");
        logRecordsCount++;
    }
}

bool threadTest(const std::string & file) {
    std::cout << "===> Starting thread stress test!\n";

    int initialLogRecordsCount = logRecordsCount;

    std::thread tone(threadLoop);
    std::thread ttwo(threadLoop);
    std::thread tthree(threadLoop);
    std::thread tfour(threadLoop);

    tone.join();
    ttwo.join();
    tthree.join();
    tfour.join();

    std::cout << "Thread stress test finished!\n";

    int lineCount = findRecordInFile(file, "Thread test");

    if (lineCount == (4 * THREAD_RECORDS)) {
        std::cout << "[OK] Thread stress test wrote everything.\n";

        if ((logRecordsCount - initialLogRecordsCount) == (4 * THREAD_RECORDS)) {
            std::cout << "[OK] Thread stress test is synchronized with global counter.\n";
            return true;
        } else {
            std::cout << "[FAIL] Thread stress test is synchronized with global counter.\n";
            return false;
        }
    } else {
        std::cout << "[FAIL] Thread stress test wrote everything.\n";
        return false;
    }

    return false;
}

bool loggerWhithoutPermissionTest() {
    std::cout << "===> Initiating logger without permission test.\n";

    bool wasIssuedException = false;

    try {
        LogBuilder::getInstance().buildLogger("build_test", "/var/log/");
        LOG_DEBUG("build_test", "Build logger on directory without permission!");
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == false) {
        std::cout << "[FAIL] No exception issued while building on directory without permission.\n";
        return false;
    } else {
        std::cout << "[OK] Exception issued sucessfully while building on directory without permission.\n";
    }

    return true;
}

bool loggerDetroyingTest() {
    std::cout << "===> Initiating destroying logger test.\n";

    bool wasIssuedException = false;
    std::string logName = "log_to_destroy";
    std::string logPath = "/tmp/";

    try {
        LogBuilder::getInstance().buildLogger(logName, logPath);

        LogBuilder::getInstance().destroyLogger("wrong_log_name");
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[OK] Exception issued sucessfully while destroying logger.\n";
    } else {
        std::cout << "[FAIL] No exception issued while destroying logger.\n";
        return false;
    }

    wasIssuedException = false;

    try {
        LogBuilder::getInstance().destroyLogger(logName);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[FAIL] Exception issued while destroying logger.\n";
        return false;
    } else {
        std::cout << "[OK] No exception issued while destroying logger correctly.\n";
    }

    return true;
}

bool loggerGettingTest() {
    std::cout << "===> Initiating getting logger test.\n";

    bool wasIssuedException = false;
    std::string logName = "log_to_get";
    std::string logPath = "/tmp/";

    try {
        LogBuilder::getInstance().buildLogger(logName, logPath);

        LogBuilder::getInstance().getLogger("wrong_log_name");
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[OK] Exception issued sucessfully while getting logger.\n";
    } else {
        std::cout << "[FAIL] No exception issued while getting logger.\n";
        return false;
    }

    wasIssuedException = false;

    try {
        LogBuilder::getInstance().getLogger(logName);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[FAIL] Exception issued while getting logger.\n";
        return false;
    } else {
        std::cout << "[OK] No exception issued while getting logger correctly.\n";
    }

    return true;
}

bool loggerBuilderTest() {
    std::cout << "===> Initiating builder logger test.\n";

    bool wasIssuedException = false;
    std::string logPath = "/tmp/";

    try {
        LogBuilder::getInstance().buildLogger("", logPath);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[OK] Exception issued sucessfully while trying to build with empty string.\n";
    } else {
        std::cout << "[FAIL] No exception issued while trying to build with empty string.\n";
        return false;
    }

    wasIssuedException = false;

    try {
        LogBuilder::getInstance().buildLogger("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", logPath);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[OK] Exception issued sucessfully while trying to build with bigger string.\n";
    } else {
        std::cout << "[FAIL] No exception issued while trying to build with bigger string.\n";
        return false;
    }

    wasIssuedException = false;

    try {
        LogBuilder::getInstance().buildLogger("log_exist", logPath);
        LogBuilder::getInstance().buildLogger("log_exist", logPath);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[OK] Exception issued sucessfully while trying to build a logger that already exist.\n";
    } else {
        std::cout << "[FAIL] No exception issued while trying to build a logger that already exist.\n";
        return false;
    }

    wasIssuedException = false;

    try {
        LogBuilder::getInstance().buildLogger("new_log", logPath);
    } catch (LoggerException & e) {
        std::cerr << "[" << __PRETTY_FUNCTION__ << "][" << __LINE__ << "] - " << e.what() << "\n";
        wasIssuedException = true;
    }

    if (wasIssuedException == true) {
        std::cout << "[FAIL] Exception issued while trying to build a logger.\n";
        return false;
    } else {
        std::cout << "[OK] No exception issued while trying to build a logger correctly.\n";
    }

    return true;
}

bool loggerEnableTest(const std::string & file) {
    std::cout << "===> Testing enable/disable!\n";

    LogBuilder::getInstance().getLogger(logName)->setEnable(false);

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Enable test false!");

    if (findRecordInFile(file, "Enable test false!") > 0) {
        std::cout << "[FAIL] Enable test.\n";
        return false;
    } else {
        std::cout << "[OK] Enable test.\n";
    }

    LogBuilder::getInstance().getLogger(logName)->setEnable(true);

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Enable test true!");

    if (findRecordInFile(file, "Enable test true!") > 0) {
        std::cout << "[OK] Enable test.\n";
        logRecordsCount++;
    } else {
        std::cout << "[FAIL] Enable test.\n";
        return false;
    }

    return true;
}

bool loggerActiveSeverityTest(const std::string & file) {
    std::cout << "===> Testing active severity!\n";

    LogBuilder::getInstance().getLogger(logName)->setActiveSeverity(static_cast<int>(SeverityLevel::Info) | static_cast<int>(SeverityLevel::Warning));

    LOG_INFO(logName, "(" << logRecordsCount << ") Logging info!");
    LOG_WARNING(logName, "(" << logRecordsCount << ") Logging waring!");

    if ((findRecordInFile(file, "Logging info!") > 0) &&
         (findRecordInFile(file, "Logging waring!") > 0)) {
        std::cout << "[OK] Logging allowed is info/warning.\n";
        logRecordsCount+=2;
    } else {
        std::cout << "[FAIL] Logging allowed is info/warning.\n";
        return false;
    }

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging debug not allowed!");

    if (findRecordInFile(file, "Logging debug not allowed!") > 0) {
        std::cout << "[FAIL] Logging allowed is info/warning not debug.\n";
        logRecordsCount++;
        return false;
    } else {
        std::cout << "[OK] Logging allowed is info/warning not debug.\n";
    }

    LogBuilder::getInstance().getLogger(logName)->addActiveSeverity(static_cast<int>(SeverityLevel::Debug));

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging debug added!");

    if (findRecordInFile(file, "Logging debug added!") > 0) {
        std::cout << "[OK] Logging debug added.\n";
        logRecordsCount++;
    } else {
        std::cout << "[FALSE] Logging debug added.\n";
        return false;
    }

    LogBuilder::getInstance().getLogger(logName)->rmActiveSeverity(static_cast<int>(SeverityLevel::Debug));

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging debug removed!");

    if (findRecordInFile(file, "Logging debug removed!") > 0) {
        std::cout << "[FALSE] Logging debug removed.\n";
        return false;
    } else {
        std::cout << "[OK] Logging debug removed.\n";
    }

    LogBuilder::getInstance().getLogger(logName)->enableAllSeverity();

    LOG_INFO(logName, "(" << logRecordsCount << ") Logging all info!");
    LOG_WARNING(logName, "(" << logRecordsCount << ") Logging all warning!");
    LOG_ERROR(logName, "(" << logRecordsCount << ") Logging all error!");
    LOG_FATAL(logName, "(" << logRecordsCount << ") Logging all fatal!");
    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging all debug!");

    if ((findRecordInFile(file, "Logging all info!") > 0) &&
        (findRecordInFile(file, "Logging all warning!") > 0) &&
        (findRecordInFile(file, "Logging all error!") > 0) &&
        (findRecordInFile(file, "Logging all fatal!") > 0) &&
        (findRecordInFile(file, "Logging all debug!") > 0)) {
        std::cout << "[OK] Logging all severity.\n";
        logRecordsCount+=5;
    } else {
        std::cout << "[FAIL] Logging all severity.\n";
        return false;
    }

    LogBuilder::getInstance().getLogger(logName)->setDebugSeverityEnable(false);

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging set debug disable!");

    if (findRecordInFile(file, "Logging set debug disable!") > 0) {
        std::cout << "[FALSE] Logging set debug disable.\n";
        return false;
    } else {
        std::cout << "[OK] Logging set debug disable.\n";
    }

    LogBuilder::getInstance().getLogger(logName)->setDebugSeverityEnable(true);

    LOG_DEBUG(logName, "(" << logRecordsCount << ") Logging set debug enable!");

    if (findRecordInFile(file, "Logging set debug enable!") > 0) {
        std::cout << "[OK] Logging set debug enable.\n";
        logRecordsCount++;
    } else {
        std::cout << "[FALSE] Logging set debug enable.\n";
        return false;
    }

    return true;
}