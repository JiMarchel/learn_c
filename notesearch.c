#include "hacking.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define FILENAME "/var/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]) {
  int userid, printing = 1, fd; // File descriptor

  char searchstring[100];

  if (argc > 1)                    // If there is an arg,
    strcpy(searchstring, argv[1]); // that is the search string;
  else                             // otherwise,
    searchstring[0] = 0;           // search string is empty.

  userid = getuid();

  fd = open(FILENAME, O_RDONLY); // Open the file for read-only access.

  if (fd == -1)
    fatal("in main() while opening file for reading");

  while (printing)
    printing = print_notes(fd, userid, searchstring);
  printf("-------[ end of note data ]-------\n");

  close(fd);
}

int print_notes(int fd, int uid, char *searchstring) {
  int note_length;
  char byte = 0, note_buffer[100];

  note_length = find_user_note(fd, uid);
  if (note_length == -1)
    return 0;

  read(fd, note_buffer, note_length);         // Read note data.
  note_buffer[note_length] = 0;               // Terminate the string.
  if (search_note(note_buffer, searchstring)) // If searchstring found,
    printf(note_buffer);                      // print the note.
  return 1;
}

int find_user_note(int fd, int user_uid) {
  int note_uid = -1;
  unsigned char byte;
  int length;
  while (note_uid != user_uid) {
    if (read(fd, &note_uid, 4) != 4) // Read the uid data.
      return -1; // If 4 bytes aren't read, return end of file code.
    if (read(fd, &byte, 1) != 1) // Read the newline separator.
      return -1;
    byte = length = 0;
    while (byte != '\n') { // Figure out how many bytes to the end of line.
      if (read(fd, &byte, 1) != 1) // Read a single byte.
        return -1; // If byte isn't read, return end of file code.
      length++;
    }
  }
  lseek(fd, length * -1, SEEK_CUR); // Rewind file reading by length bytes.
  printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
  return length;
}

// A function to search a note for a given keyword;
// returns 1 if a match is found, 0 if there is no match.
int search_note(char *note, char *keyword) {
  int i, keyword_length, match = 0;
  keyword_length = strlen(keyword);
  if (keyword_length == 0)             // If there is no search string,
    return 1;                          // always "match".
  for (i = 0; i < strlen(note); i++) { // Iterate over bytes in note.
    if (note[i] == keyword[match])     // If byte matches keyword,
      match++;                         // get ready to check the next byte;
    else {                             // otherwise,
      if (note[i] == keyword[0]) // if that byte matches first keyword byte,
        match = 1;               // start the match count at 1.
      else
        match = 0; // Otherwise it is zero.
    }
    if (match == keyword_length) // If there is a full match,
      return 1;                  // return matched.
  }
  return 0; // Return not matched.
}
