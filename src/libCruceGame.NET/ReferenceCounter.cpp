#include "NETReferenceCounter.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace CruceGame;

ReferenceCounter::ReferenceCounter() : _values(gcnew Dictionary<IntPtr, int>)
{
}

ReferenceCounter^ ReferenceCounter::Instance::get()
{
    if (_instance == nullptr)
    {
        _instance = gcnew ReferenceCounter();
    }
    return _instance;
}

Void ReferenceCounter::Add(IntPtr handle)
{
    if (_values.ContainsKey(handle))
    {
        _values[handle] += 1;
    }
    else
    {
        _values.Add(handle, 1);
    }
}

Void ReferenceCounter::Subtract(IntPtr handle)
{
    if (_values.ContainsKey(handle))
    {
        _values[handle] -= 1;
        if (_values[handle] < 1)
        {
            _values.Remove(handle);
        }
    }
}

Void ReferenceCounter::Remove(IntPtr handle)
{
    if (_values.ContainsKey(handle))
    {
        _values.Remove(handle);
    }
}

int ReferenceCounter::default::get(IntPtr handle)
{
    if (_values.ContainsKey(handle))
    {
        return _values[handle];
    }
    return 0;
}