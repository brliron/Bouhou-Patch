#include	<stdio.h>
#include	<string.h>

#define	CLASS_NAME	"IDirect3DSurface9"
#define	TRACE_CALLS

int	my_cpy(char *dest, const char *src, char delim)
{
  int	i;

  i = 0;
  while (*src != delim)
    {
      *dest = *src;
      dest++;
      src++;
      i++;
    }
  *dest = '\0';
  return (i + 1);
}

void	parse_line(char *line, FILE *out)
{
  char	*ptr;
  char	ret[255];
  char	name[255];
  char	params_ret[10][255];
  char	params[10][255];
  int	i;

  if (strstr(line, "STDMETHOD_"))
    {
      ptr = strstr(line, "STDMETHOD_") + strlen("STDMETHOD_") + 1;
      ptr += my_cpy(ret, ptr, ',');
      if (*ptr == ' ')
        ptr++;
    }
  else
    {
      strcpy(ret, "HRESULT");
      ptr = strstr(line, "STDMETHOD") + strlen("STDMETHOD") + 1;
    }
  ptr += my_cpy(name, ptr, ')') + 1;

  if (strncmp(ptr, "THIS_", 5) != 0)
    params[0][0] = '\0';
  else
    {
      ptr += strlen("THIS_ ");
      i = 0;
      while (strstr(ptr, ")"))
        {
	  ptr += my_cpy(params_ret[i], ptr, ' ');
	  while (strstr(ptr, " ") && (strstr(ptr, " ") < strstr(ptr, ",") || (strstr(ptr, ",") == NULL && strstr(ptr, " ") < strstr(ptr, ")"))))
	    {
	      strcat(params_ret[i], " ");
	      ptr += my_cpy(params_ret[i] + strlen(params_ret[i]), ptr, ' ');
	    }
	  while (*ptr == '*')
	    {
	      strcat(params_ret[i], "*");
	      ptr++;
	    }
	  ptr += my_cpy(params[i], ptr, strstr(ptr, ",") ? ',' : ')');
	  if (*ptr == ' ')
	    ptr++;
	  i++;
	}
      params[i][0] = '\0';
    }

  /* Write string */
  fprintf(out, "%s\t%s::%s(", ret, CLASS_NAME, name);
  i = 0;
  while (params[i][0])
    {
      fprintf(out, "%s%s %s", i == 0 ? "" : ", ", params_ret[i], params[i]);
      i++;
    }
  fprintf(out, ")\n"
	       "{\n"
#ifdef TRACE_CALLS
	       "  printf(\"%s\\n\");\n"
#endif /* TRACE_CALLS */
	       "  return orig->%s(",
#ifdef TRACE_CALLS
	       name,
#endif /* TRACE_CALLS */
	       name);
  i = 0;
  while (params[i][0])
    {
      fprintf(out, "%s%s", i == 0 ? "" : ", ", params[i]);
      i++;
    }
  fprintf(out, ");\n"
	       "}\n\n");
}

int	main()
{
  FILE	*in;
  FILE	*out;
  char	in_str[65536];
  char	*line_ptr;
  char	line[256];

  in = fopen("input.cpp", "r");
  out = fopen("output.cpp", "w");
  if (in == NULL || out == NULL)
    {
      perror("fopen");
      return (1);
    }
  fread(in_str, 65535, 1, in);
  fclose(in);
  line_ptr = strtok(in_str, "\n");
  do
    {
      if (strstr(line_ptr, "STDMETHOD"))
        {
	  strcpy(line, line_ptr);
	  parse_line(line, out);
	}
    }
  while ((line_ptr = strtok(NULL, "\n")));
  fclose(in);
  fclose(out);
  return (0);
}
