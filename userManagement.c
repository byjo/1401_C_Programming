#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include "UserManagement.h"

#define USER_COUNT 50
#define PRINT_NUM 10
//Q.종료하는 문자 변수를 define하고 싶은데 안되는듯 ㅠ_ㅠ 
//#define	QUIT_CHAR '0'; 

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
	int data_idx=0;

	char input = 0;
	char continue_act;
	
	if (readFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}
	user_count=loadData(userInfo, readFile); // data.txt를 userInfo 구조체 배열에 입력, user_count 갱신
	
	while (input!='7'){

		menu();
		continue_act = 0;

		input = _getch();
		switch (input){
		case '1': // 회원보기
			printAllData(userInfo, user_count);
			break;
		case '2': // 회원등록
			insertData(userInfo, &user_count, &new_count);
			break;
		case '3': // 회원검색 후 검색 회원 삭제
			while (continue_act!='0'){
				searchData2(userInfo, user_count, &data_idx);
				deleteData(userInfo, &data_idx, &continue_act);
			}
			break;
		case '4': // 회원검색 후 검색 회원 수정
			searchData2(userInfo, user_count, &data_idx);
			updateData(userInfo, &data_idx);
			break;
		case '5': // 회원검색
			searchData(userInfo, user_count);
			break;
		case '6': // 저장
			saveData(userInfo, user_count, 0);
			break;
		case '7': // 종료
			if (bf_update_save > 0)
				checkSave(userInfo, user_count);
			printf("잘가요\n");
			system("exit");
			break;
		case '0':
			break;
		default:
			printf("enter 1 to 7\n");
		}
	}

	fclose(readFile);
	return 0;

}

void menu(){

	system("cls");

	puts("");
	puts("==== Menu ====");
	puts("1.회원 보기");
	puts("2.회원 등록");
	puts("3.회원 삭제");
	puts("4.회원 수정");
	puts("5.회원 검색");
	puts("6.저장");
	puts("7.종료");
	puts("===============");

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

void printAllData(UserInfo *userInfo, int user_count){

	int i = 0;
	int user_num = 1;

	system("cls");

	printf("=====================================================\n");
	printf("ID\t이름\t주소\t\t\t연락처\n");
	printf("-----------------------------------------------------\n");

	while (i < user_count){

		printf("%d\t%s\t%s\t%s\n",
			userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
		user_num++;
		i++;

		if (user_num > PRINT_NUM){
			user_num = 1;
			printf("------------------------------------------%d page-----\n", i / PRINT_NUM);
			printf("=====================================================\n");
			printf("      1:이전 페이지 2.다음 페이지 3.메인 메뉴 \n");
			printf("=====================================================\n");

			switch (_getch()){
			case '1':
				i -= 2 * PRINT_NUM;
				if (i < 1)
					i = 0;
				break;
			case '2':
				printf("\n");
				break;
			case '3':
				i = user_count + 2;
				break;
			default:
				printf("enter 1 to 3\n");
			}

			system("cls");
			printf("=====================================================\n");
			printf("ID\t이름\t주소\t\t\t연락처\n");
			printf("-----------------------------------------------------\n");
		}

		if (i == user_count){

			while (PRINT_NUM%user_num == 0){
				printf("\n");
				user_num++;
			}
			printf("------------------------------------------%d page-----\n", i / PRINT_NUM);
			printf("=====================================================\n");
			printf("      1:이전 페이지 2.다음 페이지 3.메인 메뉴 \n");
			printf("=====================================================\n");

			switch (_getch()){
			case '1':
				i = ((i / PRINT_NUM) - 2)*PRINT_NUM + 1;
				user_num = 1;

				system("cls");
				printf("=====================================================\n");
				printf("ID\t이름\t주소\t\t\t연락처\n");
				printf("-----------------------------------------------------\n");
				break;
			case '2':
				i = i - user_count%PRINT_NUM;
				user_num = 1;

				system("cls");
				printf("=====================================================\n");
				printf("ID\t이름\t주소\t\t\t연락처\n");
				printf("-----------------------------------------------------\n");
				break;
			case '3':
				i = user_count + 2;
				break;
			default:
				printf("enter 1 to 3\n");
			}

		}
	}
}

/*
//회원보기
void printAllData(UserInfo *userInfo, int user_count){

	int i = 0;
	int user_num = 1;
	int page = 1;
	char input=0;

	system("cls");

	printf("회원보기\n");

	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId != 0)
			printf("%d\t%s\t%s\t%s\n",
			userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	printf("3.메인메뉴 이동\n");

	while (input != '3'){
		input = _getch();
		switch (input) {
		case '1':
			break;
		case '2':
			break;
		case '3':
			break;
		default:
			break;
		}
	}

	return;
}
*/


// 회원등록 
void insertData(UserInfo *userInfo, int *user_count, int *new_count)
{
	char input = 0;
	char input_chk_dup = 0;

	int biggerId;
	int data_idx;
	int warningMsg = 0;

	while (input != '0'){

		system("cls");

		printf("회원등록\n");

		printf("추가하실 데이터를 입력하세요\n");

		biggerId = checkBigId(userInfo, user_count); // 가장 큰 ID값 확인
		userInfo[*user_count].userId = biggerId + 1;
		printf("ID\t: %d\n", userInfo[*user_count].userId);

		printf("이름\t: ");
		gets(userInfo[*user_count].userName);
		while (isNotValidChar(userInfo[*user_count].userName)){ // 이름 유효성 체크
			
			if (warningMsg == 0){
				printf("다시입력\n이름\t: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userName);
		}

		warningMsg = 0;
		printf("주소\t: ");
		gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // 주소 유효성 체크
			
			if (warningMsg == 0){
				printf("다시입력\n주소\t: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userAddress);
		}

		warningMsg = 0;
		printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n전화번호: ");
		gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // 전화번호 유효성 체크

			if (warningMsg == 0){
				printf("다시입력\n전화번호: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userPhone);
		}

		data_idx = checkDuplicated(userInfo, user_count); // 중복 회원 체크, 존재시 중복회원의 data_idx로 갱신

		if (data_idx == -1){ // 중복회원이 없을 경우

			printf("등록 되었습니다\n");
			(*user_count)++;
			(*new_count)++;
			bf_update_save++;

			continueAct(&input);
		}

		else{// 중복회원이 있을 경우

			printf("아래와 같은 중복 회원이 있습니다. 그래도 등록하시겠습니까? 1.예 2.아니오\n");

			printSingleData(userInfo, &data_idx);

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
				printf("잘눌러라\n");
			}
		}

	}
}

// 회원검색(회원삭제 및 등록을 위한): 검색 데이터의 위치를 data_idx에 저장
void searchData2(UserInfo *userInfo, int user_count, int *data_idx)
{
	char data[100];
	char input = 0;

	int i;
	while (input != '4'){
		*data_idx = 0; // 초기화: 안해주면 이후 검색시 data_idx가 이미 설정되어 있음

		system("cls");

		printf("어떤 정보?\n");

		puts("1.ID");
		puts("2.이름");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		switch (input){
		case '1':
			printf("ID는 6자리 숫자로 입력해주세요\n");
			printf("찾으실 ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID 유효성 체크
				printf("다시입력\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (atoi(data) == userInfo[i].userId)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // 검색 자료 출력
			input = '4';
			break;

		case '2':
			printf("찾으실 이름: ");
			gets(data);
			while (isNotValidChar(data)){ // 이름 유효성 체크
				printf("다시입력\n이름: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userName) == 0)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // 검색 자료 출력
			input = '4';
			break;

		case '3':
			printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n");
			printf("찾으실 전화번호: ");
			gets(data);
			while (isNotValidNum(data)){ // 전화번호 유효성 체크
				printf("다시입력\n전화번호: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx 갱신
				if (strcmp(data, userInfo[i].userPhone) == 0)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // 검색 자료 출력
			input = '4';
			break;

		case '4':
			*data_idx = -1;
			break;

		default:
			printf("enter 1 to 4\n");
		}

	}
}

// 회원삭제: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void deleteData(UserInfo *userInfo, int *data_idx, char *continue_act)
{
	char input=0;

	if (*data_idx == -1) // 검색 없이 메인메뉴로 나갈 경우
		return;

	else if (*data_idx == 0){ // 찾는 데이터가 없을 경우
		printf("데이터 없다\n");
		continueAct(continue_act);
		return;
	}
	
	printf("삭제 할래?\n");

	puts("1. 삭제");
	puts("2. 메인메뉴");

	input = _getch();

	if (input == '2'){
		input = '0';
		*continue_act = '0';
	}

	switch (input){
	case '1':
		userInfo[*data_idx].userId = 0;
		strcpy(userInfo[*data_idx].userName, "");
		strcpy(userInfo[*data_idx].userAddress, "");
		strcpy(userInfo[*data_idx].userPhone, "");
		bf_update_save++;
		continueAct(continue_act);
		break;

	case '0':
		break;

	default:
		printf("제대로 입력\n");
		break;
	}
}

//회원 수정: 회원 검색 후 data_idx를 통해 userInfo의 자료에 접근
void updateData(UserInfo *userInfo, int *data_idx)
{

	char input = 0;

	if (*data_idx == -1) // 검색 없이 메인메뉴로 나갈 경우
		return;

	else if (*data_idx == 0){ // 찾는 데이터가 없을 경우
		printf("데이터 없다, 나가라면 아무거나 눌러");
		_getch();
		return;
	}

	while (input != '4'){

		printf("뭐 수정?\n");

		puts("1.이름");
		puts("2.주소");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		switch(input){
		case '1':
			printf("수정하실 이름을 입력: ");
			scanf("%s", userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // 입력받은 이름 유효성 체크
				printf("다시입력\n이름: ");
				scanf("%s", userInfo[*data_idx].userName);
			}
			bf_update_save++;
			break;

		case '2':
			printf("수정하실 주소 입력: ");
			scanf("%s", userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // 입력받은 주소 유효성 체크
				printf("다시입력\n주소: ");
				scanf("%s",userInfo[*data_idx].userAddress);
			}
			bf_update_save++;
			break;

		case '3':
			printf("수정하실 전화번호을 입력: ");
			scanf("%s", userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // 입력받은 전화번호 유효성 체크
				printf("다시입력\n전화번호: ");
				scanf("%s",userInfo[*data_idx].userPhone);
			}
			bf_update_save++;
			break;

		case '4':
			break;

		default:
			printf("다시 눌러라\n");
		}
	}
	
}

//회원검색
void searchData(UserInfo *userInfo, int user_count)
{
	char data[100];
	char input=0;
	char input2 = 0;

	int i;
	int data_idx;

	while (input != '0'){
		
		data_idx = 0;

		system("cls");
		printf("회원검색\n");
		
		printf("어떤 정보?\n");

		puts("1.ID");
		puts("2.이름");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		if (input == '4')
			input = '0';
		
		switch (input){
		case '1':
			printf("ID는 6자리 숫자로 입력해주세요\n");
			printf("찾으실 ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID 유효성 체크
				printf("다시입력\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId) // str->int 변환
					data_idx = i;
			}
			
			printSingleData(userInfo, &data_idx); // 검색 자료 출력
			continueAct(&input);
			break;

		case '2':
			printf("찾으실 이름: ");
			gets(data);
			while (isNotValidChar(data)){ // 이름 유효성 체크
				printf("다시입력\n이름: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data,userInfo[i].userName)==0)
					data_idx = i;
			}

			printSingleData(userInfo, &data_idx); // 검색 자료 출력
			continueAct(&input);
			break;
			
		case '3':
			printf("(000-0000-0000의 형식으로 '-'포함 입력해주세요)\n전화번호: ");
			printf("찾으실 전화번호: ");
			gets(data);
			while (isNotValidNum(data)){ // 전화번호 유효성 체크
				printf("다시입력\n전화번호: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone)==0)
					data_idx = i;
			}

			printSingleData(userInfo, &data_idx); // 검색 자료 출력
			continueAct(&input);
			break;
			
		case '0':
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

	system("cls");
	printf("회원저장\n");

	fprintf(saveFile, "회원 아이디	회원 이름	회원 주소	핸드폰 번호\n"); //Index 저장
	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId!=0)
			fprintf(saveFile, "%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	fclose(saveFile);

	bf_update_save = 0;
	printf("저장 되었습니다\n");
	
	if (chk_exit){
		printf("종료하려면 아무키나 누르세요\n");
		_getch();
		return 0;
	}
	
	printf("메인메뉴로 돌아가려면 아무키나 누르세요\n");
	_getch();
	
	return 0;
}

// 회원보기(회원삭제 및 등록을 위한): 검색 데이터의 정보 하나를 출력
void printSingleData(UserInfo *userInfo, int *data_idx)
{
	if (*data_idx == 0) // 찾는 데이터가 없을 경우
		printf("찾는 데이터 없어\n");

	/*searchData2 함수에서 바로 메인메뉴로 나갈 경우에는
	이 함수로 들어오지 않기 때문에 data_idx==-1일 경우는 확인할 필요 없음*/

	else // 검색 자료 출력
		printf("%d\t%s\t%s\t%s\n", userInfo[*data_idx].userId, userInfo[*data_idx].userName,
		userInfo[*data_idx].userAddress, userInfo[*data_idx].userPhone);
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
1) 중복 없을시 -1 return
2) 중복 존재시 중복되는 기존 데이터의 index값 반환
*/
int checkDuplicated(UserInfo *userInfo, int *user_count)
{
	int i;

	for (i = 0; i < *user_count; i++){
		if (strcmp(userInfo[*user_count].userName, userInfo[i].userName) == 0
			&& strcmp(userInfo[*user_count].userAddress, userInfo[i].userAddress) == 0
			&& strcmp(userInfo[*user_count].userPhone, userInfo[i].userPhone) == 0)

			return i;
	}

	return -1;
}

// 종료 전 변경사항이 저장되지 않았을 때 저장여부 확인
void checkSave(UserInfo *userInfo, int user_count)
{
	char input;
	int chk_exit = 1;

	printf("%d개의 저장되지 않은 사항이 있습니다.\n저장하시고 종료하시겠습니까? 1.예 2.아니오\n", bf_update_save);
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

	printf("더 하시겠습니까? 1.예 2.메인메뉴\n");
	continue_input = _getch();

	if (continue_input == '1')
		return;
	else if (continue_input == '2'){
		*input = '0';
	}
	else
		printf("1이나 2눌러라\n");

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

/* 이름, 주소 유효성 체크:
1. NULL값이 아닌지 확인
2. 맨 첫번째 칸 띄어쓰기 확인
+) 이0씨를 위해 이름에 숫자가 들어가는 체크는 하지 않았습니다.(단호박)
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
