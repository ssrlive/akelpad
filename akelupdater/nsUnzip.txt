*****************************************************************
***                 nsUnzip NSIS plugin v1.1                 ***
*****************************************************************

2009 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)


nsUnzip plugin will test or extract files from a ZIP archive.
Code of the DLL uses sources of UnZip 5.52 of 28 February 2005, by Info-ZIP.


Features:
- Flexible extract options.
- Regular expressions (wildcards) may be used to match multiple members.
- Possibility to exclude specified files from processing.
- User function for unpacking progress.
- User function for listing archive.
- User function for crypted archive.


**** Extract ****

nsUnzip::Extract [options] "file[.zip]" "[file(s) ...]" /END
Pop $var


[options]
	/t	       -test archive files. This option extracts each specified file in memory and compares the CRC of the expanded file with the original file's stored CRC value.
	/e             -extract only those files that already exist on disk.
	/f             -extract only those files that already exist on disk and that are newer than the disk copies.
	/u             -update existing files and create new ones if needed.
	/n             -never overwrite existing files.
	/j             -junk paths. The archive's directory structure is not recreated.
	/C             -use case-insensitive matching for the selection of archive entries.
	/s             -convert spaces in filenames to underscores.
	/X             -restore security ACLs under Windows NT.
	/XX            -use privileges for extraction under Windows NT.
	"/x=file"      -file to be excluded from processing.
	                This option may be used to exclude any files that are in subdirectories.
	                For example, `` nsUnzip::Extract foo.zip *.[ch] /x=*\* '' would extract all C source files in the main directory, but none in any subdirectories.
	                Without the -x option, all C source files in all directories within the zipfile would be extracted.
	"/d=c:\dir"    -an optional directory to which to extract files. If not specidied, extract into the current directory.
	"/P=password"  -use password to decrypt encrypted zipfile entries (if any).
	/callP=$var    -Password callback function address. Only if "/P=password" not specified. See nsUnzipTest.nsi.
	/callS=$var    -Service callback function address. Useful to show unpacking progress. See nsUnzipTest.nsi.
	/callV=$var    -Verbose callback function address. List contents of archive. See nsUnzipTest.nsi.

"file[.zip]"
	Path of the ZIP archive(s). If the file specification is a wildcard,
	each matching file is processed in an order determined by the operating
	system (or file system). Only the filename can be a wildcard;
	the path itself cannot. Wildcard  may  contain:

	*      matches a sequence of 0 or more characters

	?      matches exactly 1 character

	[...]  matches any single character found inside the brackets;
	       ranges are specified by a beginning character, a hyphen,
	       and an ending character. If an exclamation point or a
	       caret (`!' or `^') follows the left bracket, then the range
	       of characters within the brackets is complemented (that is,
	       anything except the characters inside the brackets is
	       considered a match). To specify a verbatim left bracket, the
	       three-character sequence ``[[]'' has to be used.

	If no matches are found, the specification is assumed to be a literal
	filename; and if that also fails, the suffix .zip is appended.
	Note that self-extracting ZIP files are supported, as with any other
	ZIP archive; just specify the .exe suffix (if any) explicitly.

"[file(s) ...]"
	An optional list of archive members to be processed, separated by
	spaces. Regular expressions (wildcards) may be used to match
	multiple members; see above.

	The two-character sequence ``**'' acts as a multi-char wildcard that
	includes the directory separator in its matched characters. Examples:

	 "*.c" matches "foo.c" but not "mydir\foo.c"
	 "**.c" matches both "foo.c" and "mydir\foo.c"
	 "*\*.c" matches "bar\foo.c" but not "baz\bar\foo.c"
	 "??*\*" matches "ab\foo" and "abc\foo"
	         but not "a\foo" or "a\b\foo"

/END
	Stop parsing parameters (required).

Pop $var
	 0 - Normal; no errors or warnings detected. (There may still be errors in the archive, but if so, they weren't particularly relevant to UnZip's processing and are presumably quite minor.)
	 1 - One or more warning errors were encountered, but processing completed successfully anyway. This includes zipfiles where one or more files was skipped due to unsupported compression method or encryption with an unknown password.
	 2 - A generic error in the zipfile format was detected. Processing may have completed successfully anyway; some broken zipfiles created by other archivers have simple work-arounds.
	 3 - A severe error in the zipfile format was detected. Processing probably failed immediately.
	 4 - UnZip was unable to allocate memory for one or more buffers during program initialization.
	 5 - UnZip was unable to allocate memory or unable to obtain a tty (terminal) to read the decryption password(s).
	 6 - UnZip was unable to allocate memory during decompression to disk.
	 7 - UnZip was unable to allocate memory during in-memory decompression.
	 9 - The specified zipfile(s) was not found.
	10 - Invalid options were specified on the command line.
	11 - No matching files were found.
	50 - The disk is (or was) full during extraction.
	51 - The end of the ZIP archive was encountered prematurely.
	80 - The user aborted UnZip prematurely with control-C (or similar)
	81 - Testing or extraction of one or more files failed due to unsupported compression methods or unsupported decryption.
	82 - No files were found due to bad decryption password(s). (If even one file is successfully processed, however, the exit status is 1.)


**** GetPassword dialog ****

nsUnzip::GetPassword /NOUNLOAD "title" "group" "label" "ok_button" "cancel_button"
Pop $0
