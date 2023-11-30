#ifndef ELFFILE_H
#define ELFFILE_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <QString>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// TODO: Move this to the disassembler class when created
#include <capstone/capstone.h>

using namespace std;

//#include "elfed.h"
#include "elf.h"

/*
extern char *FileClass[];
extern char *DataEncod[];
extern char *FType[];
extern char *Machine[];
extern char *PHSegType[];
extern char *SHSegType[];
extern char *Version[];
*/

typedef struct {
  unsigned char	namesz[4];		/* Size of entry's owner string */
  unsigned char	descsz[4];		/* Size of the note descriptor */
  unsigned char	type[4];		/* Interpretation of the descriptor */
  char		name[1];		/* Start of the name+desc data */
} Elf_External_Note;

typedef struct {
  int index;
  int Offset;
  int reported_size;  // was size
  int actual_size;    // was padcount;
  unsigned char *rawData;
}SECTION_RAW_DATA;

typedef struct {
  int Offset;
  char *f_string;
}STRING_DATA;

class ELFFILE
{
 public:
  ELFFILE();
  ~ELFFILE();
  int ELFOpen(const QString *);
  int ELFRead();
  void ELFDisplay();
  int get_PT_Dynamic_entrycount();
  unsigned int get_PT_Dynamic_offset();
  char *get_PT_Dynamic_dtag(int);
  const char *get_PT_Dynamic_dtagtype(int);
  char *get_PT_Dynamic_dtagNVal(int);
  const char *get_symtab_String_Val();
  const char *get_Interpreter_String_Val();
  int  get_Symtable_EntryCount();
  char *get_SYMTAB_Value(int);
  char *get_SYMTAB_Size(int);
  char *get_SYMTAB_Bind(int);
  char *get_SYMTAB_Type(int);
  const char *get_SYMTAB_SHIndex(int);
  const char *get_SYMTAB_Name(int);


  Elf32_Ehdr *get_ELFhdrptr();
  char *get_ID();
  const char *get_Class();
  const char *get_EncodeType();
  char *get_File_Version();
  const char *get_File_Type();
  const char *get_Machine_Type();
  const char *get_Version();
  char *get_Entry_Point();
  void  set_Entry_Point(unsigned int);
  char *get_PHDRT_Offset();
  char *get_SHDRT_Offset();
  char *get_Flags();
  char *get_HDR_Size();
  char *get_HDRTE_Size();
  char *get_TPHDR_Entries();
  char *get_SHDR_Size();
  char *get_TST_Entries();
  char *get_SHDRT_Index();
  const char *get_PHSegType(int);
  Elf32_Phdr *get_PHDRptr();
  int   get_PHDR_Count();
  char *get_PH_offset(int);
  char *get_PH_vaddress(int);
  char *get_PH_physaddress(int);
  char *get_PH_filesz(int);
  char *get_PH_memsz(int);
  char *get_PH_flags(int);
  char *get_PH_align(int);
  int   get_SHDR_Count();
  char *get_SH_name(int);
  const char *get_SHSegType(int);
  char *get_SH_addr(int);
  char *get_SH_offset(int);
  char *get_SH_size(int);
  char *get_SH_entsize(int);
  char *get_SH_flags(int);
  char *get_SH_link(int);
  char *get_SH_info(int);
  char *get_SH_addralign(int);
  bool  get_IsDirty();
  void  set_IsDirty(bool);
  int   save_File_as(char *);
  int   get_StringOffset(int);
  char  *get_StringVal(int);
  int   get_StringCount();
  int   get_fd();
 private:
  int fd; // file descriptor
  int stringcount;
  int charcount;
  int stringmemblockcount;
  unsigned char inBuff[1024];
  char tempstr[1024]; // temp string used in char return functions
  char tempstr2[1024]; // temp string used in char return functions
  bool isDirty;
  bool hasDynamic;
  int num_Phdrs; // number of program headers
  int num_Shdrs; // number of section headers
  //  int Phdrs_index; // program header index
  //  int Shdrs_index; // section header index
  int StrTableCount;
  int PT_DynamicCount;
  int PT_Dynamic_Index;
  int PT_DYNAMIC_Entrycount;
  int symtabEntries;
  unsigned long pt_loadAddr;
  unsigned long dt_Strtab_Address;

  Elf32_Ehdr *elfhdr;
  Elf32_Shdr *elfShdr, *elfShdrptr;
  Elf32_Phdr *elfPhdr, *elfPhdrptr;
  Elf32_Dyn  *elfDyn, *elfDynptr;
  Elf32_Sym  *elfSym, *elfSymptr;

  char *shstrTable;
  
  SECTION_RAW_DATA *sectionRawData, *sectionRawDataptr;
  STRING_DATA *stringdata, *stringdataptr;

  const char *getString(int, int);
  //  char *getString(int);
  const char *getString(char *buff, int offset);
  int read_PT_Dynamic_entries(int, int, int);
  int checkMagic(unsigned char *);
  int processStrings(int);

};

extern ELFFILE *elffile;

#endif
