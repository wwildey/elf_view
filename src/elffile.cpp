#include "../includes/elffile.h"
#include "elf.h"

#define ELFMAGICSIZE 16

const char *FileClass[] = {"Invalid Class", "32-bit", "64-bit"};

const char *DataEncode[] = {"Invalid", "LSB", "MSB"};

const char *FType[] = {
    "None",
    "Relocatable",
    "Executable",
    "Shared Object",
    "Core",
    "OS Specific",
    "Processor Specific"};

const char *Machine[] = {
    "None",
    "AT&T WE 32100",
    "SPARC",
    "INTEL 80386",
    "Motorola 68000"
    "Motorola 88000",
    "Intel 80860",
    "MIPS RS3000"
    "IBM System/370",
    "MIPS R3000 little-endian"};

struct PHSEGTYPE
{
  unsigned long value;
  const char *Type;
  const char *Description;
} PHSegType[] = {
    {0, "PT_NULL", "Program header table entry unused"},
    {1, "PT_LOAD", "Loadable program segment"},
    {2, "PT_DYNAMIC", "Dynamic linking information"},
    {3, "PT_INTERP", "Program interpreter"},
    {4, "PT_NOTE", "Auxiliary information"},
    {5, "PT_SHLIB", "Reserved"},
    {6, "PT_PHDR", "Entry for header table itself"},
    {7, "PT_TLS", "Thread-local storage segment"},
    {8, "PT_NUM", "Number of defined types"},
    {0x60000000, "PT_LOOS", "Start of OS-specific"},
    {0x6474e550, "PT_GNU_EH_FRAME", "GCC eh_frame_hdr segment"},
    {0x6474e551, "PT_GNU_STACK", "Indicates stack executability"},
    {0x6474e552, "PT_GNU_RELRO", "Read-only after relocation"},
    {0x6ffffffa, "PT_LOSUNW", "Unknown"},
    {0x6ffffffa, "PT_SUNWBSS", "Sun Specific segment"},
    {0x6ffffffb, "PT_SUNWSTACK", "Stack segment "},
    {0x6fffffff, "PT_HISUNW", "Unknown"},
    {0x6fffffff, "PT_HIOS", "End of OS-specific "},
    {0x70000000, "PT_LOPROC", "Start of processor-specific "},
    {0x7fffffff, "PT_HIPROC", "End of processor-specific"},
};

struct SHSEGTYPE
{
  unsigned long value;
  const char *Type;
  const char *Description;
} SHSegType[] = {
    {0, "SHT_NULL", "Section header table entry unused"},
    {1, "SHT_PROGBITS", "Program data"},
    {2, "SHT_SYMTAB", "Symbol table"},
    {3, "SHT_STRTAB", "String table"},
    {4, "SHT_RELA", "Relocation entries with addends"},
    {5, "SHT_HASH", "Symbol hash table"},
    {6, "SHT_DYNAMIC", "Dynamic linking information"},
    {7, "SHT_NOTE", "Notes"},
    {8, "SHT_NOBITS", "Program space with no data (bss)"},
    {9, "SHT_REL", "Relocation entries, no addends"},
    {10, "SHT_SHLIB", "Reserved"},
    {11, "SHT_DYNSYM", "Dynamic linker symbol table"},
    {14, "SHT_INIT_ARRAY", "Array of constructors"},
    {15, "SHT_FINI_ARRAY", "Array of destructors"},
    {16, "SHT_PREINIT_ARRAY", "Array of pre-constructors"},
    {17, "SHT_GROUP", "Section group"},
    {18, "SHT_SYMTAB_SHNDX", "Extended section indeces"},
    {19, "SHT_NUM", "Number of defined types"},
    {0x60000000, "SHT_LOOS", "Start OS-specific "},
    {0x6ffffff7, "SHT_GNU_LIBLIST", "Prelink library list"},
    {0x6ffffff8, "SHT_CHECKSUM", "Checksum for DSO content."},
    {0x6ffffffa, "SHT_LOSUNW", "Sun-specific low bound."},
    {0x6ffffffa, "SHT_SUNW_move", "unknown"},
    {0x6ffffffb, "SHT_SUNW_COMDAT", "unknown"},
    {0x6ffffffc, "SHT_SUNW_syminfo", "unknown"},
    {0x6ffffffd, "SHT_GNU_verdef", "Version definition section"},
    {0x6ffffffe, "SHT_GNU_verneed", "Version needs section"},
    {0x6fffffff, "SHT_GNU_versym", "Version symbol table"},
    {0x6fffffff, "SHT_HISUNW", "Sun-specific high bound"},
    {0x6fffffff, "SHT_HIOS", "End OS-specific type"},
    {0x70000000, "SHT_LOPROC", "Start of processor-specific"},
    {0x7fffffff, "SHT_HIPROC", "End of processor-specific"},
    {0x80000000, "SHT_LOUSER", "Start of application-specific"},
    {0x8fffffff, "SHT_HIUSER", "End of application-specific"},
};

/* Dynamic section tags. info taken from readelf common.h include file */
struct DT_TAGS
{
  unsigned long value;
  const char *Type;
} DT_Tags[] = {
    {0, "DT_NULL"},
    {1, "DT_NEEDED"},
    {2, "DT_PLTRELSZ"},
    {3, "DT_PLTGOT"},
    {4, "DT_HASH"},
    {5, "DT_STRTAB"},
    {6, "DT_SYMTAB"},
    {7, "DT_RELA"},
    {8, "DT_RELASZ"},
    {9, "DT_RELAENT"},
    {10, "DT_STRSZ"},
    {11, "DT_SYMENT"},
    {12, "DT_INIT"},
    {13, "DT_FINI"},
    {14, "DT_SONAME"},
    {15, "DT_RPATH"},
    {16, "DT_SYMBOLIC"},
    {17, "DT_REL"},
    {18, "DT_RELSZ"},
    {19, "DT_RELENT"},
    {20, "DT_PLTREL"},
    {21, "DT_DEBUG"},
    {22, "DT_TEXTREL"},
    {23, "DT_JMPREL"},
    {24, "DT_BIND_NOW"},
    {25, "DT_INIT_ARRAY"},
    {26, "DT_FINI_ARRAY"},
    {27, "DT_INIT_ARRAYSZ"},
    {28, "DT_FINI_ARRAYSZ"},
    {29, "DT_RUNPATH"},
    {30, "DT_FLAGS"},
    {31, "DT_ENCODING"},
    {32, "DT_PREINIT_ARRAY"},
    {33, "DT_PREINIT_ARRAYSZ"},

    /* Note, the Oct 4, 1999 draft of the ELF ABI changed the values
       for DT_LOOS and DT_HIOS.  Some implementations however, use
       values outside of the new range (see below).	 */
    {0x60000000, "OLD_DT_LOOS"},
    {0x6000000d, "DT_LOOS"},
    {0x6ffff000, "DT_HIOS"},
    //  {0x6fffffff, "OLD_DT_HIOS"},
    //  {0x70000000, "DT_LOPROC"},
    //  {0x7fffffff, "DT_HIPROC"},

    /* The next four dynamic tags are used on Solaris.  We support them
       everywhere.	Note these values lie outside of the (new) range for
       OS specific values.	This is a deliberate special case and we
       maintain it for backwards compatability.  */
    {0x6ffffd00, "DT_VALRNGLO"},
    {0x6ffffdf5, "DT_GNU_PRELINKED"},
    {0x6ffffdf6, "DT_GNU_CONFLICTSZ"},
    {0x6ffffdf7, "DT_GNU_LIBLISTSZ"},
    {0x6ffffdf8, "DT_CHECKSUM"},
    {0x6ffffdf9, "DT_PLTPADSZ"},
    {0x6ffffdfa, "DT_MOVEENT"},
    {0x6ffffdfb, "DT_MOVESZ"},
    {0x6ffffdfc, "DT_FEATURE"},
    {0x6ffffdfd, "DT_POSFLAG_1"},
    {0x6ffffdfe, "DT_SYMINSZ"},
    {0x6ffffdff, "DT_SYMINENT"},
    {0x6ffffdff, "DT_VALRNGHI"},
    {0x6ffffe00, "DT_ADDRRNGLO"},
    {0x6ffffef6, "DT_TLSDESC_PLT"},
    {0x6ffffef7, "DT_TLSDESC_GOT"},
    {0x6ffffef8, "DT_GNU_CONFLICT"},
    {0x6ffffef9, "DT_GNU_LIBLIST"},
    {0x6ffffefa, "DT_CONFIG"},
    {0x6ffffefb, "DT_DEPAUDIT"},
    {0x6ffffefc, "DT_AUDIT"},
    {0x6ffffefd, "DT_PLTPAD"},
    {0x6ffffefe, "DT_MOVETAB"},
    {0x6ffffeff, "DT_SYMINFO"},
    {0x6ffffeff, "DT_ADDRRNGHI"},
    {0x6ffffff9, "DT_RELACOUNT"},
    {0x6ffffffa, "DT_RELCOUNT"},
    {0x6ffffffb, "DT_FLAGS_1"},
    {0x6ffffffc, "DT_VERDEF"},
    {0x6ffffffd, "DT_VERDEFNUM"},
    {0x6ffffffe, "DT_VERNEED"},
    {0x6fffffff, "DT_VERNEEDNUM"},

    /* This tag is a GNU extension to the Solaris version scheme.  */
    {0x6ffffff0, "DT_VERSYM"},
    {0x70000000, "DT_LOPROC"},
    {0x7fffffff, "DT_HIPROC"},

    /* These section tags are used on Solaris.  We support them
       everywhere, and hope they do not conflict.  */

    {0x7ffffffd, "DT_AUXILIARY"},
    {0x7ffffffe, "DT_USED"},
    //  {0x7fffffff, "DT_FILTER"}0x
};

const char *Version[] = {"Invalid", "Current"};

unsigned char ELFMAGIC32[ELFMAGICSIZE] = {0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char ELFMAGIC64[ELFMAGICSIZE] = {0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef struct _STRDATA
{
  int charindex; // section name string table index
                 // is actually a index to beginning of string
  int index;
  char *Name;
  _STRDATA *next;
  _STRDATA *prev;
} STRDATA;

typedef struct _STRTBLS
{
  int TableNum;
  STRDATA *strdata;
} STRTBLS;

ELFFILE::ELFFILE()
{
  fd = 0;
  isDirty = false;
  //  Phdrs_index = 0;
  //  Shdrs_index = 0;
  StrTableCount = 0;
  PT_DynamicCount = 0;
  PT_Dynamic_Index = 0;
  hasDynamic = 0;
}

ELFFILE::~ELFFILE()
{
}

// ELFOpen(char *)
// Opens an elf file for reading/writing
//
// Parameters: char *fpath - Name and path of file to open
// return: 0 on success
//         fd equal to the error code returned when open was called
int ELFFILE::ELFOpen(const QString *fpath)
{
  char target_path[1024];
  int len = 0;
  hasDynamic = 0;

  len = readlink(fpath->toStdString().c_str(), target_path, sizeof(target_path));
  //  cout << "len =  " << len << endl;
  if (len != -1) // call not failed
  {
    target_path[len + 1] = '\0';                                                     // null terminate target path
    strncpy((char *)fpath->toStdString().c_str(), target_path, strlen(target_path)); // copy true file name for display
  }
  else
  {
    strncpy(target_path, fpath->toStdString().c_str(), strlen(fpath->toStdString().c_str()));
  }

  if ((fd = open(target_path, O_RDONLY, 0)) <= 0)
  {
    cout << "File open error " << target_path << endl;
    printf("%s\n", strerror(errno));
    return (fd); // failure
  }

  return (fd); // success
}

// ELFRead()
// Reads in and parses an ELF file with information sent to the ELFOpen function
//
// Parameters: Nothing
// Return:     0 on success
//             > 0 on failure
// TODO: 1. Currently cout is used to send error codes to the console.  Will
//          need a error reporting class that will display the error string in
//          a modal window.
//       2. Break this up into smaller functions
int ELFFILE::ELFRead()
{
  int BytesRead = 0;
  int ii;
  //  unsigned int bytes_2_read;
  //  Elf32_Shdr *tempShdrptr;

  PT_DynamicCount = 0;
  PT_Dynamic_Index = 0;
  PT_DYNAMIC_Entrycount = 0;

  if (fd <= 0)
  {
    cout << "No ELF file opened!" << endl;
    return (-1); // failure
  }

  // TODO: Read file to determine if it is a 64 or 32 bit binary
  if ((elfhdr = (Elf32_Ehdr *)calloc(1, sizeof(Elf32_Ehdr))) == NULL)
  {
    cout << "Memory allocation error (elfhdr)!" << endl;
    return (-3); // failure
  }

  //  if((BytesRead = read(fd, inBuff, sizeof(Elf32_Ehdr))) < 1)

  if ((BytesRead = read(fd, elfhdr, sizeof(Elf32_Ehdr))) < 1)
  {
    cout << "File read error (Elf32_Ehdr): bytesRead " << BytesRead << endl;
    return (-2); // failure
  }

  // memcpy(elfhdr, inBuff, sizeof(Elf32_Ehdr));

  if (checkMagic(elfhdr->e_ident) != 0)
  {
    cout << "Not a valid ELF file!" << endl;
    cout << "Magic Number aquired: ";
    for (ii = 0; ii < 16; ii++)
      printf("%02x ", elfhdr->e_ident[ii]);

    cout << endl;
    return (-1); // failed
  }

  num_Phdrs = elfhdr->e_phnum;
  num_Shdrs = elfhdr->e_shnum;

  if ((elfPhdr = (Elf32_Phdr *)calloc(num_Phdrs + 1, sizeof(Elf32_Phdr))) == NULL)
  {
    cout << "Memory allocation error (elfPhdr)!" << endl;
    return (-3); // failure
  }

  if ((elfShdr = (Elf32_Shdr *)calloc(num_Shdrs + 1, sizeof(Elf32_Shdr))) == NULL)
  {
    cout << "Memory allocation error (elfShdr)!" << endl;
    return (-4); // failure
  }

  if ((sectionRawData = (SECTION_RAW_DATA *)calloc(num_Shdrs + 1, sizeof(SECTION_RAW_DATA))) == NULL)
  {
    cout << "Memory allocation error (sectionRawData)!" << endl;
    return (-4); // failure
  }

  // Start reading program headers, file MUST have program headers
  lseek(fd, elfhdr->e_phoff, 0); // set location to pheader entry block
  for (ii = 0; ii < num_Phdrs; ii++)
  {
    elfPhdrptr = &elfPhdr[ii];

    if ((BytesRead = read(fd, inBuff, sizeof(Elf32_Phdr))) < 1)
    {
      cout << "File read error (Elf32_Phdr): bytesRead " << BytesRead << endl;
      return (-5); // failure
    }
    else
    {
      memcpy(elfPhdrptr, inBuff, sizeof(Elf32_Phdr));
      if (elfPhdrptr->p_type == PT_DYNAMIC)
      {
        PT_DynamicCount++;
        if (PT_DynamicCount > 1)
        {
          cout << "ERROR: Multiple dynamic segments found! Count: " << PT_DynamicCount << endl;
          return (-1);
        }
        else
        {
          PT_Dynamic_Index = ii;
        }
      }
      else if ((elfPhdrptr->p_type == PT_LOAD) && (elfPhdrptr->p_offset == 0x0))
      {
        pt_loadAddr = elfPhdrptr->p_vaddr;
      }
    }
  }

  lseek(fd, elfhdr->e_shoff, 0);

  cout << "Start reading section header " << endl;
  for (ii = 0; ii < num_Shdrs; ii++)
  {
    elfShdrptr = &elfShdr[ii];

    if ((BytesRead = read(fd, elfShdrptr, sizeof(Elf32_Shdr))) < 1)
    {
      cout << "File read error (Elf32_Shdr): bytesRead " << BytesRead << endl;
      //      return(-6);// failure
    }
  }

  // ToDo: 1) Need to read in program header raw data blocks here
  //       2) Need to build a linked list of offsets with pointers to
  //          the sequential bytes of data

  // Read in section header raw data blocks
  // ToDo: When program header raw data block read code is complete, need to
  //       check program header info and set section raw data pointer to the
  //       raw data block in program header that matches
  /*
  for(ii = 0; ii < num_Shdrs; ii++)
  {
    bytes_2_read = 0;
    elfShdrptr = &elfShdr[ii];
    sectionRawDataptr = &sectionRawData[ii];

    sectionRawDataptr->index = ii;

    if(ii < (num_Shdrs - 1))
    {
      tempShdrptr = &elfShdr[ii + 1];
      if(elfShdrptr->sh_size > 0)
      {
  //       tempShdrptr = &elfShdr[ii + 1];
       if(tempShdrptr->sh_offset > elfShdrptr->sh_offset)
          bytes_2_read = tempShdrptr->sh_offset - elfShdrptr->sh_offset;
       else
          bytes_2_read = elfShdrptr->sh_size;
      }
      else
      {
        bytes_2_read = elfShdrptr->sh_size;
      }
    }
    else
    {
      bytes_2_read = elfShdrptr->sh_size;
    }

    if(bytes_2_read != elfShdrptr->sh_size)
    {
      sectionRawDataptr->actual_size = bytes_2_read;
      sectionRawDataptr->reported_size = elfShdrptr->sh_size;
    }
    else
    {
      sectionRawDataptr->actual_size = bytes_2_read;
      sectionRawDataptr->reported_size = elfShdrptr->sh_size;
    }

    if((sectionRawDataptr->rawData = (unsigned char *)calloc(bytes_2_read + 1, sizeof(unsigned char))) == NULL)
    {
  cout << "Memory allocation error (sectionRawData->rawData)! " << hex << bytes_2_read << dec << endl;
      //return(-4); // failure
    }
    else
    {
      sectionRawDataptr->Offset = elfShdrptr->sh_offset;
      lseek(fd, elfShdrptr->sh_offset, 0);
      if((BytesRead = read(fd, sectionRawDataptr->rawData, bytes_2_read)) < (int)bytes_2_read)
      {
    cout << "File read error (shstrTable): bytesRead " << hex << BytesRead << dec << "  Bytes expected "
             << (int)bytes_2_read << " Section number " << ii <<  endl;
    free(sectionRawDataptr->rawData);
          sectionRawDataptr->rawData = NULL;
      }
    }

    if(sectionRawDataptr->rawData != NULL)
    {
      if(elfShdrptr->sh_type == SHT_STRTAB)
      {
        if(strcmp(getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name), ".strtab") == 0)
        {
          // allocate a block of memory to hold the string table block in memory
    shstrTable = (char *)sectionRawDataptr->rawData; // point to strtab raw data
        }

        StrTableCount++;
      }
      else
      if(elfShdrptr->sh_type == SHT_SYMTAB) //SHT_STRTAB)
      {
        elfSym = (Elf32_Sym *)sectionRawDataptr->rawData; // set elfSym to point to the symtab raw data
        symtabEntries = elfShdrptr->sh_size / sizeof(Elf32_Sym); // calculate total symtab entries
      }
    }
  }
*/

  for (ii = 0; ii < num_Shdrs; ii++)
  {
    elfShdrptr = &elfShdr[ii];
    if (strcmp(getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name), ".dynamic") == 0)
    {
      hasDynamic = 1;
      break;
    }
  }

  if ((PT_DynamicCount > 0) && (hasDynamic != 0))
  {
    elfPhdrptr = &elfPhdr[PT_Dynamic_Index];
    PT_DYNAMIC_Entrycount = read_PT_Dynamic_entries(fd, elfPhdrptr->p_filesz, elfPhdrptr->p_offset);
  }

  // Process strings
  if (processStrings(fd) != 0)
    printf("String processing error!\n");

  return (0);
}

int ELFFILE::processStrings(int fd)
{
  int BytesRead = 0;
  char ch;
  char tempstr[2048] = {""};
  //  int tempoffset;
  bool foundPrintable = false;
  //  int strdatablocksize = sizeof(STRING_DATA) * 20;
  int strdatablocksize = 1;
  STRING_DATA *temp;

  stringcount = 0;
  charcount = 0;
  // stringmemblockcount = 1;

  // initialize for 1 strings
  if ((stringdataptr = (STRING_DATA *)calloc(strdatablocksize, sizeof(STRING_DATA))) == NULL)
  {
    printf("ERROR: calloc stringdataptr initialization error!\n");
    return (0);
  }
  stringdata = &stringdataptr[stringcount];
  stringdata->Offset = lseek(fd, 0, SEEK_SET); // start at beginning of file

  while ((BytesRead = read(fd, &ch, sizeof(char))) > 0) // read one char at a time
  {
    // if found printable char (0-9, a-z, A-Z, lf, cr, etc)
    if ((ch >= 0x20) && (ch <= 0x7e)) // found printable char, cr, lf
    {
      if (foundPrintable == false)
      {
        stringdata->Offset = lseek(fd, 0, SEEK_CUR) - 1; // get current file location
        foundPrintable = true;
      }
      tempstr[charcount] = ch;

      charcount++;
    }
    else if ((ch == 0x0a) || (ch == 0x0d)) // found cr, lf
    {
      if (foundPrintable == true)
      {
        tempstr[charcount] = ch;
        charcount++;
      }
    }
    else
    {
      if (foundPrintable == true)
      {
        if ((ch != 0x00) && (charcount > 1))
        {
          tempstr[charcount] = 0x00;
        }
        else if ((ch == 0x00) && (charcount > 1)) // found null so may be end of line
        {
          tempstr[charcount] = ch;
        }
        else
        {
          strcpy(tempstr, "");
          charcount = 0;
          foundPrintable = false;
          continue;
        }

        if ((stringdata->f_string = (char *)calloc(charcount + 1, sizeof(char))) == NULL)
        {
          printf("ERROR: calloc stringdata->f_string initialization error!\n");
          return (-1);
        }

        strncpy(stringdata->f_string, tempstr, strlen(tempstr));
        strcpy(tempstr, "");
        foundPrintable = false; // end of line so reset
        // reallocate when a new string is found
        stringcount++; // next string
        strdatablocksize++;

        if ((temp = (STRING_DATA *)realloc(stringdataptr, strdatablocksize * sizeof(STRING_DATA))) == NULL)
        {
          printf("ERROR!: realloc stringdataptr failed!\n");
          return (-1);
        }
        stringdataptr = temp;
        stringdata = &stringdataptr[stringcount];
        charcount = 0;
        foundPrintable = false;
      }
      else
      {
        charcount = 0;
      }
    }
  } // end while

  return (0);
}

// checkMagic(unsigned char *)
// Verifies the magic number at the beginning of an ELF file
//
// Parameters: unsigned char * (usually the elfhdr struct member e_ident)
// Returns:    0 on success (Valid ELF file)
//            -1 on failure (not a valid ELF file)
int ELFFILE::checkMagic(unsigned char *ident)
{
  int ii;
  bool found = true;

  if (ident[4] == ELFCLASS32) // 32 bit
  {
    for (ii = 0; ii < EI_NIDENT; ii++)
    {
      if (ident[ii] != ELFMAGIC32[ii])
        found = false;
    }
  }
  else if (ident[4] == ELFCLASS64) // 64 bit
  {
    for (ii = 0; ii < EI_NIDENT; ii++)
    {
      if (ident[ii] != ELFMAGIC64[ii])
        found = false;
    }
  }
  else
  {
    // invalid, not 32 or 64 bit flag found
    found = false;
  }
  /********************************************************************
   * This block is for 64 bit executable support
   * Uncomment when ready to add this capability
   * as currently program segfaults when attempting
   * to load a 64 bit executable
   ********************************************************************/
  /*
    if(found == false)
    {
      found = true;
      for(ii = 0; ii < ELFMAGICSIZE; ii++)
      {
        if(ident[ii] != ELFMAGIC64[ii])
          found = false;
      }
    }
  */
  if (found == false)
    return (-1);

  return (0);
}

// Display PE file information out to STDOUT
// Mostly for debug purposes
void ELFFILE::ELFDisplay()
{
  int ii, jj, kk;
  char TempOutStr[2048] = {" "};
  char charstr[20] = {""};

  cout << "File ID:\t\t\t\t" << elfhdr->e_ident << endl;
  cout << "File Class:\t\t\t\t" << FileClass[elfhdr->e_ident[4]] << endl;
  cout << "Data Encode:\t\t\t\t" << DataEncode[elfhdr->e_ident[5]] << endl;
  cout << "File Version:\t\t\t\t" << (int)elfhdr->e_ident[6] << endl;

  if ((elfhdr->e_type == 0xfe00) || (elfhdr->e_type == 0xfeff))
    cout << "File Type:\t\t\t\t" << FType[5] << endl;
  else if ((elfhdr->e_type == 0xff00) || (elfhdr->e_type == 0xffff))
    cout << "File Type:\t\t\t\t" << FType[6] << endl;
  else
    cout << "File Type:\t\t\t\t" << FType[elfhdr->e_type] << endl;

  cout << "Machine Type:\t\t\t\t" << Machine[elfhdr->e_machine] << endl;
  cout << "Version:\t\t\t\t" << Version[elfhdr->e_version] << endl;

  cout << "Entry Point:\t\t\t\t0x" << hex << elfhdr->e_entry << endl;
  cout << "Program Header Table's Offset:\t\t0x" << hex << elfhdr->e_phoff
       << "(" << dec << elfhdr->e_phoff << " dec) bytes" << endl;
  cout << "Section Header Table's Offset:\t\t0x" << hex << elfhdr->e_shoff
       << "(" << dec << elfhdr->e_shoff << " dec) bytes" << endl;
  cout << "Flags:\t\t\t\t\t0x" << hex << elfhdr->e_flags << endl;
  cout << "Header Size:\t\t\t\t0x" << hex << elfhdr->e_ehsize << "(" << dec
       << elfhdr->e_ehsize << " dec) bytes" << endl;
  cout << "Header Table's Entry Size:\t\t0x" << hex << elfhdr->e_phentsize << "("
       << dec << elfhdr->e_phentsize << " dec) bytes" << endl;
  cout << "Total Program Header Table Entries:\t" << elfhdr->e_phnum << endl;
  cout << "Section Header Size:\t\t\t0x" << hex << elfhdr->e_shentsize << "("
       << dec << elfhdr->e_shentsize << " dec) bytes" << endl;
  cout << "Total Section Table Entries:\t\t" << dec << elfhdr->e_shnum << endl;
  cout << "Section Header Table Index:\t\t0x" << hex << elfhdr->e_shstrndx << dec
       << " " << elfhdr->e_shstrndx << endl;

  cout << endl
       << "Program Headers:" << endl;
  cout << "#     "
       << "Type             "
       << "Offset        "
       << "VirtAddr      "
       << "PhysAddr      "
       << "FileSZ        "
       << "MemSZ         "
       << "FLG           "
       << "Align" << endl;
  for (ii = 0; ii < num_Phdrs; ii++)
  {
    elfPhdrptr = &elfPhdr[ii];
    sprintf(TempOutStr, "%-6d", ii);
    for (jj = 0; jj < 20; jj++)
    {

      if (PHSegType[jj].value == elfPhdrptr->p_type)
      {
        sprintf(TempOutStr, "%s%-15s", TempOutStr, PHSegType[jj].Type);
        break;
      }
    }

    sprintf(TempOutStr, "%-15s  0x%08x    0x%08x    0x%08x    0x%08x    0x%08x    0x%08x    0x%08x",
            TempOutStr, elfPhdrptr->p_offset, elfPhdrptr->p_vaddr, elfPhdrptr->p_paddr,
            elfPhdrptr->p_filesz, elfPhdrptr->p_memsz, elfPhdrptr->p_flags, elfPhdrptr->p_align);

    cout << TempOutStr << endl;
  }

  if (num_Shdrs > 0)
  {
    cout << endl
         << "Section Headers:" << endl;
    cout << "#     "
         << "Name                "
         << "Type                  "
         << "Addr        "
         << "Offset      "
         << "Size        "
         << "ES  "
         << "FLG  "
         << "LNK  "
         << "INF  "
         << "AAC" << endl;

    for (ii = 0; ii < num_Shdrs; ii++)
    {
      elfShdrptr = &elfShdr[ii];

      sprintf(TempOutStr, "%-6d%-20s", ii,
              getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name));

      for (jj = 0; jj < 34; jj++)
      {
        if (SHSegType[jj].value == (unsigned long)elfShdrptr->sh_type)
        {
          sprintf(TempOutStr, "%s%-20s", TempOutStr, SHSegType[jj].Type);
          break;
        }
      }
      sprintf(TempOutStr, "%s  0x%08x  0x%08x  0x%08x  %02d  %03d  %03d  %03d  %03d",
              TempOutStr, elfShdrptr->sh_addr, elfShdrptr->sh_offset, elfShdrptr->sh_size,
              elfShdrptr->sh_entsize, elfShdrptr->sh_flags, elfShdrptr->sh_link,
              elfShdrptr->sh_info, elfShdrptr->sh_addralign);

      cout << TempOutStr << endl;
    }
  }
  else
  {
    cout << endl
         << "No section headers found!" << endl;
  }

  if (get_PT_Dynamic_entrycount() > 0)
  {
    elfPhdrptr = &elfPhdr[PT_Dynamic_Index];

    cout << endl
         << "Dynamic Section at 0x" << hex << elfPhdrptr->p_offset << dec << " has "
         << get_PT_Dynamic_entrycount() << " entries:" << endl;
    cout << "Tag            Type                    Name/Value" << endl;

    for (ii = 0; ii < PT_DYNAMIC_Entrycount; ii++)
    {
      elfDynptr = &elfDyn[ii];

      for (jj = 0; jj < 74; jj++)
      {
        if ((unsigned long)elfDynptr->d_tag == DT_Tags[jj].value)
        {
          sprintf(TempOutStr, "0x%08x     %-20s", elfDynptr->d_tag, DT_Tags[jj].Type);
          break;
        }
      }
      switch (elfDynptr->d_tag)
      {
      case DT_NULL:
        sprintf(TempOutStr, "%s    0x%08x", TempOutStr, elfDynptr->d_un.d_ptr);
        break;
      case DT_PLTGOT: // display addresses
      case DT_HASH:
      case DT_STRTAB:
      case DT_SYMTAB:
      case DT_RELA:
      case DT_INIT:
      case DT_FINI:
      case DT_SYMBOLIC:
      case DT_REL:
      case DT_DEBUG:
      case DT_TEXTREL:
      case DT_JMPREL:
      case DT_VERNEED:
      case DT_VERSYM:
      case DT_BIND_NOW:
      case DT_INIT_ARRAY:
      case DT_FINI_ARRAY:
      case DT_PREINIT_ARRAY:
      case DT_FLAGS:
      case DT_PREINIT_ARRAYSZ:
        sprintf(TempOutStr, "%s    0x%08x", TempOutStr, elfDynptr->d_un.d_ptr);
        break;
      case DT_PLTRELSZ: // Display size in bytes
      case DT_RELASZ:
      case DT_RELAENT:
      case DT_STRSZ:
      case DT_SYMENT:
      case DT_RELSZ:
      case DT_RELENT:
      case DT_INIT_ARRAYSZ:
      case DT_FINI_ARRAYSZ:
        sprintf(TempOutStr, "%s    %d bytes", TempOutStr, elfDynptr->d_un.d_val);
        break;
      case DT_VERNEEDNUM: // display dec value
      case DT_ENCODING:
        sprintf(TempOutStr, "%s    %d", TempOutStr, elfDynptr->d_un.d_val);
        break;
      case DT_PLTREL:
        sprintf(TempOutStr, "%s    %s", TempOutStr, "REL");
        break;
      case DT_NEEDED: // Display strings
        sprintf(TempOutStr, "%s    Shared Library: [%s]", TempOutStr,
                getString(fd, (dt_Strtab_Address - pt_loadAddr) + elfDynptr->d_un.d_val));
        break;
      case DT_SONAME:
      case DT_RPATH:
      case DT_RUNPATH:
        sprintf(TempOutStr, "%s    %s", TempOutStr, "FIND STRING");
        break;
      default:
        sprintf(TempOutStr, "%s    0x%08x(%s)", TempOutStr, elfDynptr->d_un.d_val, "Unknown");
        break;
      }
      cout << TempOutStr << endl;
    }
  }
  else
  {
    cout << endl
         << "No Dynamic Sections found." << endl
         << endl;
  }

  lseek(fd, elfhdr->e_shoff, 0);

  for (ii = 0; ii < num_Shdrs; ii++)
  {
    elfShdrptr = &elfShdr[ii];

    // Get the interpreter string, maybe move this to display
    // when section headers are displayed
    if (getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name) != NULL)
    {
      if ((elfShdrptr->sh_type == SHT_PROGBITS) &&
          (strcmp(getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name), ".interp") == 0)) // SHT_STRTAB)
      {
        sprintf(TempOutStr, "Interpreter:  %-20s\n", getString(fd, elfShdrptr->sh_offset));
        cout << endl
             << TempOutStr << endl;
      }
    }

    // Symbol table info
    if (elfShdrptr->sh_type == SHT_SYMTAB) // SHT_STRTAB)
    {
      cout << "Section " << getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name)
           << " contains " << symtabEntries << " Entries: " << endl;
      cout << "num:\tValue\t\tSize\tInfo\tOther\tSHINDX\tName" << endl;
      for (kk = 0; kk < symtabEntries; kk++)
      {
        elfSymptr = &elfSym[kk];
        if (elfSymptr->st_name == 0)
          sprintf(TempOutStr, "%d\t0x%08x\t%d\t%d\t%d\t%d\t%d", kk, elfSymptr->st_value,
                  elfSymptr->st_size, elfSymptr->st_info, elfSymptr->st_other,
                  elfSymptr->st_shndx, elfSymptr->st_name);
        else
          sprintf(TempOutStr, "%d\t0x%08x\t%d\t%d\t%d\t%d\t%d\t%s", kk, elfSymptr->st_value,
                  elfSymptr->st_size, elfSymptr->st_info, elfSymptr->st_other,
                  elfSymptr->st_shndx, elfSymptr->st_name, getString(shstrTable, elfSymptr->st_name));
        cout << TempOutStr << endl;
      }
    }
  }

  for (ii = 0; ii < num_Shdrs; ii++)
  {
    sectionRawDataptr = &sectionRawData[ii];
    cout << dec << "\n\nSection Index: " << sectionRawDataptr->index << " Address: 0x" << hex << sectionRawDataptr->Offset
         << "  Reported Size:  " << sectionRawDataptr->reported_size << "    Actual Size: " << sectionRawDataptr->actual_size << endl;

    strcpy(charstr, " ");
    for (jj = 0; jj < sectionRawDataptr->actual_size; jj++)
    {
      printf("%02x ", sectionRawDataptr->rawData[jj]);

      if ((sectionRawDataptr->rawData[jj] < 0x20) || (sectionRawDataptr->rawData[jj] > 0x7e))
      {
        sprintf(charstr, "%s%c", charstr, '.');
      }
      else
      {
        sprintf(charstr, "%s%c", charstr, sectionRawDataptr->rawData[jj]);
      }

      if ((jj > 0) && (jj < 16))
      {

        if (jj % 15 == 0)
        {
          printf("   %s", charstr);
          strcpy(charstr, " ");
          cout << endl;
        }
      }
      else
      {
        if ((jj + 1) % 16 == 0)
        {
          printf("   %s", charstr);
          strcpy(charstr, " ");
          cout << endl;
        }
      }
    }

    if ((jj % 16) != 0)
    {
      // pad beginning with spaces
      char tmp[512] = {""};
      for (kk = 0; kk < (16 - (jj % 16)); kk++)
      {
        sprintf(tmp, "%s   ", tmp);
      }
      //      printf("222   %s", charstr);
      printf("%s", tmp);
      printf("   %s", charstr);
      //      cout << endl;
      //      printf("jj = %d  jj%16   %d\n", jj, jj%16);
    }

    cout << endl;
  }
}

// Public function that returns the number of dynamic section entries found and processed
int ELFFILE::get_PT_Dynamic_entrycount()
{
  return (PT_DYNAMIC_Entrycount);
}

// Public function that returns the number of dynamic section entries found and processed
unsigned int ELFFILE::get_PT_Dynamic_offset()
{
  elfPhdrptr = &elfPhdr[PT_Dynamic_Index];
  return (elfPhdrptr->p_offset);
}

char *ELFFILE::get_PT_Dynamic_dtag(int index)
{
  elfDynptr = &elfDyn[index];
  sprintf(tempstr2, "%08x", elfDynptr->d_tag);
  return (tempstr2);
}

const char *ELFFILE::get_PT_Dynamic_dtagtype(int index)
{
  int jj;
  elfDynptr = &elfDyn[index];

  for (jj = 0; jj < 74; jj++)
  {
    if ((unsigned long)elfDynptr->d_tag == DT_Tags[jj].value)
    {
      sprintf(tempstr2, "%s", DT_Tags[jj].Type);
      return (tempstr2);
    }
  }

  return ((const char *)"NULL");
}

char *ELFFILE::get_PT_Dynamic_dtagNVal(int index)
{
  elfDynptr = &elfDyn[index];

  switch (elfDynptr->d_tag)
  {
  case DT_NULL:
    sprintf(tempstr2, "0x%08x", elfDynptr->d_un.d_ptr);
    break;
  case DT_PLTGOT: // display addresses
  case DT_HASH:
  case DT_STRTAB:
  case DT_SYMTAB:
  case DT_RELA:
  case DT_INIT:
  case DT_FINI:
  case DT_SYMBOLIC:
  case DT_REL:
  case DT_DEBUG:
  case DT_TEXTREL:
  case DT_JMPREL:
  case DT_VERNEED:
  case DT_VERSYM:
  case DT_BIND_NOW:
  case DT_INIT_ARRAY:
  case DT_FINI_ARRAY:
  case DT_PREINIT_ARRAY:
  case DT_FLAGS:
  case DT_PREINIT_ARRAYSZ:
    sprintf(tempstr2, "0x%08x", elfDynptr->d_un.d_ptr);
    break;
  case DT_PLTRELSZ: // Display size in bytes
  case DT_RELASZ:
  case DT_RELAENT:
  case DT_STRSZ:
  case DT_SYMENT:
  case DT_RELSZ:
  case DT_RELENT:
  case DT_INIT_ARRAYSZ:
  case DT_FINI_ARRAYSZ:
    sprintf(tempstr2, "%d bytes", elfDynptr->d_un.d_val);
    break;
  case DT_VERNEEDNUM: // display dec value
  case DT_ENCODING:
    sprintf(tempstr2, "%d", elfDynptr->d_un.d_val);
    break;
  case DT_PLTREL:
    sprintf(tempstr2, "%s", "REL");
    break;
  case DT_NEEDED: // Display strings
    sprintf(tempstr2, "Shared Library: [%s]", getString(fd, (dt_Strtab_Address - pt_loadAddr) + elfDynptr->d_un.d_val));
    break;
  case DT_SONAME:
  case DT_RPATH:
  case DT_RUNPATH:
    sprintf(tempstr2, "%s", "FIND STRING");
    break;
  default:
    sprintf(tempstr2, "0x%08x(%s)", elfDynptr->d_un.d_val, "Unknown");
    break;
  }

  return (tempstr2);
}
// Sets up and populates the dynamic section info structure and
// returns the number of entries
// This is used during elf file processing and only needs to be
// called once
// Private function that is called by the ELFRead function
int ELFFILE::read_PT_Dynamic_entries(int fd, int fsize, int offset)
{
  int count = 0;
  int alloc_size = 0;
  int BytesRead = 0;

  if (fsize > 0)
  {
    alloc_size = fsize / sizeof(Elf32_Dyn);
    printf("alloc_size %d  fsize  %d  sizeof(Elf32_Dyn)  %d\n", alloc_size, fsize, sizeof(Elf32_Dyn));
    if ((elfDyn = (Elf32_Dyn *)calloc(alloc_size, sizeof(Elf32_Dyn))) == NULL)
    {
      cout << "ERROR: read_PT_Dynamic_entries() memory allocation of elfDyn!" << endl;
      return (-1);
    }

    lseek(fd, offset, 0);
    do
    {
      elfDynptr = &elfDyn[count];
      if ((BytesRead = read(fd, elfDynptr, sizeof(Elf32_Dyn))) < 1)
      {
        cout << "File read error in get_PT_Dynamic_entrycount: bytesRead " << BytesRead << endl;
        return (-1); // failure
      }
      else
      {
        if (elfDynptr->d_tag == DT_STRTAB)
        {
          dt_Strtab_Address = elfDynptr->d_un.d_ptr;
        }
        count++;
      }
      //     printf("elfDynptr->d_tag: %08x  %08x  %08x\n", elfDynptr->d_tag, elfDynptr->d_un.d_ptr, elfDynptr->d_un.d_val);
    } while ((elfDynptr->d_tag != 0));
  }
  else
  {
    cout << "ERROR: get_PT_Dynamic_entrycount Section size < 0!" << endl;
    return (-1);
  }

  return (count);
}

const char *ELFFILE::get_symtab_String_Val()
{
  int ii;
  for (ii = 0; ii < num_Shdrs; ii++)
  {
    elfShdrptr = &elfShdr[ii];
    if (elfShdrptr->sh_type == SHT_SYMTAB) // SHT_STRTAB)
    {
      return (getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name));
    }
  }
  return ("NULL");
}

const char *ELFFILE::get_Interpreter_String_Val()
{
  int ii;
  for (ii = 0; ii < num_Shdrs; ii++)
  {
    elfShdrptr = &elfShdr[ii];

    // Get the interpreter string, maybe move this to display
    // when section headers are displayed
    if (getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name) != NULL)
    {
      if ((elfShdrptr->sh_type == SHT_PROGBITS) &&
          (strcmp(getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name), ".interp") == 0)) // SHT_STRTAB)
      {
        return (getString(fd, elfShdrptr->sh_offset));
      }
    }
  }
  return ("NULL");
}

int ELFFILE::get_Symtable_EntryCount()
{
  return (symtabEntries);
}

char *ELFFILE::get_SYMTAB_Value(int index)
{
  elfSymptr = &elfSym[index];
  sprintf(tempstr2, "%08x", elfSymptr->st_value);
  return (tempstr2);
}

char *ELFFILE::get_SYMTAB_Size(int index)
{
  elfSymptr = &elfSym[index];
  sprintf(tempstr2, "%08x", elfSymptr->st_size);
  return (tempstr2);
}

char *ELFFILE::get_SYMTAB_Bind(int index) // st_info >> 4
{
  elfSymptr = &elfSym[index];
  switch (elfSymptr->st_info >> 4)
  {
  case STB_LOCAL:
    sprintf(tempstr2, "%s", "STB_LOCAL");
    break;
  case STB_GLOBAL:
    sprintf(tempstr2, "%s", "STB_GLOBAL");
    break;
  case STB_WEAK:
    sprintf(tempstr2, "%s", "STB_WEAK");
    break;
  case STB_LOPROC:
    sprintf(tempstr2, "%s", "STB_LOPROC");
    break;
  case STB_HIPROC:
    sprintf(tempstr2, "%s", "STB_HIPROC");
    break;
  default:
    sprintf(tempstr2, "%08x", elfSymptr->st_info);
    break;
  }
  return (tempstr2);
}

char *ELFFILE::get_SYMTAB_Type(int index) // st_info & 0xf
{
  elfSymptr = &elfSym[index];
  switch (elfSymptr->st_info & 0xf)
  {
  case STT_NOTYPE:
    sprintf(tempstr2, "%s", "STT_NOTYPE");
    break;
  case STT_OBJECT:
    sprintf(tempstr2, "%s", "STT_OBJECT");
    break;
  case STT_FUNC:
    sprintf(tempstr2, "%s", "STT_FUNC");
    break;
  case STT_SECTION:
    sprintf(tempstr2, "%s", "STT_SECTION");
    break;
  case STT_FILE:
    sprintf(tempstr2, "%s", "STT_FILE");
    break;
  case STT_LOPROC:
    sprintf(tempstr2, "%s", "STT_LOPROC");
    break;
  case STT_HIPROC:
    sprintf(tempstr2, "%s", "STT_HIPROC");
    break;
  default:
    sprintf(tempstr2, "%08x", elfSymptr->st_info);
    break;
  }

  return (tempstr2);
}

const char *ELFFILE::get_SYMTAB_SHIndex(int index)
{
  elfSymptr = &elfSym[index];
  if (elfSymptr->st_shndx == SHN_UNDEF)
  {
    return ("*UND*");
  }
  else if (elfSymptr->st_shndx == SHN_ABS)
  {
    //    sprintf(tempstr2, "%08x",  elfSymptr->st_shndx);
    return ("*UND*");
  }
  else if (elfSymptr->st_shndx == SHN_COMMON)
  {
    //    sprintf(tempstr2, "%08x",  elfSymptr->st_shndx);
    return ("*COMMON*");
  }
  return (get_SH_name(elfSymptr->st_shndx));
}

const char *ELFFILE::get_SYMTAB_Name(int index)
{
  elfSymptr = &elfSym[index];
  if (elfSymptr->st_name == 0)
  {
    if (elfSymptr->st_shndx != 0)
    {
      return (get_SH_name(elfSymptr->st_shndx));
    }
    else
      return ("0");
  }
  return (getString(shstrTable, elfSymptr->st_name));
}

// getString(int file, int offset)
// Reads a 'file' at 'offset' until a NULL character is found
// and returns the string found
const char *ELFFILE::getString(int fd, int offset)
{
  char c;
  int BytesRead = 0;
  int count = 0;

  lseek(fd, offset, 0);
  do
  {
    if ((BytesRead = read(fd, &c, sizeof(char))) < 1)
    {
      cout << "File read error (Elf32_Shdr string table): bytesRead " << BytesRead << endl;
      return ("NULL"); // failure
    }
    else
    {
      tempstr[count] = c;
      count++;
    }
  } while (c != '\0');

  tempstr[count] = '\0';
  return (tempstr);
}

// getString(int file, int offset)
// Reads a 'buff' at 'offset' until a NULL character is found
// and returns the string found
const char *ELFFILE::getString(char *buff, int offset)
{
  int count = 0;

  //  strcpy(tempstr, " ");
  do
  {
    tempstr[count] = buff[offset];
    count++;
    offset++;
  } while (buff[offset] != '\0');

  tempstr[count] = '\0';
  return (tempstr);
}

// return a pointer to the elf header struct
Elf32_Ehdr *ELFFILE::get_ELFhdrptr()
{
  return (elfhdr);
}

// return file ID
char *ELFFILE::get_ID()
{
  return ((char *)elfhdr->e_ident);
}

// return executable type
const char *ELFFILE::get_Class()
{
  return (FileClass[elfhdr->e_ident[4]]);
}

// return encode type
const char *ELFFILE::get_EncodeType()
{
  return (DataEncode[elfhdr->e_ident[5]]);
}

// return file version
char *ELFFILE::get_File_Version()
{
  sprintf(tempstr, "%d", (int)elfhdr->e_ident[6]);
  return (tempstr);
}

// return file type
const char *ELFFILE::get_File_Type()
{
  if ((elfhdr->e_type == 0xfe00) || (elfhdr->e_type == 0xfeff))
    return (FType[5]);
  else if ((elfhdr->e_type == 0xff00) || (elfhdr->e_type == 0xffff))
    return (FType[6]);

  return (FType[elfhdr->e_type]);
}

// return machine type
const char *ELFFILE::get_Machine_Type()
{
  return (Machine[elfhdr->e_machine]);
}

// return version
const char *ELFFILE::get_Version()
{
  return (Version[elfhdr->e_version]);
}

// return program entry point
char *ELFFILE::get_Entry_Point()
{
  sprintf(tempstr, "0x%08x", elfhdr->e_entry);
  return (tempstr);
}

// set program entry point
void ELFFILE::set_Entry_Point(unsigned int ep)
{
  elfhdr->e_entry = ep;
  cout << "Set Entry Point:" << endl;
}

// return Program Header Table's Offset
char *ELFFILE::get_PHDRT_Offset()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_phoff, elfhdr->e_phoff);
  return (tempstr);
}

// Section Header Table's Offset
char *ELFFILE::get_SHDRT_Offset()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_shoff, elfhdr->e_shoff);
  return (tempstr);
}

// return flags
char *ELFFILE::get_Flags()
{
  sprintf(tempstr, "0x%08x", elfhdr->e_flags);
  return (tempstr);
}

// return ELF header size
char *ELFFILE::get_HDR_Size()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_ehsize, elfhdr->e_ehsize);
  return (tempstr);
}

// returns the Header table's entry size
char *ELFFILE::get_HDRTE_Size()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_phentsize, elfhdr->e_phentsize);
  return (tempstr);
}

// return Total Program Header Table Entries
char *ELFFILE::get_TPHDR_Entries()
{
  sprintf(tempstr, "%d", elfhdr->e_phnum);
  return (tempstr);
}

// return Section Header Size
char *ELFFILE::get_SHDR_Size()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_shentsize, elfhdr->e_shentsize);
  return (tempstr);
}

// return Total Section Table Entries
char *ELFFILE::get_TST_Entries()
{
  sprintf(tempstr, "%d", elfhdr->e_shnum);
  return (tempstr);
}

// return index to .shstrtab section in section header table array
char *ELFFILE::get_SHDRT_Index()
{
  sprintf(tempstr, "0x%x (%d Dec) bytes", elfhdr->e_shstrndx, elfhdr->e_shstrndx);
  return (tempstr);
}

Elf32_Phdr *ELFFILE::get_PHDRptr()
{
  return (elfPhdr);
}

int ELFFILE::get_PHDR_Count()
{
  return (num_Phdrs);
}

const char *ELFFILE::get_PHSegType(int index)
{
  int jj;
  elfPhdrptr = &elfPhdr[index];

  for (jj = 0; jj < 20; jj++)
  {
    if (PHSegType[jj].value == elfPhdrptr->p_type)
    {
      // sprintf(TempOutStr, "%s%-15s", TempOutStr, PHSegType[jj].Type);
      return (PHSegType[jj].Type);
    }
  }
  return ("NULL");
}

char *ELFFILE::get_PH_offset(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "0x%08x", elfPhdrptr->p_offset);
  return (tempstr);
}

char *ELFFILE::get_PH_vaddress(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "0x%08x", elfPhdrptr->p_vaddr);
  return (tempstr);
}

char *ELFFILE::get_PH_physaddress(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "0x%08x", elfPhdrptr->p_paddr);
  return (tempstr);
}

char *ELFFILE::get_PH_filesz(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "0x%08x", elfPhdrptr->p_filesz);
  return (tempstr);
}

char *ELFFILE::get_PH_memsz(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "0x%08x", elfPhdrptr->p_memsz);
  return (tempstr);
}

char *ELFFILE::get_PH_flags(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "%04d", elfPhdrptr->p_flags);
  return (tempstr);
}

char *ELFFILE::get_PH_align(int index)
{
  elfPhdrptr = &elfPhdr[index];
  sprintf(tempstr, "%04d", elfPhdrptr->p_align);
  return (tempstr);
}

int ELFFILE::get_SHDR_Count()
{
  // cout << "section header count " << num_Shdrs << endl;
  return (num_Shdrs);
}

char *ELFFILE::get_SH_name(int index)
{
  elfShdrptr = &elfShdr[index];
  // printf("get_SH_name  fd = %d\n", fd);
  sprintf(tempstr, "%s", getString(fd, elfShdr[elfhdr->e_shstrndx].sh_offset + elfShdrptr->sh_name));
  // printf("get_SH_name  %s\n", tempstr);
  return (tempstr);
}

const char *ELFFILE::get_SHSegType(int index)
{
  int jj;
  elfShdrptr = &elfShdr[index];

  for (jj = 0; jj < 34; jj++)
  {
    if (SHSegType[jj].value == elfShdrptr->sh_type)
    {
      // sprintf(TempOutStr, "%s%-15s", TempOutStr, PHSegType[jj].Type);
      //     printf("jj: %d, type: %s,  sh_type:  0x%08x\n", jj, elfShdrptr->sh_type, SHSegType[jj].value);
      return (SHSegType[jj].Type);
    }
  }
  return ("NULL");
}

char *ELFFILE::get_SH_addr(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "0x%08x", elfShdrptr->sh_addr);
  return (tempstr);
}

char *ELFFILE::get_SH_offset(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "0x%08x", elfShdrptr->sh_offset);
  return (tempstr);
}

char *ELFFILE::get_SH_size(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "0x%08x", elfShdrptr->sh_size);
  return (tempstr);
}

char *ELFFILE::get_SH_entsize(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "%04d", elfShdrptr->sh_entsize);
  return (tempstr);
}

char *ELFFILE::get_SH_flags(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "%04d", elfShdrptr->sh_flags);
  return (tempstr);
}

char *ELFFILE::get_SH_link(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "%04d", elfShdrptr->sh_link);
  return (tempstr);
}

char *ELFFILE::get_SH_info(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "%04d", elfShdrptr->sh_info);
  return (tempstr);
}

char *ELFFILE::get_SH_addralign(int index)
{
  elfShdrptr = &elfShdr[index];
  sprintf(tempstr, "%04d", elfShdrptr->sh_addralign);
  return (tempstr);
}

bool ELFFILE::get_IsDirty()
{
  return (isDirty);
}

void ELFFILE::set_IsDirty(bool val)
{
  isDirty = val;
}

// Save modifications to file fName
int ELFFILE::save_File_as(char *fName)
{
  int temp_fd;
  int bytes_written = 0;
  int bytes_read = 0;
  char buff[2052];

  printf("saving file: %s\n", fName);

  if ((temp_fd = open(fName, O_RDWR, 0)) <= 0)
  {
    cout << "Creating file:  " << fName << endl;
    temp_fd = creat(fName, 0777); // open  read write execute for all
  }
  // else file exists, overwrite

  // this section is temporary and will be modified
  // into a better file saving solution
  // this solution will work for demo purpose for now
  bytes_written = write(temp_fd, elfhdr, sizeof(Elf32_Ehdr));
  printf("bytes written %d\n", bytes_written);
  // copy rest of file
  lseek(fd, sizeof(Elf32_Ehdr), 0);
  while ((bytes_read = read(fd, buff, 2048)) > 0)
  {
    bytes_written = write(temp_fd, buff, bytes_read);
  }

  // if not exiting program, fd = temp_fd
  // set fd to working file
  // if exiting close file
  set_IsDirty(false); // if file saved, it isn't dirty anymore
  close(temp_fd);
  return (0); // success
}

int ELFFILE::get_StringOffset(int index)
{
  stringdata = &stringdataptr[index];
  // if(stringdata == NULL)
  //   printf("index %d  [1  OOOOOPS]\n", index);
  // if(stringdata->Offset == NULL)
  //   printf("index %d  [2  OOOOOPS]\n", index);
  // else
  //  printf("index: %d stringdata->Offset: %08x\n", index, stringdata->Offset);
  return (stringdata->Offset);
}

char *ELFFILE::get_StringVal(int index)
{
  stringdata = &stringdataptr[index];
  //  if(stringdata == NULL)
  //  printf("index: %d  [3  OOOOOPS]\n", index);
  // if(stringdata->f_string == NULL)
  //  printf("index: %d  [4  OOOOOPS]\n", index);
  // else
  // printf("index: %d stringdata->f_string: %s\n", index, stringdata->f_string);
  //  getchar();
  return (stringdata->f_string);
}

int ELFFILE::get_StringCount()
{
  return (stringcount);
}

int ELFFILE::get_fd()
{
  return (fd);
}
