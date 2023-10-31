#ifndef ELFFILE_H
#define ELFFILE_H

#include <QString>

#include "elf.h"

class ELFFILE
{
private:
    Elf32_Ehdr    *pElfHeader;
    QString       *pElfFileName;

public:
    ELFFILE();
    ELFFILE(const QString *pFileName);
    ~ELFFILE();

};

#endif // ELFFILE_H
