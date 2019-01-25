#include "main.h"

int flag = false;
void handler(int );

void pool_process(char* path, int max_jobs, int index, int number)
{
	char** str_job;
	char** result;

	char buffer[100];
	char pool_in[100];
	char pool_out[100];
	
	int fpool_in, fpool_out;
	int nread, nwrite, status;
	int i = 1, count = 0, j;
	
	pid_t job[max_jobs], pid;
	jms_job job_table[max_jobs];
	
	init_struct(job_table,max_jobs);
	
	signal(SIGTERM,handler);

   	/* Stick the path behind pool_in and pool_out */
	sprintf(pool_in,"%s/pool_in_%d",path,index);
	sprintf(pool_out,"%s/pool_out_%d",path,index);
	
	/* Disconnect any fifo that exists */
	disconnect(pool_in,pool_out);
	
	/* Create and Open pool_in and pool_out */
	if( mkfifo(pool_in, 0666) == -1 ) { perror("Make fifo of pool_in failed!");  exit(1); }
	if( mkfifo(pool_out, 0666) == -1 ) { perror("Make fifo of pool_out failed!"); exit(2); } 
	if( (fpool_in = open(pool_in, O_RDONLY | O_NONBLOCK)) == -1 ) { perror("Open fifo of pool_in failed!"); exit(15); }
	if( (fpool_out = open(pool_out, O_WRONLY)) == -1 ) { perror("Open fifo of pool_out failed!"); exit(16); }
	
	printf("Create pool_%d with pid: %d\n",index,getpid());
	
	while( count < max_jobs )
	{
		if( (nread = read(fpool_in,buffer,sizeof(buffer))) > 0 )
		{	
			str_job = analyze(buffer);
			result = rearrange(str_job);
			
			if( strcmp(str_job[0],"submit") == 0 )
			{	
				job[i] = fork();
				if( job[i] == 0 ) { exec_job(result,path,number); }
				
				sprintf(buffer,"Jobid: %d, PID: %d",number,job[i]);
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
				
				job_table[i].job_id = job[i];
				job_table[i].job_num = number;
				strcpy(job_table[i].status,"Active");
				
				i++;	// For the next
				number++; // For the next
			}
			else if( strcmp(str_job[0],"status") == 0 )
			{
				int jobid = atoi(str_job[1]);
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
					if( jobid == job_table[pos].job_num )
						break;
				
				count = check_kids(job_table, max_jobs, count);
				
				sprintf(buffer,"Jobid: %d Status: %s",job_table[pos].job_num,job_table[pos].status);
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"status-all") == 0 )
			{
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
				{
					count = check_kids(job_table, max_jobs, count);
					
					sprintf(buffer,"Jobid: %d status: %s",job_table[pos].job_num,job_table[pos].status);
				
					if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
					sleep(1);
				}
			}
			else if( strcmp(str_job[0],"show-active") == 0 )
			{
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
				{
					count = check_kids(job_table, max_jobs, count);
			
					if( strcmp(job_table[pos].status,"Active") == 0 )
					{	
						sprintf(buffer,"Jobid: %d",job_table[pos].job_num);
						if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
						
						sleep(1);
					}
				}
				
				sprintf(buffer,"end");
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"show-finished") == 0 )
			{
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
				{	
					count = check_kids(job_table, max_jobs, count);
					
					if( strcmp(job_table[pos].status,"Finished") == 0 )
					{	
						sprintf(buffer,"Jobid: %d",job_table[pos].job_num);
						if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
						
						sleep(1);
					}
				}
				
				sprintf(buffer,"end");
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"show-pools") == 0 )
			{
				int pos, j = 0;
				
				for(pos = 1; pos <= max_jobs; pos++)	
				{
					count = check_kids(job_table, max_jobs, count);
					
					if( strcmp(job_table[pos].status,"Active") == 0 || strcmp(job_table[pos].status,"Suspended") == 0  ) { j++; }
				}
				
				sprintf(buffer,"%d %d",getpid(),j);
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"suspend") == 0 )
			{
				int jobid = atoi(str_job[1]);
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
					if( jobid == job_table[pos].job_num )
						break;
				
				kill(job_table[pos].job_id, SIGSTOP);
				
				strcpy(job_table[pos].status,"Suspended");
				
				sprintf(buffer,"Sent suspend signal to Jobid %d",job_table[pos].job_num);
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"resume") == 0 )
			{
				int jobid = atoi(str_job[1]);
				int pos;
				
				for(pos = 1; pos <= max_jobs; pos++)	
					if( jobid == job_table[pos].job_num )
						break;
				
				kill(job_table[pos].job_id, SIGCONT);
				
				sprintf(buffer,"Sent resume signal to Jobid %d",job_table[pos].job_num);
				if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
			}
			else if( strcmp(str_job[0],"shutdown") == 0 )
			{
				if( flag == true )
				{
					int pos, num = 0, ret;
			
					count = check_kids(job_table, max_jobs, count);
				
					for(pos = 1; pos <= max_jobs; pos++)
						if( strcmp(job_table[pos].status,"Active") == 0 )
						{	
							num++;
							if( (ret = kill(job_table[pos].job_id,SIGTERM)) == -1 ) { perror("SIGTERM"); }
							printf("SIGTERM has sent to %d\n",job_table[pos].job_id);
						}
						else if( strcmp(job_table[pos].status,"Suspended") == 0 ) // An einai suspended me SIGTERM den mou termatize
						{
							num++;
							if( (ret = kill(job_table[pos].job_id,SIGKILL)) == -1 ) { perror("SIGKILL"); }
							printf("SIGKILL has sent to %d\n",job_table[pos].job_id);
						}
				
					flag = false;
			
					sprintf(buffer,"%d",num);
					if( (nwrite = write(fpool_out,buffer,strlen(buffer)+1)) == -1 ) { perror("Error writting to coord"); }
				}
			}
	
			free_table(result);
			free_table(str_job);
		}
			
		count = check_kids(job_table, max_jobs, count);
	}	
	
	printf("Pool_%d exits.\n",index);
	
	/* If pool terminate unlick it's fifo */
	disconnect(pool_in,pool_out);
		
	exit(14);
}

/* Signal Handler */
void handler(int signum)
{
	signal(SIGINT, handler);
	flag = true;
}


