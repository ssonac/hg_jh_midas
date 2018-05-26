#pragma once

class Singleton
{
private:
    Singleton() {}
    Singleton* inst;

public:
    Singleton* getInstance();
    int showMessage();
};


