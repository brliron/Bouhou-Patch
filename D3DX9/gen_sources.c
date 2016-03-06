#include	<windows.h>
#include	<stdio.h>

HMODULE	load_module(LPCSTR name);
int	print_exports(HMODULE hLib);
LPCSTR*	exports_to_tab(HMODULE hLib);

void	write_data(FILE* fd, const char** exports)
{
  char	address_name[100];
  char	name_name[100];

  fprintf(fd, "section .data\n");
  while (*exports)
    {
      strcpy(address_name, *exports);
      strcat(address_name, "Address");
      strcpy(name_name, *exports);
      strcat(name_name, "Name");
      fprintf(fd,
	      "%s:\tdd\t0\n"
	      "%s:\tdb\t\"%s\", 0\n",
	      address_name, name_name, *exports);
      exports++;
    }
  fprintf(fd, "\n");
}

void	write_code_base(FILE* fd)
{
  fprintf(fd,
	  "section .text\n"
	  "global _DllMain\n"
	  "_DllMain:\n"
	  "\tmov\teax, 1\n"
	  "\tret\t12\n\n"
	  "extern _FindEntry@8\n\n");
}

void	write_code(FILE* fd, const char** exports)
{
  char	symbol_name[100];
  char	address_name[100];
  char	name_name[100];
  char	jmp_name[100];

  write_code_base(fd);
  symbol_name[0] = '_';
  while (*exports)
    {
      strcpy(symbol_name + 1, *exports);
      strcpy(address_name, *exports);
      strcat(address_name, "Address");
      strcpy(name_name, *exports);
      strcat(name_name, "Name");
      strcpy(jmp_name, *exports);
      strcat(jmp_name, "_jmp");
      fprintf(fd,
	      "global %s\n"
	      "%s:\n"
	      "\tcmp\t[dword %s], dword 0\n"
	      "\tjne\t%s\n"
	      "\tpush\t%s\n"
	      "\tpush\t%s\n"
	      "\tcall\t_FindEntry@8\n"
	      "%s:\n"
	      "\tjmp\t[%s]\n\n",
	      symbol_name, symbol_name, address_name, jmp_name,
	      address_name, name_name, jmp_name, address_name);
      exports++;
    }
}

void	write_def(FILE* fd, const char** exports)
{
  fprintf(fd,
	  "LIBRARY \"" LIB_NAME ".dll\" BASE=0x10000000\n\n"
	  "EXPORTS\n");

  while (*exports)
    {
      fprintf(fd, "%s\n", *exports);
      exports++;
    }
}

int		main()
{
  const char**	exports;
  FILE*		asm_file;
  FILE*		def_file;

  exports = exports_to_tab(load_module(LIB_NAME ".dll"));
  asm_file = fopen(LIB_NAME ".asm", "w");
  def_file = fopen(LIB_NAME ".def", "w");

  write_data(asm_file, exports);
  write_code(asm_file, exports);
  write_def(def_file, exports);

  fclose(asm_file);
  fclose(def_file);
  free(exports);
  return (0);
}
