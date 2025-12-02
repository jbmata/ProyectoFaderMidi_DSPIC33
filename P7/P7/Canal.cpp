#include "Canal.h"



Canal::Canal() {
    id = 0;
    ccNumber = 0;
    value = 0;
}

Canal::Canal(int idcanal, int cc, int valor){
    id = idcanal;
    ccNumber = cc;
    value = valor;
}

Canal::~Canal()
{
    
}

int Canal::getId() const
{
    return id;
}

int Canal::getCcNumber() const
{
    return ccNumber;
}

int Canal::getValue() const
{
    return value;
}

void Canal::setValue(int v)
{
    if (v < 0)   v = 0;
    if (v > 127) v = 127;
    value = v;
}
