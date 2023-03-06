#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>  
#include <sys/user.h>
#include <sys/reg.h>

void print_fun(int name) {
	switch(name) {
		case 1: printf("write\n");
		       	break;
		default: printf("unknown\n");
			 break;
	}
}

void child() {
  ptrace(PTRACE_TRACEME, 0, 0, 0);
  execl("./hello", NULL);
  perror("execl");
}

void parent(pid_t pid) {
	int status;
 	waitpid(pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	while (!WIFEXITED(status)) {

    		struct user_regs_struct state;

   		 ptrace(PTRACE_SYSCALL, pid, 0, 0);
   		 waitpid(pid, &status, 0);

    
   		 if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
     			 ptrace(PTRACE_GETREGS, pid, 0, &state);
     			 printf("SYSCALL %lld at %08llx\n", state.orig_rax, state.rip);

     			 ptrace(PTRACE_SYSCALL, pid, 0, 0);
     			 waitpid(pid, &status, 0);
   		 }
	if (state.orig_rax == 1) {
		print_fun(state.orig_rax);
      }

  }

}


int main(int argc, char *argv[]) {
  	pid_t pid = fork();
  	if (pid)
    		parent(pid);
  	else
    		child();
  	return 0;
}
