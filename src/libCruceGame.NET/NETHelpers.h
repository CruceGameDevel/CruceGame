#pragma once

#include "errors.h"

#define CheckErrorCode(errorCode) \
    if (errorCode != NO_ERROR) \
    { \
        throw CruceGame::Helpers::GenerateErrorCodeException(errorCode); \
    } 

namespace CruceGame
{
    ref class Helpers
    {
    public:
        static System::Exception^ GenerateErrorCodeException(int errorCode);
        static System::IntPtr ManagedStringToUnmanaged(System::String^ value);
        static System::String^ UnmanagedStringToManaged(System::IntPtr value);
        static System::Void FreeUnmanagedString(System::IntPtr value);
    };
}