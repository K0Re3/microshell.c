#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>

#define WHITE_SPACE " \r\n\t\a"
#define MAX_SIZE 512
#define BUFSIZE 128

#define RED "\033[0;31m" /*31*/
#define RED_BOLD "\033[1;31m" /*131*/
#define GREEN "\033[0;32m" /*32*/
#define GREEN_BOLD "\033[1;32m" /*132*/
#define YELLOW "\033[0;33m" /*33*/
#define YELLOW_BOLD "\033[1;33m" /*133*/
#define BLUE "\033[0;34m" /*34*/
#define PINK_BOLD "\033[1;34m" /*134*/
#define MAGENTA "\033[0;35m" /*35*/
#define MAGENTA_BOLD "\033[1;35m" /*135*/
#define CYAN "\033[0;36m" /*36*/
#define CYAN_BOLD "\033[1;36m" /*136*/
#define RESET_COLOR "\033[0m" /*0*/
#define RESET_BOLD_COLOR "\033[1m"/*10*/
#define UNDERLINE "\033[4m"
#define RESET_UNDERLINE "\033[24m"

void help();
void polski();
void english();
void change();
void empty();
void path();
char **polecenie_split();
void clear();
void user();
void exe();
void copy_files();
void process();
int uline();
int user_color();
int show_user();
int pathline();
int path_color();
int show_path();

int underline;
int color;
int suser;
int underline_p;
int pcolor;
int spath;

int main()
{
    char polecenie[MAX_SIZE] = {0};
    char **args;
    color = 0;
    suser = 1;
    underline = 0;
    pcolor = 0;
    spath = 1;
    underline_p = 0;
    const char *homedir = getenv("HOME");

    if(!homedir) {
        struct passwd* p = getpwuid(getuid());
        if(p)
           homedir = p->pw_dir;
    }

    for (;;) {
        user(underline, color, suser);
        path(underline_p, pcolor, spath);

        fgets(polecenie, sizeof polecenie, stdin);

        args = polecenie_split(polecenie);

        if (strcmp(polecenie, "") == 0) {
            empty();
        } else if (strcmp(polecenie, "cd") == 0) {
            change(args,homedir);
        } else if (strcmp(polecenie, "help") == 0) {
            help();
        } else if (strcmp(polecenie, "exit") == 0) {
            printf("%s", GREEN_BOLD);
        puts("HAVE A NICE DAY!");
        printf("%s", RESET_COLOR);
            exit(0);
        } else if (strcmp(polecenie, "clear") == 0) {
              clear();
        } else if(strcmp(polecenie, "cfiles") == 0) {
              copy_files(args);
        } else if(strcmp(polecenie, "uline") == 0) {
              uline(args, underline);
        } else if(strcmp(polecenie, "usrcol") == 0) {
              user_color(args, color);
        } else if(strcmp(polecenie, "pline") == 0) {
              pathline(args, underline_p );
        } else if(strcmp(polecenie, "pathcol") == 0) {
              path_color(args, pcolor);
        } else if(strcmp(polecenie,"process") == 0) {
              process(args);
        } else if(strcmp(polecenie,"suser") == 0) {
              show_user(args, suser);
        } else if(strcmp(polecenie,"spath") == 0) {
              show_path(args, spath);
        } else
              exe(args);
 }
    return 0;
}

void help() {
    printf("\n            You are in %smicroshell's help \n\n", CYAN_BOLD);
    printf("%s %s", RESET_COLOR, RESET_BOLD_COLOR);
    printf( "         Supported commands:\n"
            "         >cd -----------------------> change the shell working directory to home directory\n"
            "         >cd [directory] -----------> change the shell working directory\n"
            "         >exit ---------------------> cause normal process termination\n"
            "         >echo ---------------------> display a line of text\n"
            "         >clear --------------------> clear the terminal screen\n"
            "         >cfiles [file1] [file2] ---> copy file1 to file2\n"
            "         >process ------------------> report a snapshot of the current processes\n"
            "         >process cpuinfo ----------> show details about individual cpu cores\n"
            "         >process ram --------------> show details about total installed and free RAM\n"
            "         >process [PID] ------------> show details about selected process\n"
            "         >usrcol [color] [bold/''] -> change user's color\n"
            "         >uline [on/off] -----------> underline user's name\n"
            "         >suser [on/off] -----------> turns up user's name (1) or turn down (0)\n"
            "         >pathcol [color] [bold/'']-> change path's color\n"
            "         >pline [on/off] -----------> underline path's name\n"
            "         >spath [on/off] -----------> turns up path's name (1) or turn down (0)\n");
     printf("               Available Colours:\n");
     printf("          %s* red          %s red bold\n", RED, RED_BOLD);
     printf("          %s* green        %s green bold\n", GREEN, GREEN_BOLD);
     printf("          %s* yellow       %s yellow bold\n", YELLOW, YELLOW_BOLD);
     printf("          %s* blue         %s pink bold\n", BLUE, PINK_BOLD);
     printf("          %s* magenta      %s magenta bold\n", MAGENTA, MAGENTA_BOLD);
     printf("          %s* cyan         %s cyan bold\n", CYAN, CYAN_BOLD);
     printf("          %s* reset        %s reset bold\n\n", RESET_COLOR, RESET_BOLD_COLOR);
     printf("       %s       %sUnderline\n\n",RESET_COLOR, UNDERLINE);
     printf("%s",RESET_UNDERLINE);
     printf("%s           Cre%sted%s by %sDawid%s Kory%sbalski%s\n\n", YELLOW, CYAN, BLUE, GREEN, MAGENTA, RED, RESET_COLOR);
}

void change(char **args, char *homedir) {
  if(args[1] == NULL){
      if(chdir(homedir) != 0){
          perror("Change directory error");
      }
  } else {
    if(chdir(args[1]) != 0)
    {
            perror("Change directory failed");
        }
    }
 }
void empty() {
      printf("%s", RED);
      printf("You didn't enter the command - if you want to check aviailable commands, wrtie %s'help'\n", GREEN);
      printf("%s", RESET_COLOR);
}

void path() {
    char sciezka[512];
    getcwd(sciezka, sizeof(sciezka));
    if(spath == 1) {
    if(underline_p == 0) {
      if(pcolor == 0) {
        printf("%s",RESET_COLOR);
    } else if(pcolor == 10) {
        printf("%s", RESET_BOLD_COLOR);
    } else if(pcolor == 31) {
        printf("%s", RED);
    } else if(pcolor == 32) {
        printf("%s", GREEN);
    } else if(pcolor == 33) {
        printf("%s", YELLOW);
    } else if(pcolor == 34) {
        printf("%s", BLUE);
    } else if(pcolor == 35) {
        printf("%s", MAGENTA);
    } else if(pcolor == 36) {
        printf("%s", CYAN);
    } else if(pcolor == 131) {
        printf("%s", RED_BOLD);
    } else if(pcolor == 132) {
        printf("%s", GREEN_BOLD);
    } else if(pcolor == 133) {
        printf("%s", YELLOW_BOLD);
    } else if(pcolor == 134) {
        printf("%s", PINK_BOLD);
    } else if(pcolor == 135) {
        printf("%s", MAGENTA_BOLD);
    } else if(pcolor == 136) {
        printf("%s", CYAN_BOLD);
  }
    printf("[%s] ", sciezka);
    printf("%s",RESET_COLOR);
}
else if(underline_p == 1) {
  printf("%s",UNDERLINE);
  if(pcolor == 0) {
    printf("%s",RESET_COLOR);
} else if(pcolor == 10) {
    printf("%s", RESET_BOLD_COLOR);
} else if(pcolor == 31) {
    printf("%s", RED);
} else if(pcolor == 32) {
    printf("%s", GREEN);
} else if(pcolor == 33) {
    printf("%s", YELLOW);
} else if(pcolor == 34) {
    printf("%s", BLUE);
} else if(pcolor == 35) {
    printf("%s", MAGENTA);
} else if(pcolor == 36) {
    printf("%s", CYAN);
} else if(pcolor == 131) {
    printf("%s", RED_BOLD);
} else if(pcolor == 132) {
    printf("%s", GREEN_BOLD);
} else if(pcolor == 133) {
    printf("%s", YELLOW_BOLD);
} else if(pcolor == 134) {
    printf("%s", PINK_BOLD);
} else if(pcolor == 135) {
    printf("%s", MAGENTA_BOLD);
} else if(pcolor == 136) {
    printf("%s", CYAN_BOLD);
}
printf("[%s] ", sciezka);
printf("%s",RESET_COLOR);
printf("%s", RESET_UNDERLINE);
}
  printf(" $ ");
}
else if(spath == 0){
return;
}
}

char **polecenie_split(char *polecenie) {
    int i = 0;
    char **tokens = malloc(BUFSIZE * sizeof(char *));
    char *token;

    token = strtok(polecenie, WHITE_SPACE);
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, WHITE_SPACE);
    }

    if (i > BUFSIZE) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }
    tokens[i] = NULL;

    return tokens;
}

void clear() {
    const char *CLEAR_SCREEN_ANSI = "\n\033[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void user() {
    char *p=getenv("USER");
    if(p==NULL)
      perror("Shell's error!");
      if(suser == 1) {
      if(underline == 0) {
        if(color == 0) {
          printf("%s",RESET_COLOR);
      } else if(color == 10) {
          printf("%s", RESET_BOLD_COLOR);
      } else if(color == 31) {
          printf("%s", RED);
      } else if(color == 32) {
          printf("%s", GREEN);
      } else if(color == 33) {
          printf("%s", YELLOW);
      } else if(color == 34) {
          printf("%s", BLUE);
      } else if(color == 35) {
          printf("%s", MAGENTA);
      } else if(color == 36) {
          printf("%s", CYAN);
      } else if(color == 131) {
          printf("%s", RED_BOLD);
      } else if(color == 132) {
          printf("%s", GREEN_BOLD);
      } else if(color == 133) {
          printf("%s", YELLOW_BOLD);
      } else if(color == 134) {
          printf("%s", PINK_BOLD);
      } else if(color == 135) {
          printf("%s", MAGENTA_BOLD);
      } else if(color == 136) {
          printf("%s", CYAN_BOLD);
    }
      printf("%s",p);
      printf("%s",RESET_COLOR);
}
else if(underline == 1) {
    printf("%s",UNDERLINE);
    if(color == 0) {
      printf("%s",RESET_COLOR);
  } else if(color == 10) {
      printf("%s", RESET_BOLD_COLOR);
  } else if(color == 31) {
      printf("%s", RED);
  } else if(color == 32) {
      printf("%s", GREEN);
  } else if(color == 33) {
      printf("%s", YELLOW);
  } else if(color == 34) {
      printf("%s", BLUE);
  } else if(color == 35) {
      printf("%s", MAGENTA);
  } else if(color == 36) {
      printf("%s", CYAN);
  } else if(color == 131) {
      printf("%s", RED_BOLD);
  } else if(color == 132) {
      printf("%s", GREEN_BOLD);
  } else if(color == 133) {
      printf("%s", YELLOW_BOLD);
  } else if(color == 134) {
      printf("%s", PINK_BOLD);
  } else if(color == 135) {
      printf("%s", MAGENTA_BOLD);
  } else if(color == 136) {
      printf("%s", CYAN_BOLD);
  }
  printf("%s",p);
  printf("%s",RESET_COLOR);
  printf("%s", RESET_UNDERLINE);
}
    printf(" @ ");
}
else if(suser == 0){
  return;
}
}

void exe (char **args){
    if (fork() == 0) {
        execvp(args[0], args);
        perror("Comand execution error");
        exit(EXIT_FAILURE);
    }
    else
        wait(NULL);
}
void copy_files (char **args){
        int first_file = open(args[1], O_RDONLY);
        if (first_file == -1){
            perror("open error");
            return;
        }
        if (open(args[2], O_RDONLY) != -1){
            printf("%s", RED);
            printf("%s already exists!\n", args[2]);
            printf("%s", RESET_COLOR);
            return;
        }
        int newfile = creat(args[2], 0666);
        if (newfile == -1){
            perror("create error");
            return;
        }

        char buf[2048];
        int a;
        while ((a = read(first_file, buf, sizeof(buf))) > 0)
            write(newfile, buf, a);

        close(first_file);
        close(newfile);
}

void process(char **args) {
if(args[1] == NULL) {
        DIR *d;
        struct dirent *ents;
        int i;

        if ((d = opendir("/proc")) == NULL)
              return;
          while ((ents = readdir(d)) != NULL) { /* czytamy zawartosc */
              char *direc, *pid = ents->d_name;

              for (i=0; pid[i]; i++)
                   if (!isdigit(pid[i]))
                         break;
                 if (pid[i])
                       continue; /* jesli nie jest liczba to idziemy dalej */

                 if (fork() == 0) {
                        direc = malloc(i + 10); /* w i jest długość name */
                        sprintf(direc, "/proc/%s/status", pid);
                        printf("pid: %6s  ", pid);
                              fflush(stdout);
                        execlp("sed", "sed", "-n", "1p", direc, NULL);
                  } else
                        wait(NULL);
           }
         closedir(d);
    return;
} else {
if(args[1] != NULL) {
  if(strcmp(args[1],"ram") == 0) {
        FILE *meminfo = fopen("/proc/meminfo", "r");
        if(meminfo == NULL)
          perror("Handle error:");
        char line[256];
        while(fgets(line, sizeof(line), meminfo)) {
          int ram;
          int a_ram;
          if(sscanf(line, "MemTotal: %d kB", &ram) == 1)
          {
      printf("%s", RED);
          printf("MemTotal: %d kB\n", ram);
          }
          if(sscanf(line, "MemFree: %d kB", &a_ram) == 1)
          {
            fclose(meminfo);
        printf("%s", GREEN);
            printf("MemFree: %d kB\n", a_ram);
        printf("%s", RESET_COLOR);
}
        }
      fclose(meminfo);
  }

else if(strcmp(args[1],"cpuinfo") == 0) {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
    char *arg = 0;
    size_t size = 0;
    int i;
    while(getdelim(&arg, &size, 0, cpuinfo) != -1)
    {
       puts(arg);
    }
    free(arg);
    fclose(cpuinfo);
  }

else {
    int val;
    char *endstr;
    val = strtol(args[1],&endstr, 10);
    if(val == 0)
    {
      printf("%s", RED);
      printf("Pid must be a number! Check PID\n");
      printf("%s", RESET_COLOR);
      return;
    }
    printf("%spid        = %d\n",GREEN_BOLD, val);
    printf("%s", RESET_COLOR);
    char filename[1000];
    sprintf(filename, "/proc/%d/stat", val);
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Failed to open stat file: %s - %s! Probably the process with the given PID doesn't exist\n", filename, strerror(errno));
        return;
      }

    char comm[1000];
    char state;
    int ppid;
    int count = fscanf(f, "%*d %s %c %d", comm, &state, &ppid);
    fclose(f);

    if (3 != count) {
        fprintf(stderr, "Failed to read data from stat file: %s !\n%d\n%s\n", filename, count, strerror(errno));
        return;
      }
    printf("%scomm       =%s %s\n",YELLOW, YELLOW_BOLD, comm);
    printf("%sstate      =%s %c\n",CYAN, CYAN_BOLD, state);
    printf("%sparent pid =%s %d\n",MAGENTA, MAGENTA_BOLD, ppid);
    printf("%s", RESET_COLOR);
  }
     }
  }
}

int uline(char **args) {
    if(args[2] != NULL) {
        printf("%s", RED);
        printf("Too many arguments\n");
        printf("%s", RESET_COLOR);
        return;
    }
   else if (args[1] == NULL) {
    printf("%s", RED);
        printf("Add argument\n");
    printf("%s", RESET_COLOR);
        return;
    } else {
         if(strcmp(args[1], "on") == 0)
           underline = 1;
         else if(strcmp(args[1], "off") == 0)
           underline = 0;
         else {
        printf("%s", RED);
            printf("Unknown command! Check 'help' and try again!\n");
        printf("%s", RESET_COLOR);
    }
   }
return underline;
}

int user_color(char **args) {
    if (args[2] == NULL) {
    if(strcmp(args[1], "red") == 0)
           color = 31;
    else if(strcmp(args[1], "green") == 0)
           color = 32;
    else if(strcmp(args[1], "yellow") == 0)
           color = 33;
    else if(strcmp(args[1], "blue") == 0)
           color = 34;
    else if(strcmp(args[1], "magenta") == 0)
           color = 35;
    else if(strcmp(args[1], "cyan") == 0)
           color = 36;
    else if(strcmp(args[1], "reset") == 0)
       color = 0;
    else if(args[1] != NULL) {
           printf("%s", RED);
           printf("Unavailable color! Check available colors on 'help'!\n");
           printf("%s", RESET_COLOR);
           return;
}
    else {
        printf("%s", RED);
            printf("Unknown command! Check 'help' and try again!\n");
            printf("%s", RESET_COLOR);
}
}
  else if(strcmp(args[2],"bold") == 0)  {
        if(strcmp(args[1], "red") == 0)
           color = 131;
        else if(strcmp(args[1], "green") == 0)
           color = 132;
        else if(strcmp(args[1], "yellow") == 0)
           color = 133;
        else if(strcmp(args[1], "pink") == 0)
           color = 134;
        else if(strcmp(args[1], "magenta") == 0)
           color = 135;
        else if(strcmp(args[1], "cyan") == 0)
           color = 136;
    else if(strcmp(args[1], "reset") == 0)
           color = 10;
        else if(args[1] != NULL) {
           printf("%s", RED);
           printf("Unavailable color! Check available colors on 'help'!\n");
           printf("%s", RESET_COLOR);
           return;
     }  else {
        printf("%s", RED);
        printf("The third argument have to be a 'bold' parameter if you want to see bold text in selected color!\n");
        printf("%s", RESET_COLOR);
   }
}  else {
      printf("%s", RED);
      printf("Unknown command! Check 'help' and try again!\n");
      printf("%s", RESET_COLOR);
}
return color;
}

int show_user(char **args) {
  if(args[1] == NULL) {
    printf("%s", RED);
    printf("Add argument\n");
    printf("%s", RESET_COLOR);
    return;
    }
  else {
    if(args[2] != NULL) {
      printf("%s", RED);
      printf("Too many arguments\n");
      printf("%s", RESET_COLOR);
      return;
    } else {
      if(strcmp(args[1],"on") == 0)
        suser = 1;
      else if(strcmp(args[1],"off") == 0)
        suser = 0;
      else {
        printf("%s", RED);
        printf("Unknown command! Check 'help' and try again!\n");
        printf("%s", RESET_COLOR);
      }
    }
  }
}

int pathline(char **args) {
    if(args[2] != NULL) {
        printf("%s", RED);
        printf("Too many arguments\n");
        printf("%s", RESET_COLOR);
        return;
    }
   else if (args[1] == NULL) {
          printf("%s", RED);
          printf("Add argument\n");
          printf("%s", RESET_COLOR);
        return;
 } else {
        if(strcmp(args[1], "on") == 0)
           underline_p = 1;
        else if(strcmp(args[1], "off") == 0)
           underline_p = 0;
        else {
              printf("%s", RED);
            printf("Unknown command! Check 'help' and try again!\n");
              printf("%s", RESET_COLOR);
             }
     }
     return underline_p;
}

int path_color(char **args) {
    if (args[2] == NULL) {
         if(strcmp(args[1], "red") == 0)
           pcolor = 31;
       else if(strcmp(args[1], "green") == 0)
           pcolor = 32;
         else if(strcmp(args[1], "yellow") == 0)
           pcolor = 33;
         else if(strcmp(args[1], "blue") == 0)
           pcolor = 34;
         else if(strcmp(args[1], "magenta") == 0)
           pcolor = 35;
         else if(strcmp(args[1], "cyan") == 0)
           pcolor = 36;
       else if(strcmp(args[1], "reset") == 0)
             pcolor = 0;
         else if(args[1] != NULL) {
           printf("%s", RED);
           printf("Unavailable color! Check available colors on 'help'!\n");
           printf("%s", RESET_COLOR);
           return;
}
    else {
     printf("%s", RED);
     printf("Unknown command! Check 'help' and try again!\n");
     printf("%s", RESET_COLOR);
}
    }
  else if(strcmp(args[2],"bold") == 0)  {
        if(strcmp(args[1], "red") == 0)
           pcolor = 131;
        else if(strcmp(args[1], "green") == 0)
           pcolor = 132;
        else if(strcmp(args[1], "yellow") == 0)
           pcolor = 133;
        else if(strcmp(args[1], "pink") == 0)
           pcolor = 134;
        else if(strcmp(args[1], "magenta") == 0)
           pcolor = 135;
        else if(strcmp(args[1], "cyan") == 0)
           pcolor = 136;
          else if(strcmp(args[1], "reset") == 0)
           pcolor = 10;
        else if(args[1] != NULL) {
           printf("%s", RED);
           printf("Unavailable color! Check available colors on 'help'!\n");
           printf("%s", RESET_COLOR);
           return;
     }  else {
        printf("%s", RED);
        printf("The third argument have to be a 'bold' parameter if you want to see bold text in selected color!\n");
        printf("%s", RESET_COLOR);
   }
}  else {
      printf("%s", RED);
      printf("Unknown command! Check 'help' and try again!\n");
      printf("%s", RESET_COLOR);
}
return pcolor;
}

int show_path(char **args) {
  if(args[1] == NULL) {
    printf("%s", RED);
    printf("Add argument\n");
    printf("%s", RESET_COLOR);
    return;
    }
  else {
    if(args[2] != NULL) {
       printf("%s", RED);
       printf("Too many arguments\n");
       printf("%s", RESET_COLOR);
       return;
    } else {
      if(strcmp(args[1],"on") == 0)
        spath = 1;
      else if(strcmp(args[1],"off") == 0)
        spath = 0;
      else {
        printf("%s", RED);
        printf("Unknown command! Check 'help' and try again!\n");
        printf("%s", RESET_COLOR);
      }
    }
  }
}
