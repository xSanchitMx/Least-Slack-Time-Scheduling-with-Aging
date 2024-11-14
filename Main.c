#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Process
{
    char id[5];
    int arrival_time;
    int burst_time;
    int deadline;
    int completion_time;
    int slack_time;
    int starvation_time;
    double slack_time_scaled;
    double starvation_time_scaled;
    double priority;
};
double alpha = 0.5;

int is_all_completed(struct Process processes[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].burst_time > 0)
            return 0;
    }
    return 1;
}

void find_slack_time(struct Process processes[], int n, int current_time)
{

    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
        {
            processes[i].slack_time = processes[i].deadline - current_time - processes[i].burst_time;
        }
    }
}

int max_slack_time, min_slack_time, max_starvation_time, min_starvation_time;
void find_max_min(struct Process processes[], int n, int current_time)
{
    int this_slack = 1, this_starvation = 1;
    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
        {
            if (this_slack)
            {
                this_slack--;
                max_slack_time = processes[i].slack_time;
                min_slack_time = processes[i].slack_time;
            }
            else
            {
                if (processes[i].slack_time < min_slack_time)
                    min_slack_time = processes[i].slack_time;
                if (processes[i].slack_time > max_slack_time)
                    max_slack_time = processes[i].slack_time;
            }
            if (this_starvation)
            {
                this_starvation--;
                max_starvation_time = processes[i].starvation_time;
                min_starvation_time = processes[i].starvation_time;
            }
            else
            {
                if (processes[i].starvation_time < min_starvation_time)
                    min_starvation_time = processes[i].starvation_time;
                if (processes[i].starvation_time > max_starvation_time)
                    max_starvation_time = processes[i].starvation_time;
            }
        }
    }
}

void scale_starvation_time(struct Process processes[], int n, int current_time)
{
    if (min_starvation_time == max_starvation_time)
    {
        for (int i = 0; i < n; i++)
            processes[i].starvation_time_scaled = 0.5;
        return;
    }
    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
            processes[i].starvation_time_scaled = (double)(processes[i].starvation_time - min_starvation_time) / (max_starvation_time - min_starvation_time);
    }
}

void scale_slack_time(struct Process processes[], int n, int current_time)
{
    if (min_slack_time == max_slack_time)
    {
        for (int i = 0; i < n; i++)
            processes[i].slack_time_scaled = 0.5;
        return;
    }
    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
            processes[i].slack_time_scaled = 1 - (double)(processes[i].slack_time - min_slack_time) / (max_slack_time - min_slack_time);
    }
}

void find_priority(struct Process processes[], int n, int current_time)
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
            processes[i].priority = exp(alpha * processes[i].slack_time_scaled) + exp((1 - alpha) * processes[i].starvation_time_scaled);
    }
}

int find_highest_priority_process(struct Process processes[], int n, int current_time)
{
    int highest_priority_index = -1;
    double highest_priority = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival_time <= current_time && processes[i].burst_time > 0)
        {
            if (processes[i].priority > highest_priority)
            {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }
    }

    return highest_priority_index;
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
        scanf("%s", &processes[i].id);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("Deadline: ");
        scanf("%d", &processes[i].deadline);
        processes[i].starvation_time = 0;
        printf("\n");
    }

    // printf("Process scheduling using Least Slack Time (LST) with Arrival Time:\n\n");

    while (!is_all_completed(processes, num_processes))
    {
        find_slack_time(processes, num_processes, current_time);
        find_max_min(processes, num_processes, current_time);
        scale_slack_time(processes, num_processes, current_time);
        scale_starvation_time(processes, num_processes, current_time);
        find_priority(processes, num_processes, current_time);

        int process_index = find_highest_priority_process(processes, num_processes, current_time);
        if (process_index == -1)
        {
            idle_time++;
            current_time++;
            printf(" %d idle %d,", current_time - 1, current_time);
            continue;
        }
        processes[process_index].burst_time--;
        current_time++;
        if (processes[process_index].burst_time == 0)
            processes[process_index].completion_time = current_time;
        printf(" %d %s %d,", current_time - 1, processes[process_index].id, current_time);

        for (int i = 0; i < num_processes; i++)
        {
            if (processes[i].burst_time > 0 && processes[i].arrival_time < current_time && i != process_index)
            {
                processes[i].starvation_time++;
            }
        }
    }
    printf("\nAll processes finished.\n");

    for (int i = 0; i < num_processes; i++)
        printf("Process %s - Completion Time: %d, Deadline: %d\n", processes[i].id, processes[i].completion_time, processes[i].deadline);
    return 0;
}
