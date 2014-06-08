#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include "UserManagement.h"


//Q. 모든 함수마다 호출되는 변수의 경우 지역변수가 아닌 전역변수로 받는 것이 더 나은것인지?
//UserInfo userInfo[USER_COUNT];
//int user_count;
int bf_update_save = 0;

int main(void)
{
	UserInfo userInfo[USER_COUNT];
	FILE *readFile = fopen("data.txt", "rt");

	int user_count;
	int new_count=1;
	int data_idx[USER_COUNT] = { 0 };
	int search_num = 0;

	char input = 0;
	char continue_act;

	
	if (readFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}
	user_count=loadData(userInfo, readFile); // data.txt를 userInfo 구조체 배열에 입력, user_count 갱신
	
	while (input!=QUIT){

		menu();
		continue_act = 0;

		input = _getch();
		switch (input){
		case '1': // 회원보기
			printAllData(userInfo, user_count);
			break;
		case '2': // 회원등록
			insertData(userInfo, &user_count, &new_count, data_idx, &search_num);
			break;
		case '3': // 회원검색 후 검색 회원 삭제
			while (continue_act!=QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
				deleteData(userInfo, data_idx, &continue_act, &user_count, &search_num);
			}
			break;
		case '4': // 회원검색 후 검색 회원 수정
			while (continue_act != QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
				updateData(userInfo, data_idx, &continue_act, &search_num);
			}
			break;
		case '5': // 회원검색
			while (continue_act != QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
			}
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
			printf("enter 1 to 7\n");
		}
	}

	fclose(readFile);
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
void printAllData(UserInfo *userInfo, int user_count){

	int i = 0;
	int user_num = 1;
	int page = 1;
	int total_page;
	char input=0;

	if (user_count%PRINT_NUM == 0)
		total_page = (user_count / PRINT_NUM);
	else
		total_page = (user_count / PRINT_NUM)+1;
	
	printUp();
	gotoxy(55, 3); puts("전체 회원 보기");
	gotoxy(6, 6); puts("   I D     이 름\t 주소\t\t\t\t 전화번호");
	gotoxy(6, 7); puts("──────────────────────────────────");

	while (i<user_count){

		if (userInfo[i].userId == 0){
			i++;
			user_count++;
		}

		gotoxy(6, 8 + i-(page-1)*PRINT_NUM);
		printf("  %d\t%s\t%s\t\t%s\n",
			userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
		user_num++;
		i++;

		if (user_num > PRINT_NUM){

			user_num = 1;

			gotoxy(66, 20); printf("%d/%d page", page, total_page);
			//gotoxy(6, 21); puts("──────────────────────────────────");
			gotoxy(6, 22); puts("1: 이전 페이지 2: 다음 페이지 0: 메인메뉴                         ");
			//gotoxy(6, 23); puts("──────────────────────────────────");

			while (1){
				input = _getch();
				if (input != '1' && input != '2' && input != QUIT){
					gotoxy(55, 22); puts("잘못된 입력입니다");
				}
				else
					break;
			}

			switch (input){

			case '1':
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
				gotoxy(55, 22); puts("잘못된 입력입니다");
		
			}
		}
	}
	return;
}

// 회원등록 
void insertData(UserInfo *userInfo, int *user_count, int *new_count, int *data_idx, int *search_num)
{
	char input = 0;
	char input_chk_dup = 0;

	int biggerId;
	
	while (input != '0'){
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
			gotoxy(55, 22); puts("잘못된 입력입니다");
			gotoxy(39, 9); printf("                                   ");
			gotoxy(39, 9); gets(userInfo[*user_count].userName);
		}

		gotoxy(6, 22); printf("                                          ");
		gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // 주소 유효성 체크
			gotoxy(55, 22); puts("잘못된 입력입니다");
			gotoxy(39, 10); printf("                                   ");
			gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		}

		gotoxy(6, 22); printf("000-0000-0000의 형식으로 '-'포함 입력해주세요");
		gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // 전화번호 유효성 체크
			gotoxy(55, 22); puts("잘못된 입력입니다");
			gotoxy(39, 11); printf("                                   ");
			gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		}

		checkDuplicated(userInfo, user_count, data_idx, search_num); // 중복 회원 체크, 존재시 중복회원의 data_idx로 갱신

		if (*search_num == 0){ // 중복회원이 없을 경우

			gotoxy(55, 22); printf("등록 되었습니다\n");
			(*user_count)++;
			(*new_count)++;
			bf_update_save++;

			continueAct(&input);
		}

		else{// 중복회원이 있을 경우

			gotoxy(6, 22); gotoxy(6, 22); printf("                                                      "); 
			gotoxy(6, 22); printf("위의 중복 회원이 있습니다. 그래도 등록하시겠습니까? 1.예 2.아니오\n");

			printSingleData(userInfo, data_idx, search_num);

			input_chk_dup = _getch();
			switch (input_chk_dup){
			case '1':
				(*user_count)++;
				(*new_count)++;
				bf_update_save++;

				continueAct(&input);
				break;
			case '2':
				userInfo[*user_count].userId = 0;
				strcpy(userInfo[*user_count].userName, "");
				strcpy(userInfo[*user_count].userAddress, "");
				strcpy(userInfo[*user_count].userPhone, "");

				continueAct(&input);
				break;
			default:
				gotoxy(55, 22); printf("잘못된 입력입니다\n");
			}
		}

	}
}

// 회원검색(회원삭제 및 등록을 위한): 검색 데이터의 위치를 data_idx에 저장
void searchData2(UserInfo *userInfo, int user_count, int *data_idx, int *search_num)
{
	char data[100];
	char input = 0;

	int i;

	*search_num = 0; // 초기화: 안해주면 이후 검색시 data_idx가 이미 설정되어 있음

	printUp();
	gotoxy(55, 3); puts("회원 정보 관리");
	gotoxy(25, 6); printf("어떤 정보로 검색 하시겠습니까?\n");

	gotoxy(6, 22); puts("1.ID 2.이름 3.전화번호 0.메인메뉴");

	//input = _getch();
	/*if (input == QUIT)
		*search_num = -1;*/

	while (input != QUIT){

		input = _getch();

		switch (input){
		case '1':
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("6자리 숫자의 ID를 입력해주세요");
			gotoxy(25, 9); printf(" I        D : ");
			gotoxy(39, 9); gets(data);
			while (isNotValidId(data)){ // ID 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (atoi(data) == userInfo[i].userId){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			input = QUIT;
			break;

		case '2':
			gotoxy(6, 22); printf("1-4자의 한글 이름을 입력해 주세요");
			gotoxy(25, 9); printf("이       름 : "); gets(data);
			while (isNotValidChar(data)){ // 이름 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userName) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			input = QUIT;
			break;

		case '3':
			gotoxy(6, 22); printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n");
			gotoxy(25, 9); printf("전 화 번 호 : ");
			gets(data);
			while (isNotValidNum(data)){ // 전화번호 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userPhone) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			input = QUIT;
			break;

		case QUIT:
			*search_num = -1;
			break;

		default:
			gotoxy(55, 22); printf("잘못된 입력입니다\n");
		}

	}
}

// 회원삭제: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void deleteData(UserInfo *userInfo, int *data_idx, char *continue_act, int *user_count, int *search_num)
{
	char input=0;

	if (*search_num == -1){// 검색 없이 메인메뉴로 나갈 경우
		*continue_act = QUIT;
		return;
	}
	gotoxy(55, 3); puts("회원 정보 삭제");

	if (*search_num == 0){ // 찾는 데이터가 없을 경우
		gotoxy(6, 22); printf("위의 정보에 해당하는 회원이 존재하지 않습니다\n");
		continueAct(continue_act);
		return;
	}
	
	gotoxy(55, 22); printf("                    ");
	gotoxy(6, 22); printf("위의 회원을 삭제 하시겠습니까? 1.삭제 0.메인메뉴\n");

	input = _getch();

	switch (input){
	case '1':
		userInfo[*data_idx].userId = 0;
		strcpy(userInfo[*data_idx].userName, "");
		strcpy(userInfo[*data_idx].userAddress, "");
		strcpy(userInfo[*data_idx].userPhone, "");
		bf_update_save++;
		(*user_count)--;
		continueAct(continue_act);
		break;

	case QUIT:
		*continue_act = QUIT;
		return;

	default:
		gotoxy(55, 22); printf("잘못된 입력입니다\n");
		break;
	}
}

//회원 수정: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void updateData(UserInfo *userInfo, int *data_idx, char *continue_act, int *search_num)
{

	char input = 0;

	gotoxy(55, 3); puts("회원 정보 수정");

	if (*search_num == -1) // 검색 없이 메인메뉴로 나갈 경우
		return;

	else if (*search_num == 0){ // 찾는 데이터가 없을 경우
		gotoxy(6, 22); printf("위의 정보에 해당하는 회원이 존재하지 않습니다\n");
		continueAct(continue_act);
		return;
	}

	while (*continue_act != QUIT){

		gotoxy(25, 6); printf("어떤 정보를 수정 하시겠습니까?\n");
		gotoxy(25, 9); printf("                                                    ");
		gotoxy(6, 22); printf("                                                    ");
		gotoxy(6, 22); puts("1.이름 2.주소 3.전화번호 0.메인메뉴");

		input = _getch();

		switch(input){
		case '1':
			gotoxy(25, 9); printf("이       름 : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("수정하실 1-4자의 한글 이름을 입력해 주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // 입력받은 이름 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			}
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act);
			break;

		case '2':
			gotoxy(25, 9); printf("주       소 : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("수정하실 주소를 입력해 주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // 입력받은 주소 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			}
			bf_update_save++;
			continueAct(continue_act);
			break;

		case '3':
			gotoxy(25, 9); printf("전 화 번 호 : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("수정하실 전화번호를 000-0000-0000의 형식으로 '-'포함 입력해주세요");
			gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // 입력받은 전화번호 유효성 체크
				gotoxy(55, 22); puts("잘못된 입력입니다");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			}
			bf_update_save++;
			continueAct(continue_act);
			break;

		case QUIT:
			*continue_act = QUIT;
			break;

		default:
			gotoxy(55, 22); printf("잘못된 입력입니다\n");
		}
	}
	
}

//회원검색
void searchData(UserInfo *userInfo, int user_count, int *data_idx, int *search_num)
{
	char data[100];
	char input=0;
	char input2 = 0;

	int i;
	
	while (input != QUIT){
		
		for (i = 0; i < USER_COUNT; i++){
			data_idx[i] = 0;
		}
		*search_num = 0;

		system("cls");
		printf("회원검색\n");
		
		printf("어떤 정보?\n");

		puts("1.ID");
		puts("2.이름");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		if (input == '4')
			input = QUIT;
		
		switch (input){
		case ID:
			printf("ID는 6자리 숫자로 입력해주세요\n");
			printf("찾으실 ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID 유효성 체크
				printf("다시입력\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId){ // str->int 변환
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}
			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			continueAct(&input);
			break;

		case NAME:
			printf("찾으실 이름: ");
			gets(data);
			while (isNotValidChar(data)){ // 이름 유효성 체크
				printf("다시입력\n이름: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userName) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			continueAct(&input);
			break;
			
		case PHONE:
			printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n전화번호: ");
			printf("찾으실 전화번호: ");
			gets(data);
			while (isNotValidNum(data)){ // 전화번호 유효성 체크
				printf("다시입력\n전화번호: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // 검색 자료 출력
			continueAct(&input);
			break;
			
		case QUIT:
			break;
			
		default:
			printf("enter 1 to 4\n");
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

// 회원보기(회원삭제 및 등록을 위한): 검색 데이터의 정보 하나를 출력
void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num)
{
	int i;
	if (*search_num == 0){ // 찾는 데이터가 없을 경우
		gotoxy(17, 13); printf("위의 정보에 해당하는 회원이 존재하지 않습니다");
	}
	/*searchData2 함수에서 바로 메인메뉴로 나갈 경우에는
	이 함수로 들어오지 않기 때문에 data_idx==-1일 경우는 확인할 필요 없음*/

	else{ // 검색 자료 출력

		for (i = 0; i < *search_num; i++){
			gotoxy(6, 13); puts("   I D     이 름\t 주소\t\t\t\t 전화번호");
			gotoxy(6, 14+i); printf("  %d\t%s\t%s\t\t%s\n", userInfo[*data_idx].userId, userInfo[*data_idx].userName,
			userInfo[*data_idx].userAddress, userInfo[*data_idx].userPhone);

			data_idx++;
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
void continueAct(char *input)
{
	char continue_input;

	gotoxy(6, 22); printf("                                                                    "); 
	gotoxy(6, 22); printf("더 하시겠습니까? 1.예 0.메인메뉴\n");
	
	continue_input = _getch();
	
	switch(continue_input) {
		case '1':
			return;
		case QUIT:
			*input = QUIT;
			break;
		default:
			gotoxy(55, 22); printf("잘못된 입력입니다.\n");
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
