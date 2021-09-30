# Important SSH/SFTP commands
|`sftp -P <PORT_NO> <USERNAME>@<IP>`|
		*`get <REMOTE_FILE_NAME>`:Download file to local machine <br>
		*`put <LOCAL_FILE_NAME>`:Upload file to server<br>
		*`put -r <LOCAL_DIR_NAME>`:Upload directory to server<br>
		*`ls`: Directory in server<br>
		*`lls`: Directory in local machine<br> |
|------------------|---------------------------|
	*`ssh <USERNAME>@<IP> -p <PORT_NO>`



# Cheatsheet
|Command	|Description |
|-----------|-------------|
|?			|List of all commands or explain a command, eg ? get
|!			|Leave the environment temporarily *
|cd			|Change the active directory path on the remote host
|chmod		|Change the permissions of files on the remote host
|chown		|Change the owner of files on the remote host
|dir		|List the contents of the current directory on the remote host
|exit		|Close the connection and leave SFTP
|get		|Copy a file from the remote host to the local computer
|help		|Same as ?
|lcd		|Change the active directory on the local system
|lls		|List the contents of the current directory on the local computer
|lmkdir		|Create a directory on the local computer
|ln			|Create a symbolic link for a file on the remote host
|lpwd		|Show the present working directory on the local computer
|ls			|Same as dir
|lumask		|Change the local umask value
|mkdir		|Create a directory on the remote host
|put		|Copy a file from the local computer to the remote host
|pwd		|Show the present working directory path on the remote host
|quit		|Same as exit
|rename		|Rename a file on the remote host
|rm			|Delete a file on the remote host
|rmdir		|Remove an empty directory on the remote host
|symlink	|Same as ln
|version	|Show the SFTP version