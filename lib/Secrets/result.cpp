/*
 * Copyright (C) 2018 Jolla Ltd.
 * Contact: Chris Adams <chris.adams@jollamobile.com>
 * All rights reserved.
 * BSD 3-Clause License, see LICENSE.
 */

#include "Secrets/result.h"
#include "Secrets/result_p.h"

using namespace Sailfish::Secrets;

//--------------------------------------------

ResultPrivate::ResultPrivate()
    : QSharedData()
    , m_errorCode(Result::NoError)
    , m_code(Result::Succeeded)
{
}

ResultPrivate::ResultPrivate(const ResultPrivate &other)
    : QSharedData(other)
    , m_errorMessage(other.m_errorMessage)
    , m_errorCode(other.m_errorCode)
    , m_code(other.m_code)
{
}


ResultPrivate::~ResultPrivate()
{
}

//--------------------------------------------

/*!
  \class Result
  \brief The result of a secrets operation

  The result encapsulates information about whether a given secrets
  operation succeeded, failed, or is pending conclusion.  If the
  operation failed, the result will also include some extra information
  about why the operation failed, to better allow client applications
  to handle the failure gracefully.
 */

/*!
  \brief Constructs a new, empty, successful result.
 */
Result::Result(Result::ResultCode resultCode)
    : d_ptr(new ResultPrivate)
{
    d_ptr->m_code = resultCode;
    d_ptr->m_errorCode = resultCode == Result::Failed
            ? Result::UnknownError
            : Result::NoError;
    d_ptr->m_errorMessage = resultCode == Result::Failed
            ? QString::fromLatin1("Unknown error")
            : QString();
}

/*!
  \brief Constructs a new result with the given \a errorCode and \a errorMessage
 */
Result::Result(Result::ErrorCode errorCode, const QString &errorMessage)
    : d_ptr(new ResultPrivate)
{
    d_ptr->m_errorCode = errorCode;
    d_ptr->m_errorMessage = errorMessage;
    d_ptr->m_code = errorCode >= Result::UnknownError
            ? Result::Failed
            : Result::Succeeded;
}

/*!
  \brief Constructs a copy of the \a other result
 */
Result::Result(const Result &other)
    : d_ptr(other.d_ptr)
{
}

/*!
  \brief Destroys the result
 */
Result::~Result()
{
}

/*!
  \brief Assigns the \a other result to this result
 */
Result& Result::operator=(const Result &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
  \brief Sets the error message associated with the result to \a m
 */
void Result::setErrorMessage(const QString &m)
{
    d_ptr->m_errorMessage = m;
}

/*!
  \brief Returns the error message associated with the result

  The error message is not meant for consumption by users of
  the application, as it will not be translated and may contain
  technical information.  It is primarily intended for use
  by developers during development and debugging of applications.
 */
QString Result::errorMessage() const
{
    return d_ptr->m_errorMessage;
}

/*!
  \brief Sets the error code associated with the result to \ c
 */
void Result::setErrorCode(int c)
{
    d_ptr->m_errorCode = static_cast<Result::ErrorCode>(c);
}

/*!
  \brief Sets the error code associated with the result to \ c
 */
void Result::setErrorCode(Result::ErrorCode c)
{
    d_ptr->m_errorCode = c;
}

/*!
  \brief Returns the error code associated with the result
 */
Result::ErrorCode Result::errorCode() const
{
    return d_ptr->m_errorCode;
}

/*!
  \brief Sets the result code associated with the result to \ c
 */
void Result::setCode(int c)
{
    d_ptr->m_code = static_cast<Result::ResultCode>(c);
}

/*!
  \brief Sets the result code associated with the result to \ c
 */
void Result::setCode(Result::ResultCode c)
{
    d_ptr->m_code = c;
}

/*!
  \brief Returns the result code associated with the result
 */
Result::ResultCode Result::code() const
{
    return d_ptr->m_code;
}

/*!
  \brief Returns true if the \a lhs result is equal to the \a rhs result
 */
bool Sailfish::Secrets::operator==(const Result &lhs, const Result &rhs)
{
    return lhs.code() == rhs.code()
            && lhs.errorCode() == rhs.errorCode()
            && lhs.errorMessage() == rhs.errorMessage();
}

/*!
  \brief Returns false if the \a lhs result is equal to the \a rhs result
 */
bool Sailfish::Secrets::operator!=(const Result &lhs, const Result &rhs)
{
    return !(operator==(lhs, rhs));
}

/*!
  \brief Returns true if the \a lhs result should sort less than \a rhs result
 */
bool Sailfish::Secrets::operator<(const Result &lhs, const Result &rhs)
{
    if (lhs.code() != rhs.code())
        return lhs.code() < rhs.code();

    if (lhs.errorCode() != rhs.errorCode())
        return lhs.errorCode() < rhs.errorCode();

    return lhs.errorMessage() < rhs.errorMessage();
}
