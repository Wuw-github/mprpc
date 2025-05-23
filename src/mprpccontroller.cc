#include "mprpccontroller.h"

MprpcController::MprpcController()
{
    m_failed = false;
    m_errorText = "";
}

void MprpcController::Reset()
{
    m_failed = false;
    m_errorText = "";
}

bool MprpcController::Failed() const
{
    return m_failed;
}

std::string MprpcController::ErrorText() const
{
    return m_errorText;
}

void MprpcController::SetFailed(const std::string &reason)
{
    m_failed = true;
    m_errorText = reason;
}

void MprpcController::StartCancel()
{
    // Not implemented
}

bool MprpcController::IsCanceled() const
{
    return false; // Not implemented
}

void MprpcController::NotifyOnCancel(google::protobuf::Closure *callback)
{
    // Not implemented
}