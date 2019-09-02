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

#ifndef LOG_EXCEPTION_
#define LOG_EXCEPTION_

#include <string>
#include <exception>

/**
 * Class for a custom exception.
 */
class LoggerException : public std::exception {

public:
    /**
     * Exception constructor with basic information of the error.
     *
     * @param code Error code.
     * @param msg Error message.
     */
    LoggerException(int code,
                    std::string msg);

    /**
     * Get the error message.
     *
     * @return Return a string containing the erro message.
     */
    virtual const char * what() const throw();

private:
    int _code; ///< Error code.
    std::string _msg; ///< Error message.
};

#endif // LOG_EXCEPTION_