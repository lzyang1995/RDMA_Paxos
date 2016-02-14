#ifdef __SPEC_HOOK___libc_start_main

extern "C" void __tern_prog_begin(void);  //  lib/runtime/helper.cpp

typedef int (*main_type)(int, char**, char**);

struct arg_type
{
  char **argv;
  int (*main_func) (int, char **, char **);
};

main_type saved_init_func = NULL;
void tern_init_func(int argc, char **argv, char **env){
  dprintf("%04d: __tern_init_func() called.\n", (int) pthread_self());
  if(saved_init_func)
    saved_init_func(argc, argv, env);
  __tern_prog_begin();
}

extern "C" int my_main(int argc, char **pt, char **aa)
{
  int ret;
  arg_type *args = (arg_type*)pt;
  dprintf("%04d: __libc_start_main() called.\n", (int) pthread_self());
  ret = args->main_func(argc, args->argv, aa);
  return ret;
}

extern "C" int __libc_start_main(
  void *func_ptr,
  int argc,
  char* argv[],
  void (*init_func)(void),
  void (*fini_func)(void),
  void (*rtld_fini_func)(void),
  void *stack_end)
{
  typedef void (*fnptr_type)(void);
  typedef int (*orig_func_type)(void *, int, char *[], fnptr_type,
                                fnptr_type, fnptr_type, void*);
  orig_func_type orig_func;
  arg_type args;

  void * handle;
  int ret;

  // Get lib path.
  Dl_info dli;
  dladdr((void *)dlsym, &dli);
  std::string libPath = dli.dli_fname;
  libPath = dli.dli_fname;
  size_t lastSlash = libPath.find_last_of("/");
  libPath = libPath.substr(0, lastSlash);
  libPath += "/libc.so.6";
  libPath = "/lib/x86_64-linux-gnu/libc.so.6"; // Heming hack: weird thing on bug03.

  if(!(handle=dlopen(libPath.c_str(), RTLD_LAZY))) {
    puts("dlopen error");
    abort();
  }

  orig_func = (orig_func_type) dlsym(handle, "__libc_start_main");

  if(dlerror()) {
    puts("dlerror");
    abort();
  }

  dlclose(handle);

#ifdef __USE_TERN_RUNTIME
  dprintf("%04d: __libc_start_main is hooked.\n", (int) pthread_self());
  //fprintf(stderr, "%04d: __tern_prog_begin() called.\n", (int) pthread_self());
  args.argv = argv;
  args.main_func = (main_type)func_ptr;
  saved_init_func = (main_type)init_func;
  // note that we don't hook fini_func because it appears that fini_func
  // is only called for statically linked libc, whereas rtld_fini_func is
  // called regardless
  saved_fini_func = (fini_type)rtld_fini_func;
  ret = orig_func((void*)my_main, argc, (char**)(&args),
                  (fnptr_type)tern_init_func, (fnptr_type)fini_func,
                  rtld_fini_func, stack_end);
                  //(fnptr_type)tern_fini_func, stack_end);
  return ret;
#endif
  ret = orig_func(func_ptr, argc, argv, init_func, fini_func,
                  rtld_fini_func, stack_end);

  return ret;
}
#endif
