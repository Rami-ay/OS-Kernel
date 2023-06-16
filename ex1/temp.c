#define MAX_MSG_LEN  4
int main(void)
{
	int msgid;
	key_t key;
	char sendWord[] = "bye";
	//get and id for the queue:
	if ( (key = ftok(".",'y')) == -1 )
	{
		perror("ftok failed\n");
		exit(EXIT_FAILURE);
	}
	
	
	//create the queue, if it exists then fail:
	if( (msgid = msgget(key,0600 | IPC_CREAT | IPC_EXCL)) == -1 )
	{
		perror("msgget failed\n")
		exit(EXIT_FAILURE);
	}
	
	
	
	//send the word bye to the queue if it fails then exit:
	if ( msgsnd (msgid, &sendWord,strlen(sendWord)+1,0) == -1)
	{
		perror("msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
	
	//reading the word hi from the queue if fails then exit:
	
	
	
	
	
}
