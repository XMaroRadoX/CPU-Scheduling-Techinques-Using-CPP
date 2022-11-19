#include <bits/stdc++.h>
#include <regex>
#include <string.h>
using namespace std;

// STRUCTURE
struct process
{
    string pname;
    int arrivalTime;
    int serviceTime;
    int tempServiceTime;
    int waitTime = 0;
    int finishTime = 0;
    int feedback_level = 0;
    int turnAroundTime = 0;
    float normTurnTime = 0;
    int timesliceinFB2i = 1;
    vector<string> timeProcessing;
};
struct schedule
{
    int schedulerNumber;
    int quantum;
    float meanTurnAround = 0.0;
    float meanNormTurn = 0.0;
    float totalWaitTime = 0.0;
    float totalTurnAroundTime = 0.0;
    float totalNormTurn = 0.0;
    string schedulerName;
};
schedule scheduleMaker(int schNo)
{
    schedule schedule;
    schedule.schedulerNumber = schNo;
    schedule.quantum = 0;
    switch (schNo)
    {
    case 1:
        schedule.schedulerName = "FCFS";
        break;
    case 2:
        schedule.schedulerName = "RR";
        break;
    case 3:
        schedule.schedulerName = "SPN";
        break;
    case 4:
        schedule.schedulerName = "SRT";
        break;
    case 5:
        schedule.schedulerName = "HRRN";
        break;
    case 6:
        schedule.schedulerName = "FB-1";
        schedule.quantum = 1;
        break;
    case 7:
        schedule.schedulerName = "FB-2i";
        schedule.quantum = 1;
        break;
    case 8:
        schedule.schedulerName = "Aging";
        break;

    default:
        break;
    }
    return schedule;
}
schedule scheduleMaker(int schNo, int quantum)
{
    schedule schedule;
    schedule.schedulerNumber = schNo;
    schedule.quantum = quantum;
    switch (schNo)
    {
    case 1:
        schedule.schedulerName = "FCFS";
        break;
    case 2:
        schedule.schedulerName = "RR";
        break;
    case 3:
        schedule.schedulerName = "SPN";
        break;
    case 4:
        schedule.schedulerName = "SRT";
        break;
    case 5:
        schedule.schedulerName = "HRRN";
        break;
    case 6:
        schedule.schedulerName = "FB-1";
        schedule.quantum = 1;
        break;
    case 7:
        schedule.schedulerName = "FB-2i";
        break;
    case 8:
        schedule.schedulerName = "Aging";
        break;

    default:
        break;
    }
    return schedule;
}
process processMaker(string name, int at, int st, int cycles)
{

    process Process;
    Process.pname = name;
    Process.arrivalTime = at;
    Process.serviceTime = st;
    Process.tempServiceTime = st;
    Process.timeProcessing.resize(cycles);
    return Process;
}

// Manipulation
int countDigit(long long n)
{
    if (n / 10 == 0)
        return 1;
    return 1 + countDigit(n / 10);
}
void checkArrival(vector<process> &notHereYetProcess, int t, vector<process> &waitingProcess)
{
    for (int i = 0; i < notHereYetProcess.size(); i++)
    {
        if (notHereYetProcess[i].arrivalTime == t)
        {
            waitingProcess.insert(waitingProcess.begin(), notHereYetProcess[i]);
            reverse(notHereYetProcess.begin(), notHereYetProcess.end());
            notHereYetProcess.resize(notHereYetProcess.size() - 1);
            reverse(notHereYetProcess.begin(), notHereYetProcess.end());
        }
    }
}

void IamWaiting(vector<process> &waitingProcess, int t)
{
    for (size_t i = 0; i < waitingProcess.size(); i++)
    {
        waitingProcess[i].waitTime++;
        waitingProcess[i].timeProcessing[t] = ".";
    }
}
void processRemover(process &p1, vector<process> &ProcessList)
{
    for (size_t i = 0; i < ProcessList.size(); i++)
    {
        if (p1.pname == ProcessList[i].pname)
        {
            ProcessList.erase(ProcessList.begin() + i);
        }
    }
}
void checkArrivalAging(vector<process> &notHereYetProcess, int t, vector<process> &waitingProcess)
{
    for (int i = 0; i < notHereYetProcess.size(); i++)
    {
        if (notHereYetProcess[i].arrivalTime == t)
        {
            waitingProcess.push_back(notHereYetProcess[i]);
            processRemover(notHereYetProcess[i], notHereYetProcess);
        }
    }
}
void fillSpaces(vector<process> &processess_filtered)
{
    for (size_t i = 0; i < processess_filtered.size(); i++)
    {
        for (size_t j = 0; j < processess_filtered[i].timeProcessing.size(); j++)
        {
            if (processess_filtered[i].timeProcessing[j] != "*" && processess_filtered[i].timeProcessing[j] != ".")
            {
                processess_filtered[i].timeProcessing[j] = " ";
            }
        }
    }
}
void calculateTurnandNormTurnTime(vector<process> &processess_filtered)
{
    for (size_t j = 0; j < processess_filtered.size(); j++)
    {
        processess_filtered[j].turnAroundTime = processess_filtered[j].finishTime - processess_filtered[j].arrivalTime;
        processess_filtered[j].normTurnTime = float(processess_filtered[j].turnAroundTime) / float(processess_filtered[j].serviceTime);
    }
}
void calculateScheduleStats(schedule &schedule, vector<process> &processess_filtered, int noProcess)
{
    for (size_t i = 0; i < processess_filtered.size(); i++)
    {
        schedule.totalWaitTime += processess_filtered[i].waitTime;
        schedule.totalTurnAroundTime += processess_filtered[i].turnAroundTime;
        schedule.totalNormTurn += processess_filtered[i].normTurnTime;
    }
    schedule.meanTurnAround = schedule.totalTurnAroundTime / noProcess;
    schedule.meanNormTurn = schedule.totalNormTurn / noProcess;
}
bool orderchecker(process p1, process p2)
{
    return (p1.arrivalTime < p2.arrivalTime);
}
void sortProcessess(vector<process> &processess_filtered)
{
    sort(processess_filtered.begin(), processess_filtered.end(), orderchecker);
}
bool isShorterProcess(process p1, process p2)
{
    return (p1.tempServiceTime > p2.tempServiceTime);
}
void sortProcessesbyservicetime(vector<process> &processList)
{
    sort(processList.begin(), processList.end(), isShorterProcess);
}
bool isShorterHRRNProcess(process p1, process p2)
{
    return (((float(p1.waitTime) + (p1.serviceTime)) / float(p1.serviceTime)) < (((p2.waitTime) + float(p2.serviceTime)) / float(p2.serviceTime)));
}
void sortProcessbyHRRN(vector<process> &processList)
{
    sort(processList.begin(), processList.end(), isShorterHRRNProcess);
}
void IamMoreImportant(vector<process> &waitingProcess)
{
    for (size_t i = 0; i < waitingProcess.size(); i++)
    {
        waitingProcess[i].tempServiceTime++;
    }
}
bool isMoreImportant(process p1, process p2)
{
    return (p1.tempServiceTime > p2.tempServiceTime);
}
void sortProcessbyAging(vector<process> &processList)
{
    sort(processList.begin(), processList.end(), isMoreImportant);
}
void sendToTheHallows(process &p, vector<vector<process>> &feedback_queue)
{
    if (feedback_queue.size() == 0)
    {
        feedback_queue.push_back(vector<process>());
    }
    while (feedback_queue.size() <= p.feedback_level)
    {
        feedback_queue.push_back(vector<process>());
    }
    p.feedback_level++;
    feedback_queue[p.feedback_level - 1].push_back(p);
}
void waitingInTheHallows(vector<vector<process>> &feedback_queue, int t)
{

    for (size_t i = 0; i < feedback_queue.size(); i++)
    {
        for (size_t j = 0; j < feedback_queue[i].size(); j++)
        {
            feedback_queue[i][j].waitTime++;
            feedback_queue[i][j].timeProcessing[t] = ".";
        }
    }
}
void getFromTheHallows(vector<vector<process>> &feedback_queue, vector<process> &processingProcess)
{
    process ptemp = processingProcess[0];
    if (processingProcess.size() == 0)
    {
        for (size_t i = 0; i < feedback_queue.size(); i++)
        {
            for (size_t j = 0; j < feedback_queue[i].size() != 0; j++)
            {
                processingProcess.push_back(feedback_queue[i][0]);
                processRemover(feedback_queue[i][0], feedback_queue[i]);
                return;
            }
        }
    }
    else
    {

        for (size_t i = 0; i < feedback_queue.size(); i++)
        {
            for (size_t j = 0; j < feedback_queue[i].size() != 0; j++)
            {
                processingProcess.pop_back();
                processingProcess.push_back(feedback_queue[i][0]);
                processRemover(feedback_queue[i][0], feedback_queue[i]);
                if (ptemp.pname != processingProcess[0].pname)
                {
                    sendToTheHallows(ptemp, feedback_queue);
                }
                return;
            }
        }
    }
}
void resetPriority(vector<process> &processList)
{
    for (size_t i = 0; i < processList.size(); i++)
    {
        processList[i].tempServiceTime = processList[i].serviceTime;
    }
}
// PRINTERS
void printTrace(schedule schedule, vector<process> finsihed, int noCycles)
{
    string schedulePrinted;
    if (schedule.quantum == 0)
    {
        schedulePrinted = schedule.schedulerName;
    }
    else
    {
        if (schedule.schedulerName == "FB-1" || schedule.schedulerName == "FB-2i" || schedule.schedulerName == "Aging")
        {
            schedulePrinted = schedule.schedulerName;
        }
        else
        {

            schedulePrinted = schedule.schedulerName + "-" + to_string(schedule.quantum);
        }
    }
    cout << schedulePrinted;
    switch (schedulePrinted.length())
    {
    case 3:
        printf("  ");
        break;
    case 4:
        printf(" ");
        break;
    default:
        break;
    }
    for (int i = 0, j = 0; i < noCycles + 1; i++)
    {
        printf(" %d", j);
        j++;
        j == 10 ? j = 0 : false;
    }
    printf(" \n");
    printf("------------------------------------------------\n");
    // Outputing of processed Processess
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        cout << finsihed[i].pname;
        printf("     ");
        for (size_t j = 0; j < noCycles; j++)
        {
            printf("|");
            cout << finsihed[i].timeProcessing[j];
        }
        printf("| ");
        printf("\n");
    }
    printf("------------------------------------------------\n\n");
}
void printStats(schedule schedule, vector<process> finsihed, int noCycles)
{
    string schedulePrinted;
    if (schedule.quantum == 0)
    {
        schedulePrinted = schedule.schedulerName;
    }
    else
    {
        if (schedule.schedulerName == "FB-1" || schedule.schedulerName == "FB-2i" || schedule.schedulerName == "Aging")
        {
            schedulePrinted = schedule.schedulerName;
        }
        else
        {

            schedulePrinted = schedule.schedulerName + "-" + to_string(schedule.quantum);
        }
    }
    cout << schedulePrinted;
    printf("\n");
    // Printing Processes name
    printf("Process    ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        printf("|  %s  ", finsihed[i].pname.c_str());
    }
    printf("|\n");

    // Printing Arrival Time of processes
    printf("Arrival    ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        if (countDigit(finsihed[i].arrivalTime) == 2)
        {
            printf("| %d  ", finsihed[i].arrivalTime);
        }
        else
        {
            printf("|  %d  ", finsihed[i].arrivalTime);
        }
    }
    printf("|\n");

    // Printing service time of processes
    printf("Service    ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        if (countDigit(finsihed[i].serviceTime) == 2)
        {
            printf("| %d  ", finsihed[i].serviceTime);
        }
        else
        {
            printf("|  %d  ", finsihed[i].serviceTime);
        }
    }
    printf("| Mean|\n");
    // Printing finish time of processes
    printf("Finish     ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        if (countDigit(finsihed[i].finishTime) == 2)
        {
            printf("| %d  ", finsihed[i].finishTime);
        }
        else
        {
            printf("|  %d  ", finsihed[i].finishTime);
        }
    }
    printf("|-----|\n");
    // Printing Turnaround time of processes
    printf("Turnaround ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        if (countDigit(finsihed[i].turnAroundTime) == 2)
        {
            printf("| %d  ", finsihed[i].turnAroundTime);
        }
        else
        {
            printf("|  %d  ", finsihed[i].turnAroundTime);
        }
    }
    if (schedule.meanTurnAround / 10 >= 1)
    {
        printf("|%.2f|\n", schedule.meanTurnAround);
    }
    else
    {
        printf("| %.2f|\n", schedule.meanTurnAround);
    }
    // Printing NormTurn time of processes
    printf("NormTurn   ");
    for (size_t i = 0; i < finsihed.size(); i++)
    {
        printf("| %.2f", finsihed[i].normTurnTime);
    }
    printf("| %.2f|\n", schedule.meanNormTurn);
    printf("\n");
}
void printer(string type, schedule schedule, vector<process> finsihed, int noCycles)
{
    if (type[0] == 't')
    {
        printTrace(schedule, finsihed, noCycles);
    }
    else if (type[0] == 's')
    {
        printStats(schedule, finsihed, noCycles);
    }
}

int main()
{
    regex integer("(\\+|-)?[[:digit:]]+");
    // ############## TAKING INPUT #############

    // taking type
    string type;
    cin >> type;
    // taking no of scheduling
    string techinques;
    cin >> techinques;
    // taking schedulers
    vector<string> schedulers;
    int noSchedulers = 0;
    stringstream ss(techinques);
    while (ss.good())
    {
        noSchedulers++;
        string techinque;
        getline(ss, techinque, ',');
        schedulers.push_back(techinque);
    }
    // filtering schedulers
    vector<schedule> schedulers_filtered;
    for (size_t i = 0, j = 0; i < noSchedulers; i++)
    {
        if (regex_match(schedulers[i], integer))
        {
            schedule s = scheduleMaker(stoi(schedulers[i]));
            schedulers_filtered.push_back(s);
        }
        else
        {
            char sn = schedulers[i][0];
            char qn = schedulers[i][2];
            schedule s = scheduleMaker(sn - 48, qn - 48);
            schedulers_filtered.push_back(s);
        }
    }
    // taking no of cycles
    int noCycles;
    cin >> noCycles;
    // taking no of processes
    int noProcesses;
    cin >> noProcesses;
    // creating processes
    vector<string> processess;
    for (size_t i = 0; i < noProcesses; i++)
    {
        string pInfo;
        cin >> pInfo;
        stringstream ps(pInfo);
        while (ps.good())
        {
            string process;
            getline(ps, process, ',');
            processess.push_back(process);
        }
    }
    vector<process> processess_filtered;

    for (size_t i = 0, j = 0; i < noProcesses; i++)
    {
        process p = processMaker(processess[j], stoi(processess[j + 1]), stoi(processess[j + 2]), noCycles);
        j += 3;
        processess_filtered.push_back(p);
    }

    // ##################END OF INPUT##################
    // #################Processing ###################
    for (size_t i = 0; i < schedulers_filtered.size(); i++)
    {
        // Setup
        vector<process> notHereYetProcess;
        for (size_t i = 0; i < processess_filtered.size(); i++)
        {
            notHereYetProcess.push_back(processess_filtered[i]);
        }
        vector<process> waitingProcess;
        vector<process> processingProcess;
        vector<process> finishedProcess;
        int counterTemp = 0;

        if (schedulers_filtered[i].schedulerName == "FCFS")
        {
            notHereYetProcess[0].finishTime = notHereYetProcess[0].serviceTime;
            notHereYetProcess[0].turnAroundTime = notHereYetProcess[0].serviceTime;
            notHereYetProcess[0].waitTime = 0;
            for (size_t j = 1; j < notHereYetProcess.size(); j++)
            {
                notHereYetProcess[j].finishTime = notHereYetProcess[j - 1].finishTime + notHereYetProcess[j].serviceTime;
                notHereYetProcess[j].turnAroundTime = notHereYetProcess[j].finishTime - notHereYetProcess[j].arrivalTime;
                notHereYetProcess[j].waitTime = notHereYetProcess[j].turnAroundTime - notHereYetProcess[j].serviceTime;
                notHereYetProcess[j].normTurnTime = float(notHereYetProcess[j].turnAroundTime) / float(notHereYetProcess[j].serviceTime);
            }
            for (int k = 0; k < noProcesses; k++)
            {
                for (size_t j = (notHereYetProcess[k].arrivalTime + notHereYetProcess[k].waitTime); j < notHereYetProcess[k].finishTime; j++)
                {
                    notHereYetProcess[k].timeProcessing[j] = "*";
                }

                for (size_t j = notHereYetProcess[k].arrivalTime; j < notHereYetProcess[k].finishTime - notHereYetProcess[k].serviceTime; j++)
                {
                    notHereYetProcess[k].timeProcessing[j] = ".";
                }
            }
            calculateTurnandNormTurnTime(notHereYetProcess);
            calculateScheduleStats(schedulers_filtered[i], notHereYetProcess, noProcesses);
            fillSpaces(notHereYetProcess);
            sortProcessess(notHereYetProcess);
            printer(type, schedulers_filtered[i], notHereYetProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "RR")
        {
            int t = 0;
            while (true)
            {
                process p;
                int q = schedulers_filtered[i].quantum;

                checkArrival(notHereYetProcess, t, waitingProcess);
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0))
                {
                    process p = waitingProcess.back();
                    waitingProcess.pop_back();
                    processingProcess.push_back(p);
                }
                if (!(processingProcess.size() == 0))
                {
                    while (q && processingProcess[0].tempServiceTime != 0)
                    {
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        q--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        IamWaiting(waitingProcess, t);
                    }

                    if (processingProcess[0].tempServiceTime != 0)
                    {
                        waitingProcess.insert(waitingProcess.begin(), processingProcess[0]);
                        IamWaiting(waitingProcess, t);
                    }
                    else
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        IamWaiting(waitingProcess, t);
                    }

                    processingProcess.pop_back();
                }
                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "SPN")
        {
            int t = 0;
            while (true)
            {
                process p;
                checkArrival(notHereYetProcess, t, waitingProcess);
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0))
                {
                    process p = waitingProcess.back();
                    for (size_t j = 0; j < waitingProcess.size(); j++)
                    {
                        if (waitingProcess[j].tempServiceTime < p.tempServiceTime)
                        {
                            p = waitingProcess[j];
                        }
                    }
                    processRemover(p, waitingProcess);
                    processingProcess.push_back(p);
                }
                if (!(processingProcess.size() == 0))
                {
                    while (processingProcess[0].tempServiceTime != 0)
                    {
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        IamWaiting(waitingProcess, t);
                    }

                    if (processingProcess[0].tempServiceTime != 0)
                    {
                        waitingProcess.insert(waitingProcess.begin(), processingProcess[0]);
                        IamWaiting(waitingProcess, t);
                    }
                    else
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        IamWaiting(waitingProcess, t);
                    }

                    processingProcess.pop_back();
                }
                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "SRT")
        {
            int t = 0;
            process p;
            while (true)
            {
                checkArrival(notHereYetProcess, t, waitingProcess);
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0))
                {
                    sortProcessesbyservicetime(waitingProcess);
                    process p = waitingProcess.back();
                    processingProcess.push_back(p);
                    processRemover(p, waitingProcess);
                }
                if (!(processingProcess.size() == 0))
                {
                    if (processingProcess[0].tempServiceTime == 0)
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        sortProcessesbyservicetime(waitingProcess);
                        processingProcess.clear();

                        if (waitingProcess.size() != 0)
                        {
                            process p = waitingProcess.back();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            checkArrival(notHereYetProcess, t, waitingProcess);
                        }
                        IamWaiting(waitingProcess, t);
                    }

                    if (processingProcess[0].tempServiceTime != 0)
                    {
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        waitingProcess.push_back(processingProcess[0]);
                        processingProcess.clear();
                        sortProcessesbyservicetime(waitingProcess);
                        if (waitingProcess.size() != 0)
                        {
                            process p = waitingProcess.back();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            checkArrival(notHereYetProcess, t, waitingProcess);
                        }
                        IamWaiting(waitingProcess, t);
                    }
                }
                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "HRRN")
        {
            int t = 0;
            process p;
            while (true)
            {
                checkArrival(notHereYetProcess, t, waitingProcess);
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0))
                {
                    process p = waitingProcess.back();
                    processingProcess.push_back(p);
                    processRemover(p, waitingProcess);
                }
                if (!(processingProcess.size() == 0))
                {
                    while (processingProcess[0].tempServiceTime != 0)
                    {
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        IamWaiting(waitingProcess, t);
                    }

                    if (processingProcess[0].tempServiceTime == 0)
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        checkArrival(notHereYetProcess, t, waitingProcess);
                        IamWaiting(waitingProcess, t);
                    }
                    sortProcessbyHRRN(waitingProcess);
                    processingProcess.pop_back();
                }

                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "FB-1")
        {
            int t = 0;
            vector<vector<process>> feedback_queues;
            while (true)
            {
                process p;
                int q = schedulers_filtered[i].quantum;
                checkArrival(notHereYetProcess, t, waitingProcess);
                int feedback_empty = 1;
                for (size_t m = 0; m < feedback_queues.size(); m++)
                {
                    for (size_t n = 0; n < feedback_queues[m].size(); n++)
                    {
                        feedback_empty = 0;
                    }
                }
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0) && feedback_empty)
                {

                    process p = waitingProcess.front();
                    processingProcess.push_back(p);
                    processRemover(p, waitingProcess);
                }
                if (!(processingProcess.size() == 0))
                {
                    while (q && processingProcess[0].tempServiceTime != 0)
                    {
                        IamWaiting(waitingProcess, t);
                        waitingInTheHallows(feedback_queues, t);
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        q--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                    }
                    if (processingProcess[0].tempServiceTime != 0)
                    {
                        if (waitingProcess.size() != 0)
                        {
                            sendToTheHallows(processingProcess[0], feedback_queues);
                            process p = waitingProcess.back();
                            processingProcess.pop_back();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            getFromTheHallows(feedback_queues, processingProcess);
                        }
                    }
                    else
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        processingProcess.pop_back();
                        if (waitingProcess.size() != 0)
                        {
                            process p = waitingProcess.front();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            getFromTheHallows(feedback_queues, processingProcess);
                        }
                        IamWaiting(waitingProcess, t);
                        waitingInTheHallows(feedback_queues, t);
                    }
                }
                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "FB-2i")
        {
            int t = 0;
            vector<vector<process>> feedback_queues;
            while (true)
            {
                process p;
                checkArrival(notHereYetProcess, t, waitingProcess);
                int feedback_empty = 1;
                for (size_t m = 0; m < feedback_queues.size(); m++)
                {
                    for (size_t n = 0; n < feedback_queues[m].size(); n++)
                    {
                        feedback_empty = 0;
                    }
                }
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0) && feedback_empty)
                {

                    process p = waitingProcess.front();
                    processingProcess.push_back(p);
                    processRemover(p, waitingProcess);
                }
                if (!(processingProcess.size() == 0))
                {
                    int temp = ceil(pow(2, processingProcess[0].feedback_level));
                    while (temp && processingProcess[0].tempServiceTime != 0)
                    {
                        IamWaiting(waitingProcess, t);
                        waitingInTheHallows(feedback_queues, t);
                        processingProcess[0].timeProcessing[t] = "*";
                        t++;
                        processingProcess[0].tempServiceTime--;
                        temp--;
                        checkArrival(notHereYetProcess, t, waitingProcess);
                    }
                    if (processingProcess[0].tempServiceTime != 0)
                    {
                        if (waitingProcess.size() != 0)
                        {
                            sendToTheHallows(processingProcess[0], feedback_queues);
                            process p = waitingProcess.back();
                            processingProcess.pop_back();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            getFromTheHallows(feedback_queues, processingProcess);
                        }
                    }
                    else
                    {
                        processingProcess[0].finishTime = t;
                        finishedProcess.push_back(processingProcess[0]);
                        processingProcess.pop_back();
                        if (waitingProcess.size() != 0)
                        {
                            process p = waitingProcess.front();
                            processingProcess.push_back(p);
                            processRemover(p, waitingProcess);
                        }
                        else
                        {
                            getFromTheHallows(feedback_queues, processingProcess);
                        }
                        IamWaiting(waitingProcess, t);
                        waitingInTheHallows(feedback_queues, t);
                    }
                }
                if (notHereYetProcess.size() == 0 && processingProcess.size() == 0 && waitingProcess.size() == 0)
                {
                    break;
                }
            }

            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
        if (schedulers_filtered[i].schedulerName == "Aging")
        {
            int t = 0;
            while (true)
            {
                process p;
                int q = schedulers_filtered[i].quantum;
                checkArrivalAging(notHereYetProcess, t, waitingProcess);
                sortProcessbyAging(waitingProcess);
                if ((!waitingProcess.size() == 0) && (processingProcess.size() == 0))
                {
                    processingProcess.push_back(waitingProcess[0]);
                    processRemover(waitingProcess[0],waitingProcess);
                    //  IamWaiting(waitingProcess, t);
                }
                resetPriority(processingProcess);
                if (!(processingProcess.size() == 0))
                {
                    while (q)
                    {
                        processingProcess[0].timeProcessing[t] = "*";
                        IamWaiting(waitingProcess, t);
                        t++;
                        checkArrivalAging(notHereYetProcess, t, waitingProcess);
                        q--;
                        // sortProcessbyAging(waitingProcess);
                    }
                    if (!q)
                    {
                        IamMoreImportant(waitingProcess);
                        waitingProcess.push_back(processingProcess[0]);
                        processingProcess.pop_back();
                        if (t == noCycles)
                        {
                            for (size_t k = 0; k < waitingProcess.size(); k++)
                            {
                                finishedProcess.push_back(waitingProcess[k]);
                            }
                            for (size_t k = 0; k < processingProcess.size(); k++)
                            {
                                finishedProcess.push_back(processingProcess[k]);
                            }
                            for (size_t k = 0; k < notHereYetProcess.size(); k++)
                            {
                                finishedProcess.push_back(notHereYetProcess[k]);
                            }
                            break;
                        }
                    }
                }
            }
            calculateTurnandNormTurnTime(finishedProcess);
            calculateScheduleStats(schedulers_filtered[i], finishedProcess, noProcesses);
            fillSpaces(finishedProcess);
            sortProcessess(finishedProcess);
            printer(type, schedulers_filtered[i], finishedProcess, noCycles);
        }
    }
    // ###############End of Processing #############
    return 0;
}
// ############### NUMBRING OF SCHEDULING TECHINQUES###############
// 1. FCFS (First Come First Serve)
// 2. RR (Round Robin)
// 3. SPN (Shortest Process Next)
// 4. SRT (Shortest Remaining Time)
// 5. HRRN (Highest Response Ratio Next)
// 6. FB-1, (Feedback where all queues have q=1)
// 7. FB-2i, (Feedback where q= 2i)
// 8. Aging