#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TEAMS 12
#define NUM_MATCHES 132
#define MAX_TEAM_NAME_LENGTH 20

// Struct til kampinformation
struct Match {
    char weekday[4];
    char date[6];
    char time[6];
    char homeTeam[MAX_TEAM_NAME_LENGTH];
    char awayTeam[MAX_TEAM_NAME_LENGTH];
    int homeGoals;
    int awayGoals;
    int spectators;
};

// Struct til holdinformation
struct Team {
    char name[MAX_TEAM_NAME_LENGTH];
    int points;
    int goalsFor;
    int goalsAgainst;
};

// Funktion til at opdatere holdenes information baseret på kampresultaterne
void updateTeams(struct Match matches[], struct Team teams[]) {
    for (int i = 0; i < NUM_MATCHES; ++i) {
        // Find indeks for hjemme- og udeholdet i hold-arrayet
        int homeIndex = -1, awayIndex = -1;
        for (int j = 0; j < NUM_TEAMS; ++j) {
            if (strcmp(matches[i].homeTeam, teams[j].name) == 0) {
                homeIndex = j;
            }
            if (strcmp(matches[i].awayTeam, teams[j].name) == 0) {
                awayIndex = j;
            }
        }

        // Opdater holdenes statistik baseret på kampresultatet
        if (homeIndex != -1 && awayIndex != -1) {
            // Hjemmehold vinder
            if (matches[i].homeGoals > matches[i].awayGoals) {
                teams[homeIndex].points += 3;
            }
                // Uafgjort kamp
            else if (matches[i].homeGoals == matches[i].awayGoals) {
                teams[homeIndex].points += 1;
                teams[awayIndex].points += 1;
            }
                // Udeholdet vinder
            else {
                teams[awayIndex].points += 3;
            }

            // Opdater målscoring
            teams[homeIndex].goalsFor += matches[i].homeGoals;
            teams[homeIndex].goalsAgainst += matches[i].awayGoals;
            teams[awayIndex].goalsFor += matches[i].awayGoals;
            teams[awayIndex].goalsAgainst += matches[i].homeGoals;
        }
    }
}

// Sammenligningsfunktion til sortering af holdene
int compareTeams(const void *a, const void *b) {
    const struct Team *teamA = (const struct Team *)a;
    const struct Team *teamB = (const struct Team *)b;

    if (teamA->points != teamB->points) {
        return teamB->points - teamA->points; // Sorter efter point faldende
    } else {
        return (teamB->goalsFor - teamB->goalsAgainst) - (teamA->goalsFor - teamA->goalsAgainst); // Sorter efter målforskel faldende
    }
}

// Udskriv stillingen i turneringen
void printStandings(struct Team teams[]) {
    printf("Holdnavn         Point  Mål-af-hold  Mål-mod-hold\n");
    for (int i = 0; i < NUM_TEAMS; ++i) {
        printf("%-15s    %-3d    %-5d         %-5d\n", teams[i].name, teams[i].points, teams[i].goalsFor, teams[i].goalsAgainst);
    }
}

int main() {
    struct Match matches[NUM_MATCHES];
    struct Team teams[NUM_TEAMS];

    FILE *file = fopen("C:\\Users\\rimme\\CLionProjects\\project-3-Rimmen17\\src\\kampe-2022-2023.txt", "r");
    if (file == NULL) {
        perror("Fejl ved åbning af fil");
        printf("Kunne ikke åbne filen.\n");
        return 1;
    }

    for (int i = 0; i < NUM_MATCHES; ++i) {
        fscanf(file, "%s %s %s %s - %s %d - %d %d",
               matches[i].weekday, matches[i].date, matches[i].time,
               matches[i].homeTeam, matches[i].awayTeam,
               &matches[i].homeGoals, &matches[i].awayGoals,
               &matches[i].spectators);
    }
    fclose(file);

    // Initialiser hold-arrayet
    const char *teamNames[NUM_TEAMS] = {"FCK", "RFC", "VFF", "AaB", "LBK", "SIF",
                                        "ACH", "BIF", "AGF", "OB", "FCN", "FCM"};

    for (int i = 0; i < NUM_TEAMS; ++i) {
        strcpy(teams[i].name, teamNames[i]);
        teams[i].points = 0;
        teams[i].goalsFor = 0;
        teams[i].goalsAgainst = 0;
    }

    // Opdater holdenes information baseret på kampresultaterne
    updateTeams(matches, teams);

    // Sorter holdene
    qsort(teams, NUM_TEAMS, sizeof(struct Team), compareTeams);

    // Udskriv stillingen
    printStandings(teams);

    return 0;
}
