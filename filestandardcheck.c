#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>

#include "standards.c"

/*

Checks for portability problems.
Copyright (C) 2024 cb-bugman

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see
<https://www.gnu.org/licenses/>.
*/


#define NA 0
#define BLACKLIST 0
#define WHITELIST 1

/* Holds standards' limits and requirements. Write NA where numerical limits are non-applicable.
   If a standard does not explicitly disallow a feature, then it is considered to be allowed.
   Therefore: All bools determining if a feature is allowed or not must be TRUE or FALSE.
 */

struct standard_definition;
typedef struct standard_definition standard_definition;
struct standard_definition GLOBAL_STANDARDS[];




/* Creates STANDARD from parameters.
 Returns struct on success, NULL on failure.*/
standard_definition
*standard_create(char*      name,               
		 char*      charlist,            
		 u_int64_t  size_limit,  		
		 u_int64_t  max_file_count,	      
		 u_int64_t  component_len,	     
		 u_int64_t  path_len,		       
		 bool       are_duplicates_allowed,    
		 bool       are_symlinks_allowed,      
		 bool       are_hardlinks_allowed,     
		 bool       charlist_type)
{
  standard_definition *standard = malloc(sizeof(standard_definition));

  char* name_copy = malloc((strlen(name) * sizeof(char)) + 1);
  strcpy(name_copy, name);
  standard->name = name_copy;

  char* charlist_copy = malloc((strlen(charlist) * sizeof(char)) + 1);
  strcpy(charlist_copy, charlist);
  standard->charlist = charlist_copy;

  standard->size_limit = size_limit;
  standard->max_file_count = max_file_count;
  standard->component_len = component_len;
  standard->path_len = path_len;

  standard->are_duplicates_allowed = are_duplicates_allowed;
  standard->are_hardlinks_allowed = are_hardlinks_allowed;
  standard->charlist_type = charlist_type;

  return standard;
}




/* If a standard was ever violated by any file during the programs execution.
 This determines the programs return code. */
bool WAS_STANDARD_VIOLATION_ENCOUNTERED = false; 

/* Delimiter for use in error logging. */
char *DELIMITER = ":";


/* List of errors the program is capable of detecting and reporting. */
enum ERROR_CODES
  {
    ERR_PATHLEN,
    ERR_COMPLEN,
    ERR_TOOBIG,
    ERR_BADNAME,
    ERR_DUPE,
    ERR_FILECOUNT,
    ERR_PERMS,
    ERR_NOFILE,
    ERR_HARDLINK,
    ERR_SYMLINK,
  };

/* String versions of ERROR_CODES. */
const char *ERROR_STRINGS[] =
{
 "ERR_PATHLEN",
 "ERR_COMPLEN",
 "ERR_TOOBIG",
 "ERR_BADNAME",
 "ERR_DUPE",
 "ERR_FILECOUNT",
 "ERR_PERMS",
 "ERR_NOFILE",
 "ERR_HARDLINK",
 "ERR_SYMLINK"
};


void
program_opts_print_short_help()
{
  printf(
	 "Usage: [OPTION]... [FILE]...\n"
	 "Check if file would be portable to a selected standard. Defaults to POSIX.\n"
	 "Try standardcheck --help for more information.\n"
	 );
}


/* Prints error to stdout.*/
static inline void
log_error(const char* standard, int error_code, char* file_path)
{
  printf("%s%s%s%s\"%s\"\n",
	 ERROR_STRINGS[error_code], DELIMITER, standard, DELIMITER, file_path );
  WAS_STANDARD_VIOLATION_ENCOUNTERED = true;
  return;
}

/* Retrives standard by name. Returns pointer to matching standard_definition
 Exits program if specified standard cannot be found.   */
standard_definition *standard_retrieve_by_name(char* name_to_search, standard_definition *standard_list)
{
  int i = 0;
  while(standard_list[i].name != NULL)
    {
      if ( strcasecmp(standard_list[i].name, name_to_search) == 0 )
	return &standard_list[i];
      else
	i++;
    }

  printf("Invalid standard: %s\n", name_to_search);
  program_opts_print_short_help();
  exit(-1);
}

static inline int
check_path_length(char* file_path, u_int16_t max_length)
{
  if (strlen(file_path) > max_length)
    return -1;
  return 0;
}


/* Checks if a component of FILE_PATH is longer than max_length 
 Returns 0 on success, -1 on failure.  */
static inline int
check_component_length(char* file_path, u_int16_t max_length)
{
  char* component;
  // strtok modifies strings, must make a copy otherwise nasty errors occur.
  // due to strcpy semantics, we cant really use the stack for this either.
  char* file_path_copy = malloc((strlen(file_path) * sizeof(char)) + 1);
  strcpy(file_path_copy, file_path);
  component = strtok(file_path_copy, "/");

  assert(component != NULL);
  
  if ( strlen(component) > max_length)
    {
      free(file_path_copy);
      return -1;
    }

  while(strtok(NULL, "/") != NULL)
    {
      if ( strlen(component) > max_length)
	{
	  free(file_path_copy);
	  return -1;
	}
    }
  free(file_path_copy);
  return 0;
}


/* Checks if file at FILE_PATH violates CHARLIST, checking performed in MODE whitelist or blacklist.
 Returns 0 on success, -1 on failure.  */
static inline int
check_path_validity(char* file_path, const char* charlist, bool mode)
{
  int i = 0;
  int k = 0;
  if (mode == BLACKLIST)
    while( file_path[i] != 0 )
      {
	while( charlist[k] != 0 )
	  {
	    if ( file_path[i] == charlist[k] )
	      return -1;
	    else
	      k++;
	  }
	k = 0;
	i++;
      }

  if (mode == WHITELIST) // FIXME: is there no better wayto implement a whitelist?
    {
      bool good_char_found;
      while ( file_path[i] != 0 )
	{
	  good_char_found = false;
	  while ( charlist[k] != 0 )
	    {
	      if ( file_path[i] == charlist[k] )
		{
		  good_char_found = true;
		  break;
		}
	      else
		k++;
	    }
	  if ( good_char_found == false )
	    {
	      return -1;
	    }
	  i++;
	  k = 0;
	}
      return 0;
    }
  return 0;
}


/* Checks if file at FILE_PATH is larger than MAX_SIZE.
 Returns 0 on success, -1 on failure.  */
static inline int
check_file_size(char* file_path, uint64_t max_size)
{
  struct stat stat_buffer;

  if ( stat(file_path, &stat_buffer) == -1)
    ; //FIXME: handle error

  if ( stat_buffer.st_size > max_size )
    return -1;
  
  return 0;
}

/* Checks if directory at DIR_PATH contains more files than MAX_FILES.
   Returns 0 on success, -1 on failure.  */
static inline int
check_directory_file_count(char* dir_path, uint64_t max_files)
{
  uint64_t file_count = 0;
  DIR *dirp;
  struct dirent *entry;

  dirp = opendir(dir_path);
  if ( dirp == NULL )
    {
      log_error("SYSTEM", ERR_NOFILE, dir_path);
      return 0;
    }

  while ((entry = readdir(dirp)) != NULL)
    {
      if(entry->d_type == DT_REG)
	file_count++;
      if (file_count > max_files)
	return -1;
    }
  return 0;
}


/* Checks FILE_PATH against selected STANDARD. */
void
standard_check_file_compliance(char* file_path, standard_definition standard )
{
  struct stat stat_buf;
  lstat(file_path, &stat_buf);

  if ( standard.path_len > 0 )
    if ( check_path_length(file_path, standard.path_len) == -1 )
      log_error(standard.name, ERR_PATHLEN, file_path);
  
  if ( standard.component_len > 0 )
    if ( check_component_length(file_path, standard.component_len) == -1 )
      log_error(standard.name, ERR_COMPLEN, file_path);
  
  if ( standard.charlist != NULL )
    if ( check_path_validity(file_path, standard.charlist, standard.charlist_type) == -1 )
      log_error(standard.name, ERR_BADNAME, file_path);
  
  if ( standard.size_limit > 0 )
    if ( check_file_size(file_path, standard.size_limit) == -1 )
      log_error(standard.name, ERR_TOOBIG, file_path);
  
  if ( standard.max_file_count > 0 && S_ISDIR(stat_buf.st_mode))
    if ( check_directory_file_count(file_path, standard.max_file_count) == -1)
      log_error(standard.name, ERR_FILECOUNT, file_path);

  if ( standard.are_symlinks_allowed == false && S_ISLNK(stat_buf.st_mode))
    log_error(standard.name, ERR_SYMLINK, file_path);

  if ( standard.are_hardlinks_allowed == false && (stat_buf.st_nlink > 1) && !S_ISDIR(stat_buf.st_mode) )
    log_error(standard.name, ERR_HARDLINK, file_path);
}


/* Checks FILE against all standards in STANDARDS_LIST */
void
standards_compare_against_file(char* file, standard_definition standards_list[])
{
  int i = 0;
  while (standards_list[i].name != NULL)
    {
      standard_check_file_compliance(file, standards_list[i]);
      i++;
    }
}


/* Attempt to retrieve standards listed in STANDARDS_STRING and place them into an array.
 Returns array on success, returns NULL on failure.*/
standard_definition
*standards_create_array(char* standards_string)
{
  standard_definition *standards_list = malloc(sizeof(standard_definition) * 15);
  char *component;
  int i = 0;

  component = strtok(standards_string, ",");
    if ( component == NULL )
      return NULL;

    while (component != NULL)
      {
	standards_list[i] = *standard_retrieve_by_name(component, GLOBAL_STANDARDS);
	component = strtok(NULL, ",");
	i++;
      }
    return standards_list;
}
int
main(int argc, char **argv)
{
  int opt;
  char* standards_list = "TESTING";
  standard_definition *standards_array;

  while ((opt = getopt(argc, argv,"s:d:p:f:c:b:n")) != -1) 
    {
      switch (opt)
	{
	case 's':
	  standards_list = optarg;
	  break;

	case 'd':
	  if ( strlen(optarg) == 0 )
	    {
	      printf("%s: Bad delimiter", argv[0]);
	      program_opts_print_short_help();
	      exit(-1);
	    }
	  DELIMITER = optarg;
	  break;

	default:
	  program_opts_print_short_help();
	  exit(1);
	  break;
	}
    }

  if (optind == argc)
    {
      printf("no input\n");
    }

  standards_array = standards_create_array(standards_list);

  struct stat stat_buffer;
  
  while (optind < argc)
    {
      if ( lstat(argv[optind], &stat_buffer) == -1)
	{
	  switch (errno)
	    {
	    case EACCES:
	      log_error("SYSTEM", ERR_PERMS, argv[optind]);
	      break;

	    case ENOENT:
	      log_error("SYSTEM", ERR_NOFILE, argv[optind]);
	      break;
	    default:
	      break;
	      //FIXME: handle error
	    }
	  optind++;
	}
      else
	{
	  standards_compare_against_file(argv[optind], standards_array);
	  optind++;
	}
    }

  if ( WAS_STANDARD_VIOLATION_ENCOUNTERED == true )
    exit(1);
  else
    exit(0);
}
