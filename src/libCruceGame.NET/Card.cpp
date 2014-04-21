#include "NETCard.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "deck.h"

using namespace System;

CruceGame::Card^ CruceGame::Card::Create(CruceGame::Suit suit, int value)
{
    ::Card* card = deck_createCard((::Suit)suit, value);
    if (card == (void*)0)
        return nullptr;
    return gcnew CruceGame::Card(IntPtr(card));
}

CruceGame::Card::Card(IntPtr handle) : _card(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Card::~Card()
{
    this->!Card();
}

CruceGame::Card::!Card()
{
    if (ReferenceCounter::Instance[_card] < 1) //already deleted
        return;
    ReferenceCounter::Instance->Subtract(_card);
    if (ReferenceCounter::Instance[_card] > 0)
        return;

    ::Card* card = static_cast<::Card*>(_card.ToPointer());
    deck_deleteCard(&card);
}

IntPtr CruceGame::Card::Handle::get()
{
    return _card;
}

CruceGame::Suit CruceGame::Card::Suit::get()
{
    if (_card == IntPtr::Zero)
        throw gcnew NullReferenceException("_card");

    ::Card* card = static_cast<::Card*>(_card.ToPointer());
    return (CruceGame::Suit)card->suit;
}

int CruceGame::Card::Value::get()
{
    if (_card == IntPtr::Zero)
        throw gcnew NullReferenceException("_card");

    ::Card* card = static_cast<::Card*>(_card.ToPointer());
    return card->value;
}

CruceGame::Card^ CruceGame::Card::Compare(CruceGame::Card^ second, CruceGame::Suit trump)
{
    if (_card == IntPtr::Zero)
        throw gcnew NullReferenceException("this->_card");

    if (second == nullptr)
        throw gcnew NullReferenceException("second");

    if (second->_card == IntPtr::Zero)
        throw gcnew NullReferenceException("second->_card");

    auto compareResult = deck_compareCards(static_cast<::Card*>(_card.ToPointer()),
        static_cast<::Card*>(second->_card.ToPointer()), (::Suit)trump);

    switch (compareResult)
    {
    case 1:
        return this;
    case 2:
        return second;
    case 0:
        return nullptr;
    default:
        throw CruceGame::Helpers::GenerateErrorCodeException(compareResult);
    }
}

String^ CruceGame::Card::ToString()
{
    String^ stringValue = String::Empty;
    switch (Value)
    {
    case 0:
        stringValue = L"IX";
        break;
    case 2:
        stringValue = L"2";
        break;
    case 3:
        stringValue = L"3";
        break;
    case 4:
        stringValue = L"4";
        break;
    case 10:
        stringValue = L"X";
        break;
    case 11:
        stringValue = L"A";
        break;
    }

    return String::Format(L"{0} of {1}", stringValue, Suit.ToString());
}

int CruceGame::Card::GetHashCode()
{
    return (int)_card.ToPointer();
}

bool CruceGame::Card::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Card^>(other));
}

bool CruceGame::Card::Equals(CruceGame::Card^ other)
{
    if (other == nullptr)
        return false;

    return _card == other->_card;
}