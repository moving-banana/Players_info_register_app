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
FILE *playerfile;
FILE *txtfile;

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

// 버퍼를 비워주는 함수
void clearBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
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

    // 정보 입력
    printf("\n신규선수 정보 입력\n");

    // 새로운 선수 정보를 입력받았으므로 playerCount를 사용하여 선수 번호를 자동으로 할당합니다.
    players[playerCount].playerNum = playerCount + 1;

    printf("선수이름: ");
    fgets(players[playerCount].player, sizeof(players[playerCount].player), stdin);
    players[playerCount].player[strcspn(players[playerCount].player, "\n")] = '\0';

    printf("소속팀: ");
    fgets(players[playerCount].team, sizeof(players[playerCount].team), stdin);
    players[playerCount].team[strcspn(players[playerCount].team, "\n")] = '\0';

    printf("연령: ");
    scanf("%d", &players[playerCount].age);
    getchar();
    getchar();

    // 새로운 선수 정보를 입력했으므로 playerCount를 증가시킴
    playerCount++;

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

    // 중복되는 선수 이름이 있는지 체크
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = i + 1; j < playerCount; j++) {
            if(players[i].player == players[j].player) {
                overlap = true;
                printf("중복된 이름이 있습니다. 확인하여 주세요!\n");
                printf("중복된 선수의 정보\n");
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

    // 중복이 있으면 메세지 출력후 제거 함수호출
    if (overlap == true) {
        delPlayer();
        overlap = false;
    }
    else {
        printf("새로운 선수가 등록되었습니다!\n");
    }

    // 모든 데이터를 다시 파일에 저장
    playerfile = fopen("player_data.txt", "wt");
    if (playerfile != NULL) {
        for (int i = 0; i < playerCount; i++) {
            fprintf(playerfile, "선수 번호 : %d\n", players[i].playerNum);
            fprintf(playerfile, "선수이름 : %s\n", players[i].player);
            fprintf(playerfile, "소속팀 : %s\n", players[i].team);
            fprintf(playerfile, "연령 : %d\n\n", players[i].age);
        }
        fclose(playerfile);
    }

    printf("명단에 등록된 모든 선수의 수 : %d\n", playerCount);

    printf("검색모드로 돌아가려면 ESC 를 입력하세요\n");
    printf("(계속 등록하려면 아무키나 눌러주세요)\n");

    if (EscPressed()) {
        updatePlayer();
    }
    registerNewplayer();
}

// 선수정보 삭제
void delPlayer(){
    // 사용자로부터 삭제할 선수 번호 입력받기
    int deleteplayerNum = 0;
    bool deleteIndex = false; // 삭제할 선수를 찾았는지 여부를 나타내는 변수

    printf("삭제할 선수 번호를 입력하세요: ");
    scanf("%d", &deleteplayerNum);
    getchar();

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

    while(1){
        bool correctName = false;
        bool foundName = false;
        bool foundPlayer = false;
        printf("\n<--선수 이름 입력-->\n");
        printf(" 입력 ▶ ");
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
                correctName = false;
                foundPlayer = false;
                foundName = false;
            }
            else {
                correctName = true;
            }
        }

        if (correctName) {
            int matchedChars = 0;
            for (int i = 0; i < playerCount; i++) {
                for (int j = 0; j < strlen(namesearch) && namesearch[j] != '\0'; j++) {
                    if (namesearch[j] == players[i].player[j]) {
                        matchedChars++;
                    }
                }
                if (matchedChars >= 5) {
                    foundPlayer = true;
                    foundName = false;
                }
                else {
                    foundPlayer = false;
                    foundName = true;
                }
            }
        }

        // 이름에서 일부 알파벳이 일치하는 선수 정보 검색
        if (foundPlayer) {
            printf("\n<--유사한 이름을 가진 선수들의 정보-->\n");
            for (int i = 0; i < playerCount; i++) {
                int matchedChars = 0;
                for (int j = 0; j < strlen(namesearch) && namesearch[j] != '\0'; j++) {
                    if (namesearch[j] == players[i].player[j]) {
                        matchedChars++;
                    }
                }
                if (matchedChars >= 3) { // 이름에서 3개 이상의 알파벳이 일치하는 경우 출력
                    printf("\n%s 선수의 정보\n", players[i].player);
                    printf("-----------------\n");
                    printf("선수 번호: %d\n", players[i].playerNum);
                    printf("소속팀: %s\n", players[i].team);
                    printf("연령: %d\n", players[i].age);
                    printf("-----------------\n");
                    foundPlayer = false;
                }
            }
        }

        if (foundName) {
            foundName = false;
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
    searchPlayerTeam();
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
    int choice = 0;
    char search[MAX_NAME_LEN] = {0};
    bool foundPlayer = false;

    printf("선수 번호 혹은 선수 이름을 입력하세요: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    // 선수 번호로 검색
    for (int i = 0; i < playerCount; i++) {
        if (atoi(search) == players[i].playerNum) {
            foundPlayer = true;

            printf("-----------------\n");
            printf("선수 번호 : %d\n", players[i].playerNum);
            printf("선수이름 : %s\n", players[i].player);
            printf("소속팀 : %s\n", players[i].team);
            printf("연령 : %d\n", players[i].age);
            printf("-----------------\n");

            // 수정,삭제 여부 확인
            printf("수정 혹은 삭제하시겠습니까? (1: 수정, 2: 삭제, 3:아니요): ");
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
                playerfile = fopen("player_data.txt", "wt");
                if (playerfile != NULL) {
                    for (int i = 0; i < playerCount; i++) {
                        fprintf(playerfile, "선수 번호 : %d\n", players[i].playerNum);
                        fprintf(playerfile, "선수이름 : %s\n", players[i].player);
                        fprintf(playerfile, "소속팀 : %s\n", players[i].team);
                        fprintf(playerfile, "연령 : %d\n\n", players[i].age);
                    }
                    fclose(playerfile);
                }

                printf("선수 정보가 수정되었습니다.\n");
            } 
            else if (choice == 2){
                delPlayer();
            }
            else {
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
                printf("수정 혹은 삭제하시겠습니까? (1: 수정, 2: 삭제, 3:아니요): ");
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
                    playerfile = fopen("player_data.txt", "wt");
                    if (playerfile != NULL) {
                        for (int i = 0; i < playerCount; i++) {
                            fprintf(playerfile, "선수 번호 : %d\n", players[i].playerNum);
                            fprintf(playerfile, "선수이름 : %s\n", players[i].player);
                            fprintf(playerfile, "소속팀 : %s\n", players[i].team);
                            fprintf(playerfile, "연령 : %d\n\n", players[i].age);
                        }
                        fclose(playerfile); // 파일을 닫아 변경 사항을 저장
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
    while(1){
        char checklist[10] = {0};

        printf("\n");
        printf("     <--선수목록 관리-->\n");
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

// 초기 모드 선택
void modSelect(){
    int modchoose = 0;
	// bool flag = true;

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
            printf("데이터를 저장한 후 프로그램을 종료합니다!\n");
            printf("3\n");
            sleep(1);
            printf("2\n");
            sleep(1);
            printf("1\n");
            sleep(1);
            exit(0);
            break;
        default:
            printf("올바른 메뉴를 선택하세요.\n");
    }
}

int main(){
    // 프로그램 시작후 기존 데이터 로드
    playerfile = fopen("player_data.txt", "rt");
    if (playerfile != NULL) {
        while (fscanf(playerfile, "선수 번호 : %d\n", &players[playerCount].playerNum) == 1)
        {
            fscanf(playerfile, "선수이름 : %[^\n]\n", players[playerCount].player); // 띄어쓰기를 포함하기 위해
            fscanf(playerfile, "소속팀 : %[^\n]\n", players[playerCount].team);
            fscanf(playerfile, "연령 : %d\n\n", &players[playerCount].age);
            playerCount++;
        }
        fclose(playerfile);
    }

    // 정렬중 빈 번호가 있으면 채워넣기
    int currentNumber = 1; // 현재 선수 번호
    for (int i = 0; i < playerCount; i++) {
        // 현재 선수의 번호가 현재 번호와 같으면 다음 번호로 증가
        while (players[i].playerNum > currentNumber) {
            // 빈 선수 번호를 발견하면 할당
            players[i].playerNum = currentNumber;
        }
        // 선수 번호를 증가시킴
        currentNumber++;
    }

    // 모든 데이터를 다시 파일에 저장
    playerfile = fopen("player_data.txt", "wt");
    if (playerfile != NULL) {
        for (int i = 0; i < playerCount; i++) {
            fprintf(playerfile, "선수 번호 : %d\n", players[i].playerNum);
            fprintf(playerfile, "선수이름 : %s\n", players[i].player);
            fprintf(playerfile, "소속팀 : %s\n", players[i].team);
            fprintf(playerfile, "연령 : %d\n\n", players[i].age);
        }
        fclose(playerfile);
    }

    // 메인화면 TXT파일 열기
    char character = 0;
    txtfile = fopen("reference.txt", "r");
    if (txtfile == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }
    // 파일에서 문자 하나씩 읽어와서 출력
    while ((character = fgetc(txtfile)) != EOF) {
        printf("%c", character);
    }
    // 파일 닫기
    fclose(txtfile);

    modSelect();
    return 0;
}