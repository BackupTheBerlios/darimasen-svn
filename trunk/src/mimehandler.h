/*
	mimehandler.h
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

extern char ** xdg_config_dirs,
            *  xdg_config_home,
            ** xdg_data_dirs,
            *  xdg_data_home;

typedef enum _EXISTS_BOOL {
	FIND_ALL        = 0,
	FIND_EXECUTABLE = 1,
} ExistsBool;


/* Grab enviroment variable, or set to a reasonable default                   */
void init_xdg_paths();

/* checks for a path. Will ignore non-executable entires if executable is set *
 * to false.                                                                  */
int exists(const char * path, ExistsBool exec_check);

/* Appends path "ext" to path "path" with a directory divider inbetween. Must *
 * be freed by caller.                                                        */
char * path_join(const char * path, const char * ext);

/* Returns the path to the best entry for a config file named "path" in       *
 * xdg_config_dirs. Returns NULL if none are found.                           */
char * get_config_path(const char * path);

/* runs "path" as if it where of mimetype "mimetype" and subtype "subtype."   *
 * subtype may be NULL to signify no known subtype.                           */
int run_path_as_mimetype(const char * path, const char * mimetype);

/* Splits string "line_to_split" at character "spliter" and returns a array   *
 * of "char *" to them. Results must be freed by calling program.             */
char ** splitstr(const char * line_to_split, char spliter);

/**** SHOULD GO IN APPDIR.H! ****/
/* Returns 1 if a path is a location of a AppDir, 0 otherwise. */
int get_path_is_appdir(const char * path);
