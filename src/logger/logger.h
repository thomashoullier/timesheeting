/** @file
 * @brief Logger call functions.
 *
 * This is the place to define which LoggerInterface implementation is used. */
#ifndef LOGGER_H
#define LOGGER_H

#include <string>

/** @brief Call an implementation of LoggerInterface::log(). */
void log(const std::string &msg);
/** @brief Call an implementation of LoggerInterface::tick(). */
void tick();
/** @brief Call an implementation of LoggerInterface::tock(). */
void tock();

#endif // LOGGER_H
