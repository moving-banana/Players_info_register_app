#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PLAYERS 300
#define MAX_NAME_LEN 30

typedef struct playerLi
{
    int playerNum;
    char player[MAX_NAME_LEN];
    char team[MAX_PLAYERS];
    int age;
} PlayerLi;

PlayerLi players[MAX_PLAYERS] = {0};
int playerCount = 0;
FILE *file;

// ESC키 입력시 동작 함수
bool EscPressed() {
    int putEsc = 0;
    while ((putEsc = getchar()) != '\n' && putEsc != EOF) {
        if (putEsc == 27) {  // 27은 ESC 키의 ASCII 코드입니다
            return true;
        }
    }
    return false;
}

// 터미널을 새로 그려주는 함수
void clearScreen() {
    printf("\033[H\033[J"); // ANSI 이스케이프 시퀀스로 화면 지우기
}

void delPlayer();
void modSelect();
void searchPlayer();
void updatePlayer();

// 신규선수 정보 입력
void registerNewplayer(){
    int overlap = false; // 중복확인
    char choice[10] = {0};

    while (1)
    {
        // 정보 입력
        printf("신규선수 정보 입력\n");

        printf("선수 번호: ");
        scanf("%d", &players[playerCount].playerNum);
        getchar();

        printf("선수이름: ");
        fgets(players[playerCount].player, sizeof(players[playerCount].player), stdin);
        players[playerCount].player[strcspn(players[playerCount].player, "\n")] = '\0';

        printf("소속팀: ");
        fgets(players[playerCount].team, sizeof(players[playerCount].team), stdin);
        players[playerCount].team[strcspn(players[playerCount].team, "\n")] = '\0';

        printf("연령: ");
        scanf("%d", &players[playerCount].age);
        getchar();

        // 새로운 선수 정보를 입력받았으므로 playerCount를 증가시킴
        playerCount++;

        printf("계속 입력하시겠습니까? (yes/no): ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = '\0';

        if (strcmp(choice, "no") == 0) {
            break;
        }
    }

    // 선수 번호 순서대로 정렬 (버블 정렬 사용)
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = 0; j < playerCount - i - 1; j++) {
            if (players[j].playerNum > players[j + 1].playerNum) {
                PlayerLi temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    // 중복되는 선수 번호가 있는지 체크
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = i + 1; j < playerCount; j++) {
            if (players[i].playerNum == players[j].playerNum) {
                overlap = true;
                printf("중복된 번호가 있습니다. 확인하여 주세요!\n");
                printf("중복된 선수의 정보:\n");
                printf("중복된 선수의 번호: %d\n", j);
                for (int k = i; k <= j; k++) {
                    printf("선수 번호: %d\n", players[k].playerNum);
                    printf("선수 이름: %s\n", players[k].player);
                    printf("소속 팀: %s\n", players[k].team);
                    printf("연령: %d\n", players[k].age);
                    printf("-----------------\n");
                }
                printf("위의 선수와 번호가 겹칩니다!");
                break; // 중복이 발견되면 루프를 종료합니다.
            }
        }
        if (overlap) {
            break; // 중복이 발견되면 외부 루프도 종료합니다.
        }
    }

    // 중복이 있으면 메세지 츌력후 제거 함수호출
    if (overlap == true) {
        delPlayer();
        overlap = false;
    }
    else {
        printf("중복된 번호가 없습니다!\n");
    }

    // 모든 데이터를 다시 파일에 저장
    file = fopen("player_data.txt", "wt");
    if (file != NULL) {
        for (int i = 0; i < playerCount; i++) {
            fprintf(file, "선수 번호 : %d\n", players[i].playerNum);
            fprintf(file, "선수이름 : %s\n", players[i].player);
            fprintf(file, "소속팀 : %s\n", players[i].team);
            fprintf(file, "연령 : %d\n\n", players[i].age);
        }
        fclose(file);
    }

    printf("등록된 선수의 수 : %d\n", playerCount);
}

// 선수정보 삭제
void delPlayer(){
    // 사용자로부터 삭제할 선수 번호 입력받기
    int deleteplayerNum = 0;
    bool deleteIndex = false; // 삭제할 선수를 찾았는지 여부를 나타내는 변수

    printf("삭제할 선수 번호를 입력하세요: ");
    scanf("%d", &deleteplayerNum);

    // 삭제할 선수 번호를 찾아 해당 선수 정보 삭제
    for (int i = 0; i < playerCount; i++) {
        if (players[i].playerNum == deleteplayerNum) {
            deleteIndex = true; // 삭제할 선수를 찾았음을 나타냄
            // 삭제할 선수 이후의 선수 정보를 한 칸씩 앞으로 이동시킴
            for (int j = i; j < playerCount - 1; j++) {
                players[j] = players[j + 1];
            }
            // 선수 정보를 삭제했으므로 playerCount를 감소시킴
            playerCount--;
            printf("선수 정보가 삭제되었습니다.\n");
            break; // 선수를 찾았으므로 더 이상 반복할 필요가 없음
        }
    }

    if (!deleteIndex) {
        printf("해당 선수 번호를 찾을 수 없습니다.\n");
    }
}

// 선수 번호 검색
void searchPlayerNum(){
    int numsearch = 0;
    while(1){
        printf("선수 번호 입력\n");
        printf("등록된 선수의 번호는 1부터 %d까지 입니다.\n", playerCount);
        scanf("%d", &numsearch);
        getchar();

        for(int i = 0; i < playerCount; i++){
            if(numsearch == i+1){
                printf("%d번 선수 정보\n", i+1);
                printf("선수 번호 : %d\n", players[i].playerNum);
                printf("선수이름 : %s\n", players[i].player);
                printf("소속팀 : %s\n", players[i].team);
                printf("연령 : %d\n", players[i].age);
            }
        }
        printf("검색모드로 돌아가려면 ESC 를 입력하세요\n");
        printf("(계속 검색하려면 아무키나 눌러주세요)\n");
        if (EscPressed()) {
            searchPlayer();
        }
    }
}

// 선수 이름 검색
void searchPlayerName() {
    char namesearch[MAX_NAME_LEN] = {0};
    char researchname[10] = {0};
    bool foundName = false;

    printf("선수 이름 입력\n");
    fgets(namesearch, sizeof(namesearch), stdin);
    namesearch[strcspn(namesearch, "\n")] = '\0';

    for (int i = 0; i < playerCount; i++) {
        if (strcmp(namesearch, players[i].player) == 0) {
            printf("%s 선수의 정보\n", players[i].player);
            printf("-----------------\n");
            printf("선수 번호 : %d\n", players[i].playerNum);
            printf("선수이름 : %s\n", players[i].player);
            printf("소속팀 : %s\n", players[i].team);
            printf("연령 : %d\n", players[i].age);
            printf("-----------------\n");
            foundName = true;
        }
    }

    if (!foundName) {
        printf("일치하는 선수가 없습니다!\n");
        printf("계속 입력하시겠습니까? (yes/no) \n");
        fgets(researchname, sizeof(researchname), stdin);
        researchname[strcspn(researchname, "\n")] = '\0';

        if (strcmp(researchname, "yes") == 0) {
            searchPlayerName(); // 재귀적으로 함수 호출하여 다시 검색
        }
        else{
            searchPlayer();
        }
    }

    printf("검색모드로 돌아가려면 ESC 를 입력하세요\n");
    printf("(계속 검색하려면 아무키나 눌러주세요)\n");
    if (EscPressed()) {
        searchPlayer();
    }
}

// 소속팀명으로 선수 검색
void searchPlayerTeam(){
    char teamsearch[MAX_NAME_LEN] = {0};
    char researchTeam[10] = {0};
    char showlist[10] = {0};
    bool foundTeam = false;

    printf("소속팀명으로 선수검색\n");
    // 중복 없이 소속팀 리스트 정리
    char uniqueTeams[MAX_PLAYERS][MAX_NAME_LEN] = {0};
    int uniqueTeamCount = 0;
    for (int i = 0; i < playerCount; i++) {
        bool teamoverlap = false;
        for (int j = 0; j < uniqueTeamCount; j++) {
            if (strcmp(players[i].team, uniqueTeams[j]) == 0) {
                teamoverlap = true;
                break;
            }
        }
        if (!teamoverlap) {
            strcpy(uniqueTeams[uniqueTeamCount], players[i].team);
            uniqueTeamCount++;
        }
    }

    // 소속팀 리스트 알파벳 순으로 정렬
    for (int i = 0; i < uniqueTeamCount - 1; i++) {
        for (int j = i + 1; j < uniqueTeamCount; j++) {
            if (strcmp(uniqueTeams[i], uniqueTeams[j]) > 0) {
                char temp[MAX_NAME_LEN];
                strcpy(temp, uniqueTeams[i]);
                strcpy(uniqueTeams[i], uniqueTeams[j]);
                strcpy(uniqueTeams[j], temp);
            }
        }
    }

    printf("소속팀 리스트를 출력하시겠습니까? (yes/no)\n");
    fgets(showlist, sizeof(showlist), stdin);
    showlist[strcspn(showlist, "\n")] = '\0';
    if (strcmp(showlist, "yes") == 0) {
        // 소속팀 리스트 출력
        printf("(현재 등록된 팀명 리스트)\n");
        for (int i = 0; i < uniqueTeamCount; i++) {
            printf("%s\n", uniqueTeams[i]);
        }
    }
    
    printf("선수 팀명 입력\n");
    
    fgets(teamsearch, sizeof(teamsearch), stdin);
    teamsearch[strcspn(teamsearch, "\n")] = '\0';

    // 일치하는 팀이 있을경우 그 팀 선수의 정보를 출력
    for (int i = 0; i < playerCount; i++) {
        if (strcmp(teamsearch, players[i].team) == 0) {
            printf("-----------------\n");
            printf("%s 선수의 정보\n", players[i].player);
            printf("선수 번호 : %d\n", players[i].playerNum);
            printf("선수이름 : %s\n", players[i].player);
            printf("소속팀 : %s\n", players[i].team);
            printf("연령 : %d\n", players[i].age);
            printf("-----------------\n");
            foundTeam = true;
        }
    }

    if (!foundTeam) {
        printf("일치하는 소속팀이 없습니다!\n");
        printf("계속 입력하시겠습니까? (yes/no) \n");
        fgets(researchTeam, sizeof(researchTeam), stdin);
        researchTeam[strcspn(researchTeam, "\n")] = '\0';

        if (strcmp(researchTeam, "yes") == 0) {
            searchPlayerTeam(); // 재귀적으로 함수 호출하여 다시 검색
        }
    }

    printf("검색모드로 돌아가려면 ESC 를 입력하세요\n");
    printf("(계속 검색하려면 아무키나 눌러주세요)\n");
    if (EscPressed()) {
        searchPlayer();
    }
}

// 선수정보 검색모드 선택
void searchPlayer(){
    int searchoption = 0;

    printf("\n");
    printf("  <--선수정보 검색-->\n\n");
    printf(" 등록된 번호로 찾기 : 1\n");
    printf(" 선수이름으로 찾기 : 2\n");
    printf(" 소속팀명으로 찾기 : 3\n");
    printf(" 이전으로 돌아가기 : 4\n");
    printf(" 입력 ▶ ");
    scanf("%d", &searchoption);
    getchar();

    switch (searchoption)
    {
    case 1:
        searchPlayerNum();
        break;
    case 2:
        searchPlayerName();
        break;
    case 3:
        searchPlayerTeam();
        break;
    case 4:
        modSelect();
        break;
    default:
        printf("입력값이 올바르지 않습니다(다시 입력하세요!)");
        searchPlayer();
        break;
    }
}

// 선수정보 수정
void modifyPlayerData() {
    int choice;
    char search[MAX_NAME_LEN] = {0};
    bool foundPlayer = false;

    printf("선수 번호 혹은 선수 이름을 입력하세요: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    // 선수 번호로 검색
    for (int i = 0; i < playerCount; i++) {
        if (atoi(search) == players[i].playerNum) {
            foundPlayer = true;
            printf("선수 번호 : %d\n", players[i].playerNum);
            printf("선수이름 : %s\n", players[i].player);
            printf("소속팀 : %s\n", players[i].team);
            printf("연령 : %d\n", players[i].age);
            printf("-----------------\n");

            // 수정 여부 확인
            printf("수정하시겠습니까? (1: 예, 2: 아니오): ");
            scanf("%d", &choice);
            getchar(); // 개행 문자 처리

            if (choice == 1) {
                // 선수 정보 수정
                printf("새로운 선수 이름: ");
                fgets(players[i].player, sizeof(players[i].player), stdin);
                players[i].player[strcspn(players[i].player, "\n")] = '\0';

                printf("새로운 소속팀: ");
                fgets(players[i].team, sizeof(players[i].team), stdin);
                players[i].team[strcspn(players[i].team, "\n")] = '\0';

                printf("새로운 연령: ");
                scanf("%d", &players[i].age);
                getchar(); // 개행 문자 처리

                // 파일에 수정된 선수 정보를 다시 쓰기
                file = fopen("player_data.txt", "wt");
                if (file != NULL) {
                    for (int i = 0; i < playerCount; i++) {
                        fprintf(file, "선수 번호 : %d\n", players[i].playerNum);
                        fprintf(file, "선수이름 : %s\n", players[i].player);
                        fprintf(file, "소속팀 : %s\n", players[i].team);
                        fprintf(file, "연령 : %d\n\n", players[i].age);
                    }
                    fclose(file);
                }

                printf("선수 정보가 수정되었습니다.\n");
            } else {
                printf("선수 정보 수정을 취소합니다.\n");
            }
            break;
        }
    }

    // 선수 이름으로 검색
    if (!foundPlayer) {
        for (int i = 0; i < playerCount; i++) {
            if (strcmp(search, players[i].player) == 0) {
                foundPlayer = true;
                printf("선수 번호 : %d\n", players[i].playerNum);
                printf("선수이름 : %s\n", players[i].player);
                printf("소속팀 : %s\n", players[i].team);
                printf("연령 : %d\n", players[i].age);
                printf("-----------------\n");

                // 수정 여부 확인
                printf("수정하시겠습니까? (1: 예, 2: 아니오): ");
                scanf("%d", &choice);
                getchar(); // 개행 문자 처리

                if (choice == 1) {
                    // 선수 정보 수정
                    printf("새로운 선수 이름: ");
                    fgets(players[i].player, sizeof(players[i].player), stdin);
                    players[i].player[strcspn(players[i].player, "\n")] = '\0';

                    printf("새로운 소속팀: ");
                    fgets(players[i].team, sizeof(players[i].team), stdin);
                    players[i].team[strcspn(players[i].team, "\n")] = '\0';

                    printf("새로운 연령: ");
                    scanf("%d", &players[i].age);
                    getchar(); // 개행 문자 처리

                    // 파일에 수정된 선수 정보를 다시 쓰기
                    file = fopen("player_data.txt", "wt");
                    if (file != NULL) {
                        for (int i = 0; i < playerCount; i++) {
                            fprintf(file, "선수 번호 : %d\n", players[i].playerNum);
                            fprintf(file, "선수이름 : %s\n", players[i].player);
                            fprintf(file, "소속팀 : %s\n", players[i].team);
                            fprintf(file, "연령 : %d\n\n", players[i].age);
                        }
                        fclose(file); // 파일을 닫아 변경 사항을 저장
                    }

                    printf("선수 정보가 수정되었습니다.\n");
                } else {
                    printf("선수 정보 수정을 취소합니다.\n");
                }
                break;
            }
        }
    }

    if (!foundPlayer) {
        printf("일치하는 선수가 없습니다.\n");
    }
}

// 선수목록 관리
void managementPlayerLi(){
    char checklist[10] = {0};
    printf("선수정보를 수정 혹은 삭제를 합니다.\n");
    printf("(선수목록을 확인 하시겠습니까? (yes/no))\n");

    fgets(checklist, sizeof(checklist), stdin);
    checklist[strcspn(checklist, "\n")] = '\0';
    if (strcmp(checklist, "yes") == 0) {
        for (int i = 0; i < playerCount; i++) {
            printf("-----------------\n");
            printf("선수 번호 : %d\n", players[i].playerNum);
            printf("선수이름 : %s\n", players[i].player);
            printf("소속팀 : %s\n", players[i].team);
            printf("연령 : %d\n", players[i].age);
            printf("-----------------\n");
        }
    }
    modifyPlayerData();
    printf("검색모드로 돌아가려면 ESC 를 입력하세요\n");
    printf("(계속 검색하려면 아무키나 눌러주세요)\n");
    if (EscPressed()) {
        updatePlayer();
    }
}

// 선수 등록&수정&삭제
void updatePlayer(){
    int updatemod = 0;

    printf("\n");
    printf("선수목록 관리및 신규 등록\n");
    printf("선수목록 관리하기 : 1\n");
    printf("신규선수 등록하기 : 2\n");
    printf("이전으로 돌아가기 : 3\n");
    printf("입력 ▶ ");
    scanf("%d", &updatemod);
    getchar();

    switch (updatemod)
    {
    case 1:
        managementPlayerLi();
        break;
    case 2:
        registerNewplayer();
        break;
    case 3:
        modSelect();
        break;
    default:
        printf("잘못된값을 입력하셨습니다!\n");
        printf("다시 입력해 주세요!");
        updatePlayer();
        break;
    }
}

void modSelect(){
    int modchoose = 0;
	// bool flag = true;

    clearScreen();
    printf("\033[1m\n"); //굵은글자
    printf("명단 관리 프로그램!!\n\n");
    printf("\033[0m");
    printf("선수 검색 : 1\n");
    printf("선수 등록 및 관리 : 2\n");
    printf("프로그램 종료 : 3\n");
    printf("입력 ▶ ");
    scanf("%d", &modchoose);
    getchar();

    switch(modchoose) {
        case 1:;
            searchPlayer();
            break;
        case 2:
            updatePlayer();
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("올바른 메뉴를 선택하세요.\n");
    }
    
}

int main()
{
    clearScreen();
    // 프로그램 시작후 기존 데이터 로드
    file = fopen("player_data.txt", "rt");
    if (file != NULL) {
        while (fscanf(file, "선수 번호 : %d\n", &players[playerCount].playerNum) == 1)
        {
            fscanf(file, "선수이름 : %[^\n]\n", players[playerCount].player); // 띄어쓰기를 포함하기 위해
            fscanf(file, "소속팀 : %[^\n]\n", players[playerCount].team);
            fscanf(file, "연령 : %d\n\n", &players[playerCount].age);
            playerCount++;
        }
        fclose(file);
    }
    printf("playercount = %d\n", playerCount);
    modSelect();
    return 0;
}