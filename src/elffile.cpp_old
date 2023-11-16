#include "../includes/elffile.h"

ELFFILE::ELFFILE()
{
}

ELFFILE::ELFFILE(const QString *pFileName)
{
  pElfHeader = new Elf32_Ehdr;
  pElfFileName = new QString(*pFileName);
}

ELFFILE::~ELFFILE()
{
    if ( pElfHeader )
        delete pElfHeader;
}
