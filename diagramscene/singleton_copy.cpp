#include "Singleton.h"
#include <iostream>

Singleton* Singleton::inst = nullptr;

Singleton* Singleton::getInstance()
{
    if (inst == nullptr)
        inst = new Singleton();

    return inst;
}

int Singleton::showMessage()
{
    return 4;
}
