 #include <iostream>
 #include <pty.h>
 #include <string>
 #include <sys/wait.h>
 #include <unistd.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
 using namespace std;
char buf[4097];
ssize_t size;

void openpty_demo(const char *output_size) {
   int master;
   int slave;
   openpty(&master, &slave, NULL, NULL, NULL);

   // Temporarily redirect stdout to the slave, so that the command executed in
   // the subprocess will write to the slave.
   int _stdout = dup(STDOUT_FILENO);
    dup2(slave, STDOUT_FILENO);

     pid_t pid = fork();//create copy of current process
     if (pid == 0) {//if child
    //const char *argv[] = {"head", "-c", output_size, "/dev/zero", NULL};
      const char *argv[] ={"echo","hello world!",NULL,NULL,NULL};
         execvp(argv[0], const_cast<char *const *>(argv));
      }

    fd_set rfds;
    struct timeval tv{0, 0};
     //char buf[4097];
     //ssize_t size;
     size_t count = 0;

     // Read from master as we wait for the child process to exit.
     //
      // We don't wait for it to exit and then read at once, because otherwise the
      // command being executed could potentially saturate the slave's buffer and
     // stall.
      while (1) {
      if (waitpid(pid, NULL, WNOHANG) == pid) {
      break;
      }
     FD_ZERO(&rfds);
     FD_SET(master, &rfds);
     if (select(master + 1, &rfds, NULL, NULL, &tv)) {
     size = read(master, buf, 4096);
     buf[size] = '\0';
     count += size;
      }
      }

     // Child process terminated; we flush the output and restore stdout.
     fsync(STDOUT_FILENO);
      dup2(_stdout, STDOUT_FILENO);

     // Read until there's nothing to read, by which time we must have read
     // everything because the child is long gone.
     /*while (1) {
     FD_ZERO(&rfds);
     FD_SET(master, &rfds);
     if (!select(master + 1, &rfds, NULL, NULL, &tv)) {
     // No more to read.
     break;
      }
     size = read(master, buf, 4096);
     buf[size] = '\0';
     count += size;
     }*/

    // Close both ends of the pty.
   close(master);
   close(slave);

 cout << "Total characters read: " << count  << endl;


 }

  int main(int argc, const char *argv[]) {
   //openpty_demo(argv[1]);
   Display *display;
   Window window;
   XEvent event;
   int s,i,j,k;




    /* open connection with the server */
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
     }

    s = DefaultScreen(display);


    /* create window */
    window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 200, 200, 1,
   BlackPixel(display, s), WhitePixel(display, s));

  /* select kind of events we are interested in */
  XSelectInput(display, window, KeyPressMask | KeyReleaseMask );
  /* map (show) the window */
   XMapWindow(display, window);
   /* event loop */

   while (1)
   {
   XNextEvent(display, &event);
   /* keyboard events */
   if (event.type == KeyPress)
   {
    KeySym ks = XKeycodeToKeysym(display, event.xkey.keycode, 0);
 //char* result = XKeysymToString(ks);
 char* result;
   openpty_demo(argv[1]);
 for(k=0;k<size;k++){
   result=&buf[k];
    if ((i%100)==0){j+=10;i=0;}
  XDrawString(display, window, DefaultGC(display, s), i+=10, j, result, 1);
  //printf( "KeyPress: %c\n", result);
 }
 /* exit on ESC key press */
  if ( event.xkey.keycode == 0x09 )
   break;
   }
   else if (event.type == KeyRelease)
   {
     //printf( "KeyRelease: %x\n", event.xkey.keycode );
   }
   }


   /* close connection to server */
    XCloseDisplay(display);


                                          return 0;
  }
