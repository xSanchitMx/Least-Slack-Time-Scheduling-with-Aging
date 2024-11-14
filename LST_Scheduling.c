#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int id;
    int arrival_time;
    int burst_time;
    int deadline;
    int starvation_time;
};

int find_least_slack_process(struct Process processes[], int n, int current_time)
{
    int least_slack_index = -1;
    int least_slack = 100000;

    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
        {
            int slack_time = processes[i].deadline - current_time - processes[i].burst_time;
            if (slack_time < least_slack)
            {
                least_slack = slack_time;
                least_slack_index = i;
            }
        }
    }

    return least_slack_index;
}

int main()
{
    int num_processes;
    int current_time = 0, idle_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    struct Process processes[num_processes];

    for (int i = 0; i < num_processes; i++)
    {
        printf("Enter details for Process %d:\n", i + 1);
        printf("Process ID: ");
        scanf("%d", &processes[i].id);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("Deadline: ");
        scanf("%d", &processes[i].deadline);
        printf("\n");
    }

    printf("Process scheduling using Least Slack Time (LST) with Arrival Time:\n\n");

    while (1)
    {
        int process_index = find_least_slack_process(processes, num_processes, current_time);
        if (process_index == -1)
        {
            idle_time++;
            current_time++;
            printf(" %d idle %d,", current_time - 1, current_time);
            continue;
        }
        processes[process_index].burst_time--;
        current_time++;
        printf(" %d %d %d,", current_time - 1, processes[process_index].id, current_time);

        int all_done = 1;
        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].burst_time > 0)
            {
                all_done = 0;
                break;
            }
        }
        if (all_done)
            break;
    }
    printf("\nAll processes finished.\n");
    return 0;
}
