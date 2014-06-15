#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include "UserManagement.h"

int user_count;
int bf_update_save = 0;
int user_size = 100;

void clearPart()
{
	printf("\t\t\t\t\t");
}

void clearLine()
{
	printf("\t\t\t\t\t\t\t\t\t\t");
}

void clearSection(int x, int y, int line)
{
	int i;

	gotoxy(x, y); for (i = 0; i < line; i++)
		clearLine();
}

// 검색 조건에 맞는 회원들 중 한 명의 회원 선택
int selectData(UserInfo *userInfo, int *user_count, int *data_idx, char *continue_act, int *search_num, char menu[])
{
	int select;
	//int select_idx;

	if (*search_num == -1){// 검색 없이 메인메뉴로 나갈 경우
		*continue_act = QUIT;
		return -1;
	}

	gotoxy(55, 3); printf("회원 정보 %s", menu);

	if (*search_num == 0){ // 찾는 데이터가 없을 경우
		gotoxy(6, 22); printf("위의 정보에 해당하는 회원이 존재하지 않습니다\n");
		return -1;
	}

	if (*search_num > 1){
		gotoxy(6, 22); printf("%s 하실 회원의 NO.를 입력하고 enter키를 눌러 주세요: ", menu);
		scanf("%d", &select);
		*search_num = 1;
		data_idx += (select-1);

		clearSection(6, 13, 8);
		printSingleData(userInfo, data_idx, search_num);
	}
	return *(data_idx);
}

// 회원 삭제, 수정, 검색을 위한 검색 메인
void searchMain(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, int menu)
{ 
	char continue_act=0;

	while (continue_act != QUIT){

		searchData(userInfo, *user_count, data_idx, search_num, &continue_act);

		if (menu == 1){
			deleteData(userInfo, user_count, data_idx, search_num, &continue_act);
			continueAct(&continue_act, "삭제");
		}
		else if (menu == 2){
			updateData(userInfo, user_count, data_idx, search_num, &continue_act);
			continueAct(&continue_act, "수정");
		}
		else if (menu==3)
			continueAct(&continue_act, "검색");
	}
}

// 회원 삭제시 뒤에 있는 회원 앞으로 땡기기
void deleteUser(UserInfo *userInfo, int *select_idx)
{
	int i;

	if (*select_idx == user_count - 1){
		userInfo[*select_idx].userId = 0;
		return;
	}

	for (i = *select_idx; i < user_count; i++){
		userInfo[i].userId = userInfo[i + 1].userId;
		strcpy(userInfo[i].userName, userInfo[i + 1].userName);
		strcpy(userInfo[i].userAddress, userInfo[i + 1].userAddress);
		strcpy(userInfo[i].userPhone, userInfo[i + 1].userPhone);
	}
}

// 선택지에서 잘못된 입력시 오류 메세지
void warnInvaildInsert()
{
	gotoxy(55, 22); puts("잘못된 입력입니다");
}

// 선택지에서 잘못된 입력시 오류 메세지
void warnInvaildInput(char *input)
{
	while (1){
		*input = _getch();
		if (*input != '1' && *input != '2' && *input!='3' && *input != QUIT){
			warnInvaildInsert();
		}
		else
			break;
	}
}


// userInfo 용량 확인
void checkSize(UserInfo *userInfo)
{
	int i;
	UserInfo *new_userInfo=NULL;


	if (user_count > user_size - 5){
		user_size *= 2;
		new_userInfo = (UserInfo *)realloc(userInfo, sizeof(UserInfo)*user_size);

		if (new_userInfo)

		userInfo = new_userInfo;

		for (i = user_count; i < user_size; i++)
			userInfo[i].userId = 0;

		free(new_userInfo);
	}
}

// 데이터 초기화: data.txt의 자료를 userInfo 구조체에 저장
void initData(UserInfo *userInfo)
{
	FILE *readFile = fopen("data.txt", "rt");

	//userInfo = (UserInfo *)malloc(sizeof(UserInfo)*user_size);

	for (int i = 0; i < user_size; i++){ // data 초기화
		userInfo[i].userId = 0;
	}

	if (readFile == NULL){
		puts("data.txt file open error\n");
		return;
	}
	user_count = loadData(userInfo, readFile); // data.txt를 userInfo 구조체 배열에 입력, user_count 갱신
	
	fclose(readFile);
}

int main(void)
{
	UserInfo *userInfo=NULL;
	int data_idx[50] = { 0 };
	int search_num = 0;

	char input = 0;
	
	userInfo = (UserInfo *)malloc(sizeof(UserInfo)*user_size);
	initData(userInfo);
	
	while (input!=QUIT){

		menu();

		input = _getch();
		switch (input){
		case '1': // 회원보기
			printAllData(userInfo, user_count);
			break;
		case '2': // 회원등록
			insertData(userInfo, &user_count, data_idx, &search_num);
			break;
		case '3': // 회원검색 후 검색 회원 삭제
			searchMain(userInfo, &user_count, data_idx, &search_num, 1);
			break;
		case '4': // 회원검색 후 검색 회원 수정
			searchMain(userInfo, &user_count, data_idx, &search_num, 2);
			break;
		case '5': // 회원검색
			searchMain(userInfo, &user_count, data_idx, &search_num, 3);
			break;
		case '6': // 저장
			saveData(userInfo, user_count, 0);
			break;
		case QUIT: // 종료
			if (bf_update_save > 0)
				checkSave(userInfo, user_count);
			gotoxy(6, 22); printf("잘가요. . . ");
			system("exit");
			break;
		default:
			warnInvaildInsert();
		}
	}

	free(userInfo);
	return 0;

}

void menu(){

	printUp();
	gotoxy(64, 3); puts("MAIN");

	gotoxy(32, 6); puts("1. 회 원 보 기");
	gotoxy(32, 8); puts("2. 회 원 등 록");
	gotoxy(32, 10); puts("3. 회 원 삭 제");
	gotoxy(32, 12); puts("4. 회 원 수 정");
	gotoxy(32, 14); puts("5. 회 원 검 색");
	gotoxy(32, 16); puts("6. 저       장");
	gotoxy(32, 18); puts("0. 종       료");

	gotoxy(6, 21); puts("──────────────────────────────────");
	gotoxy(6, 22); puts("원하시는 메뉴의 숫자를 입력해 주세요");
	gotoxy(6, 23); puts("──────────────────────────────────");
	

}

// 회원 불러오기: 프로그램 실행시 한 번 수행, 맨 첫줄 인덱스 날려버리기
int loadData(UserInfo *userInfo, FILE *fp)
{

	int i = 0;
	int count = 0;

	fscanf(fp, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n",
		&userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);

	while ((fscanf(fp, "%d\t%[^\t]\t%[^\t]\t%[^\n]\n",
		&userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone)) != EOF){

		i++;
		count++;
	}

	return count;
}

// 상단 출력
void printUp()
{
	system("cls");

	gotoxy(6, 0); puts("┌────────────────────────────────┐");
	gotoxy(6, 1); puts("│                     회 원 관 리 프 로 그 램                    │");
	gotoxy(6, 2); puts("│                                                                │");
	gotoxy(6, 3); puts("│                                                                │");
	gotoxy(6, 4); puts("└────────────────────────────────┘");

	gotoxy(6, 21); puts("──────────────────────────────────");
	gotoxy(6, 23); puts("──────────────────────────────────");
}

//회원보기
void printAllData(UserInfo *userInfo, int user_count)
{
	int i = 0;
	int user_num = 1;
	int page = 1;
	int total_page;
	char input = 0;

	if (user_count%PRINT_NUM == 0)
		total_page = (user_count / PRINT_NUM);
	else
		total_page = (user_count / PRINT_NUM) + 1;

	printUp();
	gotoxy(55, 3); puts("전체 회원 보기");
	gotoxy(6, 6); puts("NO.   I D\t     이 름\t\t 주 소\t\t     전화번호");
	gotoxy(6, 7); puts("──────────────────────────────────");

	while (1){
		//
		gotoxy(6, 8 + i - (page - 1)*PRINT_NUM);
		if (userInfo[i].userId == 0) // 회원 정보가 NULL 일 때
			clearLine();

		else // 회원 정보가 존재 할 때
			printf("%2d   %d  %8s\t%20s\t   %s",
				i+1,userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
		
		user_num++;
		i++;

		if (user_num > PRINT_NUM){

			user_num = 1;
			
			gotoxy(66, 20); printf("%d/%d page", page, total_page);
			gotoxy(6, 22); puts("1: 이전 페이지 2: 다음 페이지 0: 메인메뉴                         ");

			warnInvaildInput(&input);

			switch (input){

			case '1':
				clearSection(6,8,PRINT_NUM);
				if (page == 1){
					i = (page - 1) * PRINT_NUM;
					break;
				}
				else{
					i = (page - 2) * PRINT_NUM;
					page--;
				}
				break;

			case '2':
				clearSection(6,8,PRINT_NUM);
				if (page == total_page){
					i = (page - 1)*PRINT_NUM;
					break;
				}
				else{
					i = page * PRINT_NUM;
					page++;
				}
				break;

			case QUIT:
				return;

			default:
				warnInvaildInsert();

				
			}
		}
	}
}


// 회원등록 
void insertData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num)
{
	char input = 0;
	char input_chk_dup = 0;
	
	int biggerId;

	while (input != QUIT){

		//checkSize(userInfo);

		printUp();
		gotoxy(55, 3); puts("신규 회원 등록");
		gotoxy(25, 6); printf("추가하실 데이터를 입력하세요\n");

		biggerId = checkBigId(userInfo, user_count); // 가장 큰 ID값 확인
		userInfo[*user_count].userId = biggerId + 1;
		gotoxy(25, 8); printf(" I        D : %d", userInfo[*user_count].userId);
		gotoxy(25, 9); printf("이       름 : ");
		gotoxy(25, 10); printf("주       소 : ");
		gotoxy(25, 11); printf("전 화 번 호 : ");

		gotoxy(6, 22); printf("1-4자의 한글 이름을 입력해 주세요");
		gotoxy(39, 9); gets(userInfo[*user_count].userName);
		while (isNotValidName(userInfo[*user_count].userName)){ // 이름 유효성 체크
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 9); gets(userInfo[*user_count].userName);
		}

		gotoxy(6, 22); clearLine();
		gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // 주소 유효성 체크
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		}

		gotoxy(6, 22); printf("000-0000-0000의 형식으로 '-'포함 입력해주세요");
		gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // 전화번호 유효성 체크
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		}

		checkDuplicated(userInfo, user_count, data_idx, search_num); // 중복 회원 체크, 존재시 중복회원의 data_idx로 갱신

		if (*search_num == 0){ // 중복회원이 없을 경우

			gotoxy(30, 19); printf("등록 되었습니다!\n");
			(*user_count)++;
			bf_update_save++;

			continueAct(&input, "등록");
		}

		else{// 중복회원이 있을 경우

			printSingleData(userInfo, data_idx, search_num);
			gotoxy(6, 22); printf("중복회원이 있는데도 등록하시겠습니까? 1.예 2.아니오\n");

			warnInvaildInput(&input_chk_dup);
			switch (input_chk_dup){
				
				case '1': // 중복 회원 등록
					(*user_count)++;
					bf_update_save++;
					gotoxy(30, 19); printf("등록 되었습니다!\n");
					continueAct(&input,"등록");
					break;
				case '2': // 중복 회원 등록 안함
					userInfo[*user_count].userId = 0;
					continueAct(&input,"등록");
					break;
				default:
					warnInvaildInsert();
			}
		}

	}
}


// 회원검색(회원삭제 및 등록을 위한): 검색 데이터의 위치를 data_idx에 저장
void searchData(UserInfo *userInfo, int user_count, int *data_idx, int *search_num, char *continue_act)
{
	char data[100];
	char input = 0;

	int i;

	*search_num = 0; // 초기화: 안해주면 이후 검색시 data_idx가 이미 설정되어 있음

	printUp();
	gotoxy(55, 3); puts("회원 정보 검색");
	gotoxy(25, 6); printf("어떤 정보로 검색 하시겠습니까?\n");
	gotoxy(6, 22); puts("1.ID 2.이름 3.전화번호 0.메인메뉴");

	warnInvaildInput(&input);
	switch (input){
		case '1': // ID 검색
			gotoxy(6, 22); clearLine();
			gotoxy(25, 9); printf(" I        D : ");
			gotoxy(6, 22); printf("6자리 숫자의 ID를 입력해주세요");
			gotoxy(39, 9); gets(data);
			while (isNotValidId(data)){ // ID 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (atoi(data) == userInfo[i].userId){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			break;

		case '2': // 이름 검색
			gotoxy(6, 22); clearLine();
			gotoxy(25, 9); printf("이       름 : ");
			gotoxy(6, 22); printf("1-4자의 한글 이름을 입력해 주세요");
			gotoxy(39, 9); gets(data);
			while (isNotValidName(data)){ // 이름 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userName) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			break;

		case '3': // 전화번호 검색
			gotoxy(6, 22); clearLine();
			gotoxy(25, 9); printf("전 화 번 호 : ");
			gotoxy(6, 22); printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n");
			gotoxy(39, 9); gets(data);
			while (isNotValidNum(data)){ // 전화번호 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userPhone) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			break;

		case QUIT:
			*search_num = -1;
			continue_act = input;
			return;

		default:
			warnInvaildInsert();
	}


}



// 회원삭제: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void deleteData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act)
{
	char input = 0;
	int select_idx;

	select_idx=selectData(userInfo, user_count, data_idx, continue_act, search_num, "삭제");
	if (*search_num == -1 || *search_num == 0)
		return;
	
	gotoxy(6, 22); clearLine();
	gotoxy(6, 22); printf("위의 회원을 삭제 하시겠습니까? 1.삭제 2.아니오\n");

	warnInvaildInput(&input);

	switch (input){
	case '1':
		gotoxy(30, 19); printf("삭제 되었습니다!\n");
		deleteUser(userInfo, &select_idx);
		bf_update_save++;
		(*user_count)--;
		break;

	case '2':
		return;

	default:
		warnInvaildInsert();
		break;
	}
}

//회원 수정: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void updateData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act)
{

	char input=0;

	selectData(userInfo, user_count, data_idx, continue_act, search_num, "수정");
	if (*search_num == -1 || *search_num == 0)
		return;

	while (*continue_act != QUIT){

		gotoxy(25, 6); printf("어떤 정보를 수정 하시겠습니까?\n");
		gotoxy(25, 9); clearLine();
		gotoxy(6, 22); clearLine();
		gotoxy(6, 22); puts("1.이름 2.주소 3.전화번호 0.메인메뉴");

		warnInvaildInput(&input);

		switch(input){

		case '1':
			gotoxy(25, 9); printf("이       름 : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("수정하실 1-4자의 한글 이름을 입력해 주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // 입력받은 이름 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			}
			gotoxy(30, 19); printf("수정 되었습니다!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"더 수정");
			break;

		case '2':
			gotoxy(25, 9); printf("주       소 : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("수정하실 주소를 입력해 주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // 입력받은 주소 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			}
			gotoxy(30, 19); printf("수정 되었습니다!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"더 수정");
			break;

		case '3':
			gotoxy(25, 9); printf("전 화 번 호 : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("수정하실 전화번호를 000-0000-0000의 형식으로 '-'포함 입력해주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // 입력받은 전화번호 유효성 체크
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			}
			gotoxy(30, 19); printf("수정 되었습니다!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"더 수정");
			break;

		case QUIT:
			*continue_act = QUIT;
			break;

		default:
			warnInvaildInsert();
		}
	}
	
}

// 회원저장: 
int saveData(UserInfo *userInfo, int user_count, int *chk_exit)
{
	FILE *saveFile=fopen("data.txt", "wt");
	int i;
	
	if (saveFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}

	printUp();
	gotoxy(55, 3); puts("회원 정보 저장");

	fprintf(saveFile, "회원 아이디	회원 이름	회원 주소	핸드폰 번호\n"); //Index 저장
	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId!=0)
			fprintf(saveFile, "%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	fclose(saveFile);

	bf_update_save = 0;
	gotoxy(18, 9); printf("모든 회원 정보가 data.txt에 저장 되었습니다\n");
	
	if (chk_exit){
		gotoxy(55, 3); puts("  프로그램 종료");
		gotoxy(6, 22); printf("종료하려면 아무키나 누르세요\n");
		_getch();
		return 0;
	}
	
	gotoxy(6, 22); printf("메인메뉴로 돌아가려면 아무키나 누르세요\n");
	_getch();
	
	return 0;
}



// 회원보기(회원삭제 및 등록을 위한): 검색 데이터의 정보를 출력
void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num)
{
	int i;

	if (*search_num == 0){ // 찾는 데이터가 없을 경우
		gotoxy(17, 13); printf("위의 정보에 해당하는 회원이 존재하지 않습니다");
	}
	
	else if (*search_num == -1)
		return;

	else{ // 검색 자료 출력

		clearSection(6, 14, *search_num);
		gotoxy(6, 22); clearLine();
		for (i = 0; i < *search_num; i++){
			gotoxy(6, 13); puts("NO.   I D\t     이 름\t\t 주 소\t\t     전화번호");
			gotoxy(6, 14 + i); printf("%2d   %d  %8s\t%20s\t   %s\n", i + 1, userInfo[*(data_idx+i)].userId, userInfo[*(data_idx+i)].userName,
				userInfo[*(data_idx+i)].userAddress, userInfo[*(data_idx+i)].userPhone);
		}
	}
}

// 가장 큰 ID값 확인
int checkBigId(UserInfo *userInfo, int *user_count)
{
	int i;
	int biggerId = 141100;

	for (i = 0; i < *user_count; i++){
		if (userInfo[i].userId>biggerId)
			biggerId = userInfo[i].userId;
	}

	return biggerId;
}

/* 중복 데이터 확인
1) 중복 없을시 return
2) 중복 존재시 중복되는 기존 데이터의 index값 반환
*/
void checkDuplicated(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num)
{
	int i;

	*search_num = 0;

	for (i = 0; i < *user_count; i++){

		if (strcmp(userInfo[*user_count].userName, userInfo[i].userName) == 0
			&& strcmp(userInfo[*user_count].userAddress, userInfo[i].userAddress) == 0
			&& strcmp(userInfo[*user_count].userPhone, userInfo[i].userPhone) == 0){

			data_idx[*search_num] = i;
			(*search_num)++;
			
		}
	}

	return;
}

// 종료 전 변경사항이 저장되지 않았을 때 저장여부 확인
void checkSave(UserInfo *userInfo, int user_count)
{
	char input;
	int chk_exit = 1;

	printUp();
	gotoxy(58, 3); printf("Before Save");
	gotoxy(20, 9); printf("%d개의 저장되지 않은 사항이 있습니다.", bf_update_save);
	gotoxy(6, 22); printf("저장하시고 종료하시겠습니까? 1.예 2.아니오 ");
	input = _getch();

	switch (input){
	case '1':
		//Q. 이런 경우도 call(main-checkSave)->call(checkSave-saveData)의 경우라 안좋은 것인지
		saveData(userInfo, user_count, &chk_exit);
		break;
	case '2':
		break;
	default:
		printf("제대로 입력\n");
	}
}

/* 작업을 계속 수행할지 확인
모든 함수에서 input=='0' 일 때 메인메뉴로 이동
*/
void continueAct(char *continue_act, char *menu)
{
	char continue_input;

	gotoxy(6, 22); clearLine();

	if (menu == "더 수정"){
		gotoxy(6, 22); printf("이 회원을 %s 하시겠습니까? 1.예 2.아니오\n", menu);
	}
	else{
		gotoxy(6, 22); printf("다른 회원을 %s 하시겠습니까? 1.예 0.메인메뉴\n", menu);
	}

	continue_input = _getch();
	
	switch(continue_input) {
		case '1':
			return;
		case '2':
			*continue_act = QUIT;
			break;
		case QUIT:
			*continue_act = QUIT;
			break;
		default:
			warnInvaildInsert();
	}

}

/* ID 유효성 체크:
1. 6자리인지 확인
2. 모든 입력값이 정수인지 확인
*/
int isNotValidId(char *data)
{
	int len = strlen(data);
	int i;

	if (len!=6)
		return 1;

	for (i = 0; i < len; i++){
		if (*data<48 || *data>57)
			return 1;

		data++;
	}

	return 0;
}

/* 이름 유효성 체크:
1. 이름 길이 확인(1~4자)
2. 맨 첫번째 칸 띄어쓰기 확인
+) 이0씨를 위해 이름에 숫자가 들어가는 체크는 하지 않았습니다.(단호박)
*/
int isNotValidName(char *data)
{
	int len = strlen(data);

	if (len == 0||len>8)
		return 1;

	if (*data == 32)
		return 1;

	return 0;
}

/* 주소 유효성 체크:
1. NULL값이 아닌지 확인
2. 맨 첫번째 칸 띄어쓰기 확인
*/
int isNotValidChar(char *data)
{
	int len = strlen(data);
	
	if (len == 0)
		return 1;
	
	if (*data==32)
		return 1;

	return 0;
}

/* 전화번호 유효성 체크
1. '-'포함 13자리 확인
2. 000-0000-0000 의 형식 확인 
	2.1) 4, 9번째 '-'
	2.2) 나머지 자리 숫자
*/
int isNotValidNum(char *data)
{
	int i;
	int len = strlen(data);
	
	if (len != 13) // 1. '-'포함 13자리 확인
		return 1;

	for (i = 0; i < len; i++){ // 2. 000 - 0000 - 0000 의 형식 확인

		if (i == 3 || i == 8){ // 2.1) 4, 9번째 '-'
			if (*data != 45)
				return 1;
		}

		else{ // 2.2) 나머지 자리 숫자
			if (*data<48 || *data>57)
				return 1;
		}
		data++;
	}

	return 0;

}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}