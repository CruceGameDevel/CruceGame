#include "NETHelpers.h"
#include "errors.h"
#include <cstring>
#include <cstdlib>

using namespace System;
using namespace System::Text;
using namespace System::Runtime::InteropServices;

Exception^ CruceGame::Helpers::GenerateErrorCodeException(int errorCode)
{
    const char* message = error(errorCode);
    return gcnew Exception(gcnew String(message, 0, strlen(message), Encoding::UTF8));
}

IntPtr CruceGame::Helpers::ManagedStringToUnmanaged(String^ value)
{
    array<unsigned char>^ bytes = Encoding::UTF8->GetBytes(value);
    IntPtr destination = IntPtr(calloc(bytes->Length + 1, 1));
    Marshal::Copy(bytes, 0, destination, bytes->Length);

    return destination;
}

String^ CruceGame::Helpers::UnmanagedStringToManaged(IntPtr value)
{
    const char* _value = static_cast<const char*>(value.ToPointer());
    return gcnew String(_value, 0, strlen(_value), Encoding::UTF8);
}

Void CruceGame::Helpers::FreeUnmanagedString(IntPtr value)
{
    char* _value = static_cast<char*>(value.ToPointer());
    free(_value);
}