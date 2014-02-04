#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dlist.h"

#if 1
#define INT_DATA
#else
#define STR_DATA
#endif

// proceed cmd args
#define MAX_CMD_CNT 6
#define MAX_CMD_LEN 13
#define MAX_ARG_LEN 13
//#define MAX_BUF_LEN (MAX_CMD_LEN + MAX_ARG_LEN + MAX_ARG_LEN + 5)
#define MAX_BUF_LEN (MAX_CMD_LEN + MAX_ARG_LEN + MAX_ARG_LEN)

static void get_cmd_args(char cmd[], char arg1[], char arg2[])
{
    char buf[MAX_BUF_LEN];
    int start_idx, end_idx, word_cnt;

    // get buf string
    if (NULL == fgets(buf, MAX_BUF_LEN, stdin))
    {
        perror("fgets");
        exit(EXIT_FAILURE);
    }

    memset(cmd, '\0', MAX_CMD_LEN);
    memset(arg1, '\0', MAX_ARG_LEN);
    memset(arg2, '\0', MAX_ARG_LEN);

    start_idx = 0;	// (start_idx, end_idx) means a word
    end_idx = 0;
    word_cnt = 0;

    while (buf[end_idx] != '\n')
    {
        if (buf[end_idx] == ' ')
        {
            if (word_cnt == 0)
            {
                strncpy(cmd, &buf[start_idx], end_idx - start_idx);
            }
            else if (word_cnt == 1)
            {
                strncpy(arg1, &buf[start_idx], end_idx - start_idx);
            }

            word_cnt++;
            start_idx = end_idx + 1;
        }
    
        end_idx++;
    }
    
    if (word_cnt == 0)
    {
        strncpy(cmd, &buf[start_idx], end_idx - start_idx);
    }
    else if (word_cnt == 1)
    {
        strncpy(arg1, &buf[start_idx], end_idx - start_idx);
    }
    else if (word_cnt == 2)
    {
        strncpy(arg2, &buf[start_idx], end_idx - start_idx);
    }
}

static void print_data_func(void *remaining_size, void *data)
{
#ifdef INT_DATA
    printf("%d", *(int*)data);
#else // #ifdef STR_DATA
    printf("\'%s\'", (char*)data);
#endif
    (*((int*)remaining_size))--;
    if (*((int*)remaining_size) != 0)
    {
        printf(", ");
    }
}

static void do_printall(DList *thiz)
{
    size_t size;

    size = dlist_size(thiz);
    printf("[");
    dlist_foreach(thiz, print_data_func, (void*)&size);
    printf("]\n");
}

#ifdef INT_DATA
static void find_max_func(void *max_val, void *data)
{
    if ( *((int*)data) > *((int*)max_val) )
    {
        *((int*)max_val) = *((int*)data);
    }
}
#endif

static void do_max(DList *dlistp)
{
#ifdef STR_DATA
    printf("Not supported for string data.\n");
#else // INT_DATA
    int max_val;

    max_val = 0;
    dlist_foreach(dlistp, find_max_func, (void*)&max_val);
    printf("%d\n", max_val);
#endif
}

#ifdef INT_DATA
static void accum_func(void *sum, void *data)
{
   *(int*)sum += *(int*)data; 
}
#endif

static void do_sum(DList *dlistp)
{
#ifdef STR_DATA
    printf("Not supported for string data.\n");
#else // INT_DATA
    int sum;
    sum = 0;
    dlist_foreach(dlistp, accum_func, (void *)&sum);
    printf("%d\n", sum);
#endif
}

#ifdef STR_DATA
static void to_upper_func(void *ctx, void *data)
{
    char *cp;
    for (cp = (char *)data; *cp != '\0'; cp++)
    {
        if (islower(*cp))
        {
            *cp = toupper(*cp);
        }
    }
}
#endif

static void do_upper(DList *dlistp)
{
#ifdef INT_DATA
    printf("Not supported for int data.\n");
#else // STR_DATA
    dlist_foreach(dlistp, to_upper_func, NULL);
#endif
}

static void *get_pdata(char *data_str)
{
    void *pdata;

#ifdef INT_DATA
    pdata = (void *)malloc(sizeof(int));
#else // STR_DATA
    pdata = (void *)malloc(MAX_ARG_LEN*sizeof(char));
#endif

    if (pdata == NULL)
    {
        perror("pdata malloc failed");
        exit(EXIT_FAILURE);
    }

#ifdef INT_DATA
    *(int *)pdata = atoi(data_str); // TBD: if atoi() failed ... (strtol())
#else // STR_DATA
    strcpy((char*)pdata, data_str);
#endif

    return pdata;
}

static void do_append(DList *dlistp, char *data_str)
{
    size_t idx;
    void *pdata;

    idx = dlist_size(dlistp);
    pdata = get_pdata(data_str);

    dlist_insert(dlistp, idx, pdata);
}

static void do_insert(DList *dlistp, char *idx_str, char *data_str)
{
    size_t idx;
    void *pdata;

    idx = atoi(idx_str);
    pdata = get_pdata(data_str);

    dlist_insert(dlistp, idx, pdata);
}

static void do_delete(DList *dlistp, char *idx_str)
{
    int idx;

    idx = atoi(idx_str);
    dlist_delete(dlistp, idx);
}

static void do_read(DList *dlistp, char *file_name)
{
    FILE *infile;
    int idx;
    char data_buf[MAX_ARG_LEN];

    if ((infile = fopen(file_name, "r")) == NULL)
    {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    memset(data_buf, '\0', MAX_ARG_LEN);
    while (fgets(data_buf, MAX_ARG_LEN, infile) != NULL)
    {
        // replace ending '\n' with '\0'
        for (idx = 0; data_buf[idx] != '\n'; idx++);
        data_buf[idx] = '\0';

        do_append(dlistp, data_buf);
        memset(data_buf, '\0', MAX_ARG_LEN);
    }

    fclose(infile);
}


int main(void)
{
    unsigned char quit_flag;

    char cmd[MAX_CMD_LEN];
    char arg1[MAX_ARG_LEN];
    char arg2[MAX_ARG_LEN];

    DList *dlistp;
    dlistp = dlist_create();

    quit_flag = 0;
    while(1)
    {
        printf("\n>> ");

        get_cmd_args(cmd, arg1, arg2);

        if (0 == strcmp(cmd, "printall"))
        {
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "len"))
        {
            printf("%zu\n", dlist_size(dlistp));
        }
        else if (0 == strcmp(cmd, "append"))
        {
            do_append(dlistp, arg1);
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "insert"))
        {
            do_insert(dlistp, arg1, arg2);
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "delete"))
        {
            do_delete(dlistp, arg1);
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "read"))
        {
            do_read(dlistp, arg1);
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "max"))
        {
            do_max(dlistp);
        }
        else if (0 == strcmp(cmd, "sum"))
        {
            do_sum(dlistp);
        }
        else if (0 == strcmp(cmd, "upper"))
        {
            do_upper(dlistp);
            do_printall(dlistp);
        }
        else if (0 == strcmp(cmd, "quit"))
        {
            dlist_destroy(dlistp);
            quit_flag = 1;
        }
        else
        {
            printf("Unknown Cmd\n");
            continue;
        }

        if (quit_flag == 1)
        {
            break;
        }
    }

    return EXIT_SUCCESS;
}
