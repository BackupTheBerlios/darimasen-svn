/*
	mimehandler.c
		TabTop's routines for dealing with configuration and data files.

	Copyright (c) 2005, Ron Kuslak <rds@rdsarts.com>
	All rights reserved.

	Permission to use, copy, modify, and distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "mimehandler.h"
#include "safestring.h"

/* private functions: */
static void run_path(const char * path, const char * file);

char ** xdg_config_dirs = NULL,
     *  xdg_config_home = NULL,
     ** xdg_data_dirs   = NULL,
     *  xdg_data_home   = NULL;

/* Splits string "line_to_split" at character "spliter" and returns a array   *
 * of "char *" to them. Results must be freed by calling program.             */
char ** splitstr(const char * line_to_split, char spliter){
	char      ** array_ptr = NULL,
	          ** ret_ptr   = NULL;
	const char * temp_ptr  = line_to_split;
	int size_of_ptr;

	int size_of_array = 1,
	    size_of_string;
	
	while (line_to_split && *line_to_split != '\0') {
		if (*line_to_split == spliter)
			size_of_array++;

		line_to_split++;
}

	line_to_split = temp_ptr;
	size_of_ptr   = sizeof(char *) * (size_of_array + 1);
	array_ptr     = (char **)malloc(size_of_ptr);

	assert(temp_ptr);
	assert(array_ptr);

	memset(array_ptr, 0, size_of_array + 1);
	ret_ptr = array_ptr;

	while (line_to_split != NULL) {
		temp_ptr = strchr(line_to_split, spliter);

		if (temp_ptr) {
			size_of_string = (int)(temp_ptr - line_to_split + 1);

			*array_ptr = (char *)malloc(size_of_string);
			assert(*array_ptr);
			memset(*array_ptr, '\0', size_of_string);

			safe_strcpy(*array_ptr, line_to_split, size_of_string);
			temp_ptr++;
		} else {
			size_of_string = strlen(line_to_split) + 1;

			*array_ptr = (char *)malloc(size_of_string);
			assert(*array_ptr);
			memset(*array_ptr, '\0', size_of_string);

			safe_strcpy(*array_ptr, line_to_split, size_of_string);
		}

		line_to_split = temp_ptr;
		array_ptr++;
	}

	*array_ptr = NULL;

	return ret_ptr;
}


/* Grab enviroment variable, or set to a reasonable default                   */
void init_xdg_paths() {
	char * temp_ptr = NULL,
	     * home_ptr = NULL;

	home_ptr = getenv("HOME");
	if (! (home_ptr) ) {
		printf("$HOME undefined. The world is bleak and cold.\n"
		       "Exiting before we quote The Cure songs.\n");
		exit(-1);
	}

	/* XDG_CONFIG_DIRS */
	temp_ptr = (char *) getenv("XDG_CONFIG_DIRS");
	if (!temp_ptr)
		xdg_config_dirs = splitstr("/etc/xdg", ':');
	else
		xdg_config_dirs = splitstr(temp_ptr, ':');

	/* XDG_DATA_DIRS */
	temp_ptr = (char *) getenv("XDG_DATA_DIRS");
	if (!temp_ptr)
		xdg_data_dirs = splitstr("/usr/local/share/:/usr/share/", ':');
	else
		xdg_data_dirs = splitstr(temp_ptr, ':');

	if (! (xdg_config_home = getenv("XDG_CONFIG_HOME")) ) {
		xdg_config_home = path_join(home_ptr, ".config");
	}
	if (! (xdg_data_home   = getenv("XDG_DATA_HOME")) ) {
		xdg_data_home = path_join(home_ptr, ".local/share/");
	}
}


/* checks for a path. Will ignore non-executable entires if executable is set *
 * to false.                                                                  */
int exists(const char * path, ExistsBool exec_check) {
	struct stat buff;

	if ( (stat(path, &buff)) == 0 ){
		if (! (exec_check) )
			return 1;

		/* exec_check == TRUE */
		if ( (buff.st_mode & S_IXOTH)                                  ||
			 ( (buff.st_mode & S_IXGRP) && (buff.st_gid == getgid()) ) ||
			 ( (buff.st_mode & S_IXUSR) && (buff.st_uid == getuid()) ) )
				return 1;
		printf("Not executable.\n");
	}

	return 0;
}


/* Appends path "ext" to path "path" with a directory divider inbetween. Must *
 * be freed by caller.                                                        */
char * path_join(const char * path, const char * ext) {
	char * ret_path = NULL;
	int ret_size;

    ret_size = strlen(path) + strlen(ext) + 3;
    assert(ret_size > 0);

ret_path = (char *) malloc(ret_size);
	assert(ret_path);
	memset(ret_path, (int) '\0', ret_size);

	/* XXX: How to report errors on this? */
	safe_strcpy(ret_path, path, ret_size);
	safe_strcat(ret_path, "/",  ret_size);
	safe_strcat(ret_path, ext,  ret_size);

return ret_path;
}


/* Returns the path to the best entry for a config file named "path" in       *
 * xdg_config_dirs. Returns NULL if none are found.                           */
char * get_config_path(const char * path) {
	char *  ret_path = NULL,
	     ** dir_path = NULL;

	if (! (xdg_config_dirs) ) {
		init_xdg_paths();
	}

	if ( (xdg_config_home) ) {
	ret_path = path_join( xdg_config_home, path );
		/*printf("ret_path is '%s'\n", ret_path);*/
		if ( exists(ret_path, FIND_EXECUTABLE) )
			return ret_path;
	}

	dir_path = xdg_config_dirs;
	assert(dir_path);

	while ( *dir_path ) {
		ret_path = path_join( *dir_path, path );
		printf("ret_path is '%s'\n", ret_path);

	if ( exists(ret_path, FIND_EXECUTABLE) )
			return ret_path;

		free(ret_path);
		ret_path = NULL;
		dir_path++;
	}

	return NULL;
}


/* A wrapper for exec that will spawn a new process, and is AppDir aware.     *
 * path is the path to the file or AppDir to run, and file is the file to send*
 * to it. Must NOT be NULL                                                    */
static void run_path(const char * path, const char * file) {
char * resolved_path = NULL;

	pid_t pid;
int ret;

	pid = fork();

	if (pid < 0) {
		printf("Error spawning process\n");
		return;
	} 

	if (pid != 0)
		return;

	/* in forked process, resolve if AppDir and run */
	if ( get_path_is_appdir(path) ) {
		resolved_path = path_join(path, "AppRun");
		printf("%s\n", resolved_path);
		ret = execl(resolved_path, resolved_path, file, NULL);
		free(resolved_path);
		resolved_path = NULL;
	} else {
	ret = execl(path, path, file, NULL);
	}

	if ( ret != 0 )
		printf("Error: %s %s\n%d\n", resolved_path, file, ret);
	
	return;
}


/* runs "path" as if it where of mimetype "mimetype" and subtype "subtype."   *
 * subtype may be NULL to signify no known subtype.                           */
int run_path_as_mimetype(const char * path, const char * mimetype) {
	char * mime_type   = NULL,
	     * sub_mime    = NULL,
	     * temp_path   = NULL,
	     * exec_path   = NULL;
	
	const char mime_prefix[] = "MIME-types/";
	long mime_path_size;
int  ret_val = 0;

	mime_path_size = strlen(mimetype) + 1;
	mime_type = (char *)malloc(mime_path_size);
	assert(mime_type);

	safe_strcpy(mime_type, mimetype, mime_path_size);

	sub_mime  = strchr(mime_type, '/');
	if (sub_mime != NULL)
		*sub_mime = '_';

	/* Loops a maximum of twice. First attempts to find the matching MIME-type
	 * for mime_type with '/' replaced with '_', and if not found (or mime_type
	 * never contained a '/'), tries it with the unqualified MIME-type.       */
	while (mime_type != NULL) {
		mime_path_size = strlen(mime_prefix) + strlen(mime_type) + 1;
		assert(mime_path_size > 0);

		temp_path = (char *)malloc(mime_path_size);
		assert(temp_path);
		
		safe_strcpy(temp_path, mime_prefix, mime_path_size);
		safe_strcat(temp_path, mime_type,   mime_path_size);
		
		/* Attempt to find a matching MIME-type handler... */
		exec_path = get_config_path(temp_path);
		free(temp_path);
		temp_path = NULL;

		printf("%s\n", exec_path);

		if ( exec_path != NULL ) {
			run_path(exec_path, path);
			ret_val = 1;
		}

		if ( (sub_mime) && (*sub_mime == '_' ) && ! (ret_val) ) {
			*sub_mime = '\0';
		} else {
			free(mime_type);
			mime_type = NULL;
		}
	}

	return ret_val;
}


/* Returns 1 if a path is a location of a AppDir, 0 otherwise. */
int get_path_is_appdir(const char * path) {
	char *temp_path = NULL;

	struct stat buff;
	int  ret_val = 0;

	temp_path = path_join(path, "AppRun");
	assert(temp_path);

	printf("checking if %s exists\n", temp_path);
	if ( stat(temp_path, &buff) == 0 )
		ret_val = 1;

	free(temp_path);
	temp_path = NULL;

	return ret_val;
}
