/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 30, 2013
 *************************************************************************/

/*************************************************************************
  Compile with -D__DEBUG option for enabling debugging jigs in the code

  Filename: io.c

  Description:
  Parses the input file in AIF and creates the initial data structures
  Memory allocation of the data structures is done in this routine
    1. File pointer is always non-NULL.



 *************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <io.h>
#include <main.h>

  int parse_task_type(char *);
  int intfc_mode(char *);
  int get_index(char *, t_task_interface *,   int);
void print_error(int);

void print_error(int ecode) {
    char err_mesg[__SIZE_MAX_BUFF];

    switch (ecode) {
        case __LESSARGS:
            strcpy(err_mesg, "Insufficient number of arguments.");
            break;
        case __AIF_FILE:
            strcpy(err_mesg, "Opening AIF file.");
            break;
        case __LOG_FILE:
            strcpy(err_mesg, "Opening Log file.");
            break;
        case __OPT_FILE:
            strcpy(err_mesg, "Opening Options file.");
            break;
        case __RES_FILE:
            strcpy(err_mesg, "Opening Resources file.");
            break;
        case __UNRECOPT:
            strcpy(err_mesg, "Unrecognised option at the command line.");
    }
    //fprintf(log_strm, "Error: %s\n", err_mesg);
    fprintf(stderr, "Error: %s\n", err_mesg);
    //fclose(log_strm);
    exit(ecode);
}

int parse_impl(FILE *in_strm, t_task_impl *task_impl) {
    int err = 0;
    int index = 0;
    char buff[__SIZE_MAX_BUFF], *token;
    int add_impl_num = 0;
    int mul_impl_num = 0;
    int sub_impl_num = 0;
    int div_impl_num = 0;
    int lt_impl_num = 0;
    int gt_impl_num = 0;
    int lte_impl_num = 0;
    int gte_impl_num = 0;

    //read through the lines of the arch file
    //fprintf(log_strm, "Reading the arch file...\n");

    while (fgets(buff, __SIZE_MAX_BUFF, in_strm)) {
        //fprintf(log_strm, "%s\n", buff);
        token = strtok(buff, " ");

        if (!(*token) || *token == '#' || *token == '\n')
            continue;
        else if (!strcasecmp(token, "TASK1")) {
            add_impl_num++;
            index++;
            (task_impl + index)->impl = add_impl_num;
            (task_impl + index)->type = __ADD;
        } else if (!strcasecmp(token, "TASK2")) {
            mul_impl_num++;
            index++;
            (task_impl + index)->impl = mul_impl_num;
            (task_impl + index)->type = __MULT;
        } else if (!strcasecmp(token, "TASK3")) {
            sub_impl_num++;
            index++;
            (task_impl + index)->impl = sub_impl_num;
            (task_impl + index)->type = __SUB;
        } else if (!strcasecmp(token, "TASK4")) {
            div_impl_num++;
            index++;
            (task_impl + index)->impl = div_impl_num;
            (task_impl + index)->type = __DIV;
        } else if (!strcasecmp(token, "TASK5")) {
            lt_impl_num++;
            index++;
            (task_impl + index)->impl = lt_impl_num;
            (task_impl + index)->type = __LT;
        } else if (!strcasecmp(token, "TASK6")) {
            gt_impl_num++;
            index++;
            (task_impl + index)->impl = gt_impl_num;
            (task_impl + index)->type = __GT;
        } else if (!strcasecmp(token, "TASK7")) {
            lte_impl_num++;
            index++;
            (task_impl + index)->impl = lte_impl_num;
            (task_impl + index)->type = __LTE;
        } else if (!strcasecmp(token, "TASK8")) {
            gte_impl_num++;
            index++;
            (task_impl + index)->impl = gte_impl_num;
            (task_impl + index)->type = __GTE;
        } else
            err = __RESFILE0;


        if ((token = strtok(NULL, " "))) {
            (task_impl + index)->id = index;
            (task_impl + index)->columns = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if ((token = strtok(NULL, " "))) {
            (task_impl + index)->rows = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if ((token = strtok(NULL, " \n"))) {
            (task_impl + index)->reconfig_time = atoi(token);
        }
        if ((token = strtok(NULL, " "))) {
            (task_impl + index)->latency = atoi(token);
        } else {
            err = __RESFILE2;
            break;
        }
        if ((token = strtok(NULL, " \n"))) {
            (task_impl + index)->reconfig_pwr = atoi(token);
        }
        if ((token = strtok(NULL, " \n"))) {
            (task_impl + index)->exec_pwr = atoi(token);
        } else {
            err = __RESFILE3;
            break;
        }
    }

    // the first element is reserved for storing the number of elements
    // in the list.

    task_impl->columns = index;

    return (err);
}


int parse_res(FILE *in_strm, t_task_type *task_type) {
    int err = 0;
    int index = 0, count_task_type = 0;
    char buff[__SIZE_MAX_BUFF], *token;

    count_task_type = 0;

    //read through the lines of the resources file
    //fprintf(log_strm, "Reading the resources file...\n");

    while (fgets(buff, __SIZE_MAX_BUFF, in_strm)) {
        //fprintf(log_strm, "%s\n", buff);
        token = strtok(buff, " ");

        if (!(*token) || *token == '#' || *token == '\n')
            continue;
        else if (!strcasecmp(token, "TASK1"))
            index = __ADD;
        else if (!strcasecmp(token, "TASK2"))
            index = __MULT;
        else if (!strcasecmp(token, "TASK3"))
            index = __SUB;
        else if (!strcasecmp(token, "TASK4"))
            index = __DIV;
        else if (!strcasecmp(token, "TASK5"))
            index = __LT;
        else if (!strcasecmp(token, "TASK6"))
            index = __GT;
        else if (!strcasecmp(token, "TASK7"))
            index = __LTE;
        else if (!strcasecmp(token, "TASK8"))
            index = __GTE;
        else
            err = __RESFILE0;

        if (count_task_type < index)
            count_task_type = index;

        if ((token = strtok(NULL, " "))) {
            (task_type + index)->columns = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if ((token = strtok(NULL, " "))) {
            (task_type + index)->rows = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if ((token = strtok(NULL, " "))) {
            (task_type + index)->latency = atoi(token);
        } else {
            err = __RESFILE2;
            break;
        }
        if ((token = strtok(NULL, " \n"))) {
            (task_type + index)->reconfig_time = atoi(token);
        } else {
            err = __RESFILE3;
            break;
        }
    }

    // the first element is reserved for storing the number of elements
    // in the list.

    task_type->columns = count_task_type;

    return (err);
}


int parse_aif(FILE *in_strm, t_task *task, t_task_interface *task_interface) {
    int err = 0;
    int count_intfc = 0, count_task = 0;
    t_task *task_curr;
    t_task_interface *task_interface_curr;
    char buff[__SIZE_MAX_BUFF], *token;

    count_intfc = 1;
    count_task = 1;
    task_curr = task + count_task;
    task_interface_curr = task_interface + count_intfc;

    //fprintf(log_strm,"Reading the aif file...\n");
    while (fgets(buff, __SIZE_MAX_BUFF, in_strm)) {
        token = strtok(buff, " \n");
        if (!strcasecmp(token, "inputs")
                || !strcasecmp(token, "outputs")
                || !strcasecmp(token, "regs")) {
            int mode = intfc_mode(token);
            while ((token = strtok(NULL, " \n"))) {
                strcpy(task_interface_curr->name, token);
                task_interface_curr->mode = mode;
                if ((token = strtok(NULL, " \n")))
                    task_interface_curr->width = atoi(token);
                count_intfc++;
                task_interface_curr = task_interface + count_intfc;
            }
        } else if (!strcasecmp(token, "end")) {
            break;
        } else {
            int task_attrib = 0;
            do {
                // ERROR in AIF FORMAT
                assert(task_attrib <= 6);
                switch (task_attrib) {
                    case 0: //Task name
                        strcpy(task_curr->name, token);
                        break;
                    case 1: //Task type
                        task_curr->type = parse_task_type(token);
                        break;
                    case 2: //Task width
                        task_curr->width = atoi(token);
                        break;
                    case 3: //input 1
                        task_curr->input1 = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                    case 4: //input 2
                        task_curr->input2 = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                    case 5: //output
                        task_curr->output = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                }
                task_attrib++;
            } while ((token = strtok(NULL, " \n")));
            count_task++;
            task_curr = task + count_task;
        }
    }

    task_interface->mode = __INVALID;
    task_interface->width = count_intfc - 1;
    task->type = __INVALID;
    task->width = count_task - 1;

    return err;
}

int parse_task_type(char *token) {
    int type = 0;

    if (!strcasecmp(token, "TASK1")) type = __ADD;
    else if (!strcasecmp(token, "TASK2")) type = __MULT;
    else if (!strcasecmp(token, "TASK3")) type = __SUB;
    else if (!strcasecmp(token, "TASK4")) type = __DIV;
    else if (!strcasecmp(token, "TASK5")) type = __LT;
    else if (!strcasecmp(token, "TASK6")) type = __GT;
    else if (!strcasecmp(token, "TASK7")) type = __LTE;
    else if (!strcasecmp(token, "TASK8")) type = __GTE;

    return type;
}


int intfc_mode(char *token) {
    int mode = 0;

    if (!strcasecmp(token, "inputs")) mode = __INPUT;
    else if (!strcasecmp(token, "outputs")) mode = __OUTPUT;
    else if (!strcasecmp(token, "regs")) mode = __REGISTER;

    return mode;
}


int get_index(char *token, t_task_interface* task_interface, int count){
    int index = 0;
    int i = 0;
    //t_task_interface *task_interface_curr;

    for (i = 1; i <= count; i++) {
        if (!strcasecmp((task_interface + i)->name, token)) {
            index = i;
            break;
        }
    }

    return (index) ? index : -1;
}


void display_array(int *start, int dim, int *dim_size) {
    int i = 0, j = 0;

    switch (dim) {
        case 1:
        {
            for (i = 0; i<*dim_size; i++) {
                printf("%d ", *(start + i));
            }
            break;
        }
        case 2:
        {
            for (i = 0; i<*dim_size; i++) {
                for (j = 0; j<*(dim_size + 1); j++) {
                    printf("%d ", *(start + i * (*(dim_size + 1)) + j));
                }
                printf("\n");
            }
            break;
        }
    }

    printf("\n");
}


void display_task(t_task *task, t_task_interface *task_interface) {
    int i = 0;
    int num_nodes = task->width;

    fprintf(stdout, "name type width latency reconfig_time columns rows input1 input2 output exec_sched " \
                 "reconfig_sched leftmost_column bottommost_row conf_power exec_power impl\n");
    
    for (i = 1; i <= num_nodes; i++) {
        printf("%s %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d\n", (task + i)->name, (task + i)->type, (task + i)->impl, (task + i)->width, (task + i)->latency, (task + i)->reconfig_time, (task + i)->columns, (task + i)->rows, (task + i)->input1, (task + i)->input2, (task + i)->output, (task_interface + (task + i)->output)->mode, (task + i)->exec_sched, (task + i)->reconfig_sched, (task + i)->leftmost_column, (task + i)->bottommost_row, (task + i)->reconfig_pwr, (task + i)->exec_pwr);
    }

}


void display_task_type(t_task_type *task_type) {
    int i = 0;
    int num_task_types = task_type->columns;

    for (i = 1; i < num_task_types; i++) {
        printf("%d %d %d %d %d\n", i, (task_type + i)->columns, (task_type + i)->rows, (task_type + i)->latency, (task_type + i)->reconfig_time);
    }

}


void display_task_impl(t_task_impl *task_impl) {
    int i = 0;
    int num_task_impls = task_impl->columns;

    printf("%d\n", num_task_impls);
    for (i = 1; i <= num_task_impls; i++) {
        printf("%d %d %d %d %d %d %d %d %d\n", i, (task_impl + i)->type, (task_impl + i)->impl, (task_impl + i)->columns, (task_impl + i)->rows, (task_impl + i)->latency, (task_impl + i)->reconfig_time, (task_impl + i)->reconfig_pwr, (task_impl + i)->exec_pwr);
    }
}


void set_task_parameter(t_task *task, t_task_type *task_type) {
    int i = 0;
    int num_nodes = task->width;

    for (i = 1; i <= num_nodes; i++) {
        (task + i)->columns = (task_type + (task + i)->type)->columns;
        (task + i)->rows = (task_type + (task + i)->type)->rows;
        (task + i)->latency = (task_type + (task + i)->type)->latency;
        (task + i)->reconfig_time = (task_type + (task + i)->type)->reconfig_time;
    }

}
