/*
 * Matthew Renodin
 *
 * main.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sel4/sel4.h>
#include <utils/util.h>
#include <sel4utils/util.h>
#include <sel4utils/helpers.h>

static sel4utils_thread_t   thread1;
static sel4utils_thread_t   thread2;

// the root CNode of the current thread
extern seL4_CPtr root_cnode;
// VSpace of the current thread
extern seL4_CPtr root_vspace;
// TCB of the current thread
extern seL4_CPtr root_tcb;
// Untyped object large enough to create a new TCB object

extern seL4_CPtr tcb_untyped;
extern seL4_CPtr buf2_frame_cap;
extern const char buf2_frame[4096];

// Empty slot for the new TCB object
extern seL4_CPtr tcb_cap_slot;
// Symbol for the IPC buffer mapping in the VSpace, and capability to the mapping
extern seL4_CPtr tcb_ipc_frame;
extern const char thread_ipc_buff_sym[4096];
// Symbol for the top of a 16 * 4KiB stack mapping, and capability to the mapping
extern const char tcb_stack_base[65536];
static const uintptr_t tcb_stack_top = (const uintptr_t)&tcb_stack_base + sizeof(tcb_stack_base);

int call_once(int arg) {
    printf("Hello 3 %d\n", arg);
}

int data = 42;

void initialize_subsystem_once(void)
{
  printf("*** Subsystem initialization should only happen once\n");
}

void initialize_subsystem(void)
{
  static once_flag initialize_subsystem_only_once = ONCE_FLAG_INIT;

  //call_once(&initialize_subsystem_only_once, initialize_subsystem_once);
}

int Thread1_Body(void *arg)
{
  sel4utils_thread_t *t = (thrd_t *) arg;
  int     rc;
  int     result;

  initialize_subsystem();

  printf("Thread1(0x%08lx)\n", *t);
  assert(thrd_current() == thread1);

  puts("Thread1 - yield" );
  thrd_yield();

  puts("Thread1 - join Thread2" );
  rc = thrd_join(thread2, &result);
  assert(rc == thrd_success);
  printf("Thread1 - thread2 exited with %d\n", result);

  puts("Thread1 - exit" );
  thrd_exit(0);
  return 0;
}

int Thread2_Body(void *arg)
{
  sel4utils_thread_t *t = (thrd_t *) arg;

  initialize_subsystem();

  printf("Thread2(0x%08lx)\n", *t);
  assert(thrd_current() == thread2);

  puts("Thread2 - exit" );
  thrd_exit(128);
  return 0;
}

int main(int argc, char **argv)
{
  int             rc;
  struct timespec delay = {1, 0};

  puts("*** START OF THREAD EXAMPLE ***");

  initialize_subsystem();
  sel4utils_start_thread(&thread1, &Thread1_Body, &thread1, 0);
  
  
//   seL4_DebugDumpScheduler();
//   seL4_Error result = seL4_Untyped_Retype(tcb_untyped, seL4_TCBObject, seL4_TCBBits, root_cnode, 0, 0, tcb_cap_slot, 1);
//   ZF_LOGF_IF(result, "Failed to retype thread: %d", result);
//   seL4_DebugDumpScheduler();

//   result = seL4_TCB_Configure(tcb_cap_slot, seL4_CapNull, root_cnode, 0, root_vspace, 0, (seL4_Word) thread_ipc_buff_sym, tcb_ipc_frame);
//   ZF_LOGF_IF(result, "Failed to configure thread: %d", result);

//   result = seL4_TCB_SetPriority(tcb_cap_slot, root_tcb, 254);
//   ZF_LOGF_IF(result, "Failed to set the priority for the new TCB object.\n");
//   seL4_DebugDumpScheduler();

//   UNUSED seL4_UserContext regs = {0};
//   int error = seL4_TCB_ReadRegisters(tcb_cap_slot, 0, 0, sizeof(regs)/sizeof(seL4_Word), &regs);
//   ZF_LOGF_IFERR(error, "Failed to write the new thread's register set.\n"
//                 "\tDid you write the correct number of registers? See arg4.\n");


//   sel4utils_arch_init_local_context((void*)new_thread,
//                                 (void *)call_once, (void *)&data, (void *)3,
//                                 (void *)tcb_stack_top, &regs);
//    rc = thrd_create(&thread1, Thread1_Body, &thread1);
//   error = seL4_TCB_WriteRegisters(tcb_cap_slot, 0, 0, sizeof(regs)/sizeof(seL4_Word), &regs);
//   ZF_LOGF_IFERR(error, "Failed to write the new thread's register set.\n"
//                 "\tDid you write the correct number of registers? See arg4.\n");


//   // resume the new thread
//   error = seL4_TCB_Resume(tcb_cap_slot);
//   ZF_LOGF_IFERR(error, "Failed to start new thread.\n");
  
  
  
  
  
//   puts("main - Create Thread1");
//   rc = thrd_create(&thread1, Thread1_Body, &thread1);
//   assert(rc == thrd_success);

//   puts("main - Detach Thread1");
//   rc = thrd_detach(thread1);
//   assert(rc == thrd_success);

//   puts("main - yield to Thread1" );
//   thrd_yield();

//   puts("main - Create Thread2");
//   rc = thrd_create(&thread2, Thread2_Body, &thread2);
//   assert(rc == thrd_success);

//   puts("Exercise thrd_equal");
//   rc = thrd_equal(thread1, thread2);
//   assert(rc == 0);

//   rc = thrd_equal(thread1, thread1);
//   assert(rc != 0);

//   puts("main - sleep and let other threads run");
//   rc = thrd_sleep(&delay, NULL);
//   assert(rc == 0);

//   puts("*** END OF THREAD EXAMPLE ***");
 
  return 0;
}
