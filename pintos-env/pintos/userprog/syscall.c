#include "userprog/syscall.h"
#include "lib/syscall-nr.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
void sys_write(int *);
void sys_exit(int *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  int *stack = f->esp;
  int code = *(stack+0);

  switch (code) {
    case SYS_WRITE:
      sys_write(stack);
      break;
    
    case SYS_EXIT:
      sys_exit(stack);
      break;
    
    default:
      break;
  }
}

void sys_write(int *stack) {
  int fd = *(stack+1);
  const char *buffer = (const char *) *(stack+2);
  size_t size = (size_t) *(stack+3);

  if (fd == 1)
    putbuf(buffer, size);
}

void sys_exit(int *stack) {
  int status = *(stack+1);
  thread_exit();
}
