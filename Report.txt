Brief & Command options implemented:

I implemented the shell using fork(), execve(), and wait() system calls. 
Basically my approach was to read the input string and save it in a string vector to keep track of history.
Afterwards, I send the input string (or string retrieved from the hsitory) to be processed.
In the process function, I checked if the first word of the input string is either of the internal commands, if it is, execute appropriate logic.
Else, call the execve function, assuming that its an external command. If the command is valid, the appropriate binary gets called, otherwise the execve function returns -1, and you can just prints that the command is invalid.

Internal:


echo:
	Prints the contents given as arguments to the console.
	Flags:
	-n: Ignores a new-line character after printing the message
	-e: Enable escaping of " character if preceded by '\'


cd:
	Changes the current working directory to the one specified in the following argument.
	Flags:
	Done with support for ~ as home directory.


pwd:
	Outputs the path (absolute) of the current working directory.
	Flags:
	Not Applicable


history:
	Lists and indexes the commands that have run since the shell started. With support of !n you can directly execute the command indexed at n.
	Flags:
	-c: Clears the history
	-d idx: Clears the history entry at position idx


exit:
	Exits the shell.
	Flags:
	Not applicable


External:


cat:
	Lists the contents of the file(s) passed as arguments. Multiple files supported.
	Flags:
	-n: Numbers the lines (including black lines)
	-s: Prints the file ignoring contingous newline characters


date:
	Outputs the current system date and time, and weekday.
	Flags:
	-r: Prints the last modified time of a file
	-u: Displays the time in UTC


ls:
	Outputs the contents (files and folders) of the current working directory.
	Flags:
	-a:Prints all files, including hidden files
	-U: Does not sort the results


rm:
	Removes the file (or directory) from the file system passed as an argument, or list of arguments.
	Flags:
	-r: Recursive delete to remove non empty directories
	-d: Removes the empty directories


mkdir:
	Makes a folder (directory) in the file system passed as an argument, or list of arguments.
	Flags:
	-p: Makes the non-existent directories if they dont exist
	-m: Specifies the permissions to be given to the file, expects the permissions in form of a 3 digit number in next argument
