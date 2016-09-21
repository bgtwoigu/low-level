// https://stackoverflow.com/questions/2377811/tracking-the-death-of-a-child-process
// https://stackoverflow.com/questions/7281894/how-do-i-chain-stdout-in-one-child-process-to-stdin-in-another-child-in-c
// https://stackoverflow.com/questions/9457784/how-to-use-dup2-close-correctly-to-connect-these-three-processes
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <poll.h>
#include <wait.h>

struct _env {
    int count;
    unsigned int idx;
    float interval;
} env;


void print_usage(FILE* output, char* progname, int exit_code) {
    fprintf(output, "usage: %s\n", progname);
    exit(exit_code);
}
typedef struct _stats {
    unsigned int sigfault;
    unsigned int sigtrap;
} stats;

/**
 * Specifies the maximum number of clients to keep track of.
 */
#define MAX_CLIENT_COUNT 100

/**
 * Tracks clients by storing their process IDs and pipe file descriptors.
 */
struct process_table {
    pid_t clientpids[MAX_CLIENT_COUNT];
    struct pollfd clientfds[MAX_CLIENT_COUNT];
    struct pollfd clientfds_in[MAX_CLIENT_COUNT];
} PT;

/**
 * Initializes the process table. -1 means the entry in the table is available.
 */
void initialize_table() {
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        PT.clientfds[i].fd = -1;
        PT.clientfds_in[i].fd = -1;
    }
}

/**
 * Returns the index of the next available entry in the process table.
 */
int get_next_available_entry() {
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        if (PT.clientfds[i].fd == -1) {
            return i;
        }
    }
    return -1;
}

/**
 * Adds information about a new client to the process table.
 */
void add_process_to_table(int i, pid_t pid, int fd, int fd_in) {
    PT.clientpids[i] = pid;
    PT.clientfds[i].fd = fd;
    PT.clientfds_in[i].fd = fd_in;
}

/**
 * Removes information about a client from the process table.
 */
void remove_process_from_table(int i) {
    PT.clientfds[i].fd = -1;
    PT.clientfds_in[i].fd = -1;
}

/**
 * Cleans up any dead child processes from the process table.
 */
void reap_zombie_processes() {
    int p = poll(PT.clientfds, MAX_CLIENT_COUNT, 0);

    if (p > 0) {
        char buf[256];
        for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
            short revents = PT.clientfds[i].revents;
            if ((revents & POLLIN) != 0) {
                read(PT.clientfds[i].fd, buf, 256);
                printf("[%d] stdout: %s", PT.clientpids[i], buf);
            }
            /* Has the pipe closed? */
            if ((revents & POLLHUP) != 0) {
                //printf("[%d] done\n", PT.clientpids[i]);
                waitpid(PT.clientpids[i], NULL, 0);
                remove_process_from_table(i);
            }
        }
    }
}

void launch(char* prog_args[]) {
    int err = execvp(prog_args[0], prog_args);

    if (err < 0) {
        perror("execvp");
        exit(2);
    }
}

// http://advancedlinuxprogramming.com/listings/chapter-2/getopt_long.c
void handle_options(int argc, char** argv, char* progname) {
    env.count = -1;
    /* A string listing valid short options letters.  */
    const char* const short_options = "hc:i:v";
    /* An array describing valid long options.  */
    const struct option long_options[] = {
        { "help",     no_argument, NULL, 'h' },
        { "count",    1, NULL, 'c' },
        { "interval",  1, NULL, 'i' },
        { "verbose",  0, NULL, 'v' },
        { NULL,       0, NULL, 0   }   /* Required at end of array.  */
    };

    int next_option;
    const char* output_filename = NULL;
    int verbose = 0;

    do {
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);
        switch (next_option) {
            case 'h':   /* -h or --help */
                /* User has requested usage information.  Print it to standard
                 *          output, and exit with exit code zero (normal termination).  */
                print_usage (stdout, progname, 0);

            case 'c':
                env.count = atoi(optarg);
                break;
            case 'i':
                env.interval = atof(optarg);
                break;
            case 'v':   /* -v or --verbose */
                verbose = 1;
                break;

            case '?':   /* The user specified an invalid option.  */
                /* Print usage information to standard error, and exit with exit
                 *          code one (indicating abonormal termination).  */
                print_usage (stderr, progname, 1);

            case -1:    /* Done with options.  */
                break;

            default:    /* Something else: unexpected.  */
                abort ();
        }
    }
    while (next_option != -1);

    /* Done with options.  OPTIND points to first non-option argument.
     *      For demonstration purposes, print them if the verbose option was
     *           specified.  */

    if (optind == argc) {
        print_usage(stderr, progname, 1);
    }

    // getopt reorders argv so that at the end the not-options are from here
    env.idx = optind;
}

char lopper[] = "|/-\\";
unsigned int lopper_idx = 0;

/**
 * Main program
 */
int main(int argc, char* argv[]) {
    /* Initialize the process table */
    initialize_table();
    handle_options(argc, argv, argv[0]);

    char** prog_args = argv + env.idx;

    fprintf(stderr, " [+] trying to lanch ");

    unsigned int idx;
    char* a;

    for (idx = 0 ; a ; a = prog_args[idx], idx++)
        fprintf(stderr, "%s ", a);

    fprintf(stderr, "\n");

    stats s;

    memset(&s, 0x00, sizeof(stats));

    while (env.count == -1 || env.count > 0) {
        env.count = env.count == -1 ? -1 : env.count - 1;
        lopper_idx++;
        fprintf(stderr, "[%c] %d segfault: %d sigabort: %d\r", lopper[lopper_idx % (sizeof(lopper) - 1)], lopper_idx, s.sigfault, s.sigtrap);
        //sleep(.05);
        if (env.interval)
            sleep(env.interval);

        /*
        int next_entry;
        while ((next_entry = get_next_available_entry()) == -1) {
            sleep(1);
            reap_zombie_processes();
        }*/

        /* Create the pipes */
        /*int p_in[2];
        int p_out[2];
        pipe(p_in);
        pipe(p_out);*/

        /* Fork off a child process. */
        pid_t cpid = fork();

        if (cpid == 0) {
            /* 
             * Child process: attach one pipe to stdin and one to stdout
             * so that if a process does not die we can communicate with it.
             */
            //close(p_in[0]);
            //close(p_out[0]);

            /*if(dup2(p_in[1], STDIN_FILENO) == -1)
                exit(-1);
            if(dup2(p_out[1], STDOUT_FILENO) == -1)
                exit(-2);*/

            launch(prog_args);
        } else {
            /* Parent process */
            //close(p_in[1]);
            //close(p_out[1]);
            //add_process_to_table(next_entry, cpid, p_out[0], p_in[0]);
            //printf("[%d] started\n", cpid);

            //char foo[4096] = "porcodio";

            //int nbytes = write(p_in[0], foo, sizeof(foo));

            //nbytes = read(p_out[0], foo, sizeof(foo));
            //printf("Output: (%.*s)\n", nbytes, foo);

            //reap_zombie_processes();
            int status, signal;
            wait(&status);

            if(WIFEXITED(status)) {
                //fprintf(stderr, " [I] child %d exit with status %d\n", cpid, status);
                //break;
            }

            if (WIFSIGNALED(status)) {
                signal = WTERMSIG(status);
                //fprintf(stderr, " [I] child %d received signal %d\n", cpid, signal);
                if (signal == 6)
                    s.sigtrap++;
                    //break;
                else if (signal == 11)
                    s.sigfault++;
            }
        }
        //break;
    }

    return 0;
}

