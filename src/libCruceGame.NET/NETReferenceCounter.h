#pragma once

#include "NETConstants.h"

namespace CruceGame
{
    private ref class ReferenceCounter sealed
    {
    private:
        System::Collections::Generic::Dictionary<System::IntPtr, int> _values;
        static ReferenceCounter^ _instance;
        ReferenceCounter();
    public:
        static property ReferenceCounter^ Instance
        {
            ReferenceCounter^ get();
        }
        System::Void Add(System::IntPtr handle);
        System::Void Subtract(System::IntPtr handle);
        System::Void Remove(System::IntPtr handle);
        property int default[System::IntPtr]
        {
            int get(System::IntPtr handle);
        }
    };
}