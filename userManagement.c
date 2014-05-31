#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "UserManagement.h"

#define USER_COUNT 50
#define PRINT_NUM 10


//UserInfo userInfo[USER_COUNT];
//int user_count;


int main(void)
{
	UserInfo userInfo[USER_COUNT];
	FILE *readFile = fopen("data.txt", "rt");

	int user_count;
	int new_count=1;
	int input=0;
	int data_idx=0;
	
	if (readFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}
	user_count=loadData(userInfo, readFile);
	
	
	while (input!='7'){

		menu();
		input = _getch();

		switch (input){
		case '1':
			printAllData(userInfo, user_count);
			break;
		case '2':
			insertData(userInfo, &user_count, &new_count);
			break;
		case '3':
			searchData2(userInfo, user_count, &data_idx);
			deleteData(userInfo, &data_idx);
			break;
		case '4':
			searchData2(userInfo, user_count, &data_idx);
			updateData(userInfo, &data_idx);
			break;
		case '5':
			searchData(userInfo, user_count);
			break;
		case '6':
			saveData(userInfo, user_count, new_count);
			break;
		case '7':
			printf("잘가요\n");
			system("exit");
			break;
		default:
			printf("enter 1 to 7\n");
		}
	}

	fclose(readFile);
	return 0;

}

void deleteData(UserInfo *userInfo, int *data_idx)
{
	char input=0;
	
	printf("삭제 할래?\n");

	puts("1. 삭제");
	puts("2. 메인메뉴로");
	
	input = _getch();

	while (input!='2'){

		switch (input){
		case '1':
			userInfo[*data_idx].userId = 0;
			strcpy(userInfo[*data_idx].userName, "");
			strcpy(userInfo[*data_idx].userAddress, "");
			strcpy(userInfo[*data_idx].userPhone, "");

			input = '2';
			break;
		case '2':
			break;
		default:
			printf("제대로 입력\n");
			break;
		}
	}

}

void searchData2(UserInfo *userInfo, int user_count, int *data_idx)
{
	char data[10];
	char input = 0;

	int i;
	while (input != '4'){

		system("cls");

		printf("어떤 정보?\n");

		puts("1.ID");
		puts("2.이름");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		switch (input){
		case '1':
			printf("찾으실 ID: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx);
			input = '4';
			break;
		case '2':
			printf("찾으실 이름: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userName) == 0)
					*data_idx = i;
			}
			printSingleData(userInfo, data_idx);
			input = '4';
			break;

		case '3':
			printf("찾으실 전화번호: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone) == 0)
					*data_idx = i;
			}
			printSingleData(userInfo, data_idx);
			input = '4';
			break;

		case '4':
			break;

		default:
			printf("enter 1 to 4\n");
		}

	}
}


void updateData(UserInfo *userInfo, int *data_idx)
{

	char input = 0;

	while (input != '4'){

		printf("뭐 수정?\n");

		puts("1.이름");
		puts("2.주소");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();

		switch(input){
		case '1':
			if (data_idx != 0){
				printf("수정하실 이름을 입력: ");
				scanf("%s", userInfo[*data_idx].userName);
			}
			else
				printf("찾는거 없음\n");
			break;
		case '2':
			if (data_idx != 0){
				printf("수정하실 주소 입력: ");
				scanf("%s", userInfo[*data_idx].userAddress);
			}
			else
				printf("찾는거 없음\n");
			break;
		case '3':
			if (data_idx != 0){
				printf("수정하실 전화번호을 입력: ");
				scanf("%s", userInfo[*data_idx].userPhone);
			}
			else
				printf("찾는거 없음\n");
			break;
		case '4':
			break;
		default:
			printf("다시 눌러라\n");
		}
	}
	
}

void printSingleData(UserInfo *userInfo, int *data_idx)
{
	if (*data_idx == 0)
		printf("찾는 데이터 없어\n");
	else
		printf("%d\t%s\t%s\t%s\n", userInfo[*data_idx].userId, userInfo[*data_idx].userName, 
		userInfo[*data_idx].userAddress, userInfo[*data_idx].userPhone);
}

void searchData(UserInfo *userInfo, int user_count)
{
	char data[100];
	char input=0;
	char input2 = 0;

	int i;
	int data_idx;
	int whileLoopCount = 0;

	system("cls");
	while (input != '4'){
		
		if (whileLoopCount > 0){

			int didInputDirty = 0;
			while (1){
				if( didInputDirty == 0 ) 
					printf("또 검색? 1.예 2.메인메뉴\n");
				
				input2 = _getch();
				if (input2 == '1')
					break;
				else if (input2 == '2')
					return;
				else if (didInputDirty == 0)
				{
					printf("제대로 입력하시오\n");
					didInputDirty = 1;
				}
			}
		}
		system("cls");
		whileLoopCount++;

		printf("어떤 정보?\n");

		puts("1.ID");
		puts("2.이름");
		puts("3.전화번호");
		puts("4.메인메뉴");

		input = _getch();
		
		switch (input){
		case '1':
			printf("찾으실 ID: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId)
					data_idx = i;
			}
			
			printSingleData(userInfo, &data_idx);
			break;
		case '2':
			printf("찾으실 이름: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data,userInfo[i].userName)==0)
					data_idx = i;
			}
			printSingleData(userInfo, &data_idx);
			break;
			
		case '3':
			printf("찾으실 전화번호: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone)==0)
					data_idx = i;
			}
			printSingleData(userInfo, &data_idx);
			break;
			
		case '4':
			break;
			
		default:
			printf("enter 1 to 4\n");
		}
		
	}
}

int saveData(UserInfo *userInfo, int user_count)
{
	FILE *saveFile=fopen("data.txt", "wt");
	int i;
	
	if (saveFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}

	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId!=0)
			fprintf(saveFile, "%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	fclose(saveFile);

	printf("저장 되었습니다\n종료하려면 아무키나\n");
	_getch();

	return 0;
}


int isNotValidChar(char *data)
{
	int len = strlen(data);

	if (len == 0)
		return 1;

	return 0;
}

int isNotValidNum(char *data)
{
	int i;
	int len = strlen(data);
	
	if (len != 13)
		return 1;

	for (i = 0; i < len; i++){

		if (i == 3 || i == 8){
			if (*data != 45)
				return 1;
		}

		else{
			if (*data<48 || *data>57)
				return 1;
		}
		data++;
	}

	return 0;

}


void insertData(UserInfo *userInfo, int *user_count, int *new_count)
{
	char input=0;
	
	while (input != '2'){

		system("cls");

		printf("추가하실 데이터를 입력하세요\n");

		userInfo[*user_count].userId = 141100 + *new_count;
		printf("ID\t: %d\n", userInfo[*user_count].userId);
	
		printf("이름\t: "); //이0씨를 위해 이름에 숫자가 들어가는 체크는 하지 않았습니다.(단호박)
		gets(userInfo[*user_count].userName);
		while (isNotValidChar(userInfo[*user_count].userName)){
			printf("다시입력\n이름: ");
			gets(userInfo[*user_count].userName);
		}
		
		printf("주소\t: ");
		gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){
			printf("다시입력\n주소: ");
			gets(userInfo[*user_count].userAddress);
		}

		
		printf("전화번호: ");
		gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){
			printf("다시입력\n전화번호: ");
			gets(userInfo[*user_count].userPhone);
		}

		(*user_count)++;
		(*new_count)++;

		printf("계속해서 입력하시겠습니까\n");
		printf("1: 예 2: 메인메뉴로\n");
		
		input = 0;

		while (input != '1' && input != '2'){
			input = _getch();
			
			switch (input){
			case '1':
				break;
			case '2':
				break;
			default:
				printf("enter 1 to 2\n");
			}
		}
	}
}

int loadData(UserInfo *userInfo, FILE *fp){

	int i = 0;
	int count = 0;

	fscanf(fp, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n", 
		&userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);

	while ((fscanf(fp,"%d\t%[^\t]\t%[^\t]\t%[^\n]\n", 
		&userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone)) != EOF){
		
		i++;
		count++;
	}

	return count;
}

void printAllData(UserInfo *userInfo, int user_count){

	int i = 0;
	int user_num = 1;
	int page = 1;
	char input;

	system("cls");

	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId != 0)
			printf("%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
		//user_num++;
		//i++;
	}

	input = _getch(); //잘못된거 누르면 3 으로도 다시 안가져
	while (input != '3'){
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

//
//void printData(UserInfo *userInfo, int user_count){
//
//	int i=0;
//	int user_num = 1;
//	int input;
//
//	system("cls");
//
//	printf("=====================================================\n");
//	printf("ID\t이름\t주소\t\t\t연락처\n");
//	printf("-----------------------------------------------------\n");
//
//	while (i < user_count){
//
//		printf("%d\t%s\t%s\t%s\n",
//			userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
//		user_num++;
//		i++;
//
//		if (user_num > PRINT_NUM){
//			user_num = 1;
//			printf("------------------------------------------%d page-----\n", i / PRINT_NUM);
//			printf("=====================================================\n");
//			printf("      1:이전 페이지 2.다음 페이지 3.메인 메뉴 \n");
//			printf("=====================================================\n");
//			
//			input = _getch();
//
//			switch (input){
//				case '1': 
//					i -= 2*PRINT_NUM;
//					if (i < 1)
//						i = 1;
//					break;
//				case '2':
//					i = user_count + 2;
//					break;
//				case '3': 
//					
//					break;
//				default:
//					printf("enter 1 to 3\n");
//					break;
//			}
//
//			system("cls");
//			printf("=====================================================\n");
//			printf("ID\t이름\t주소\t\t\t연락처\n");
//			printf("-----------------------------------------------------\n");
//		}
//
//		if (i == user_count){
//			
//			while (PRINT_NUM%user_num == 0){
//				printf("\n");
//				user_num++;
//			}
//			printf("------------------------------------------%d page-----\n", i / PRINT_NUM);
//			printf("=====================================================\n");
//			printf("      1:이전 페이지 2.다음 페이지 3.메인 메뉴 \n");
//			printf("=====================================================\n");
//
//			switch (_getch()){
//			case '1':
//				i = ((i/PRINT_NUM)-2)*PRINT_NUM+1;
//				user_num = 1;
//				
//				system("cls");
//				printf("=====================================================\n");
//				printf("ID\t이름\t주소\t\t\t연락처\n");
//				printf("-----------------------------------------------------\n");
//				break;
//			case '2':
//				i = i-user_count%PRINT_NUM;
//				user_num = 1;
//
//				system("cls");
//				printf("=====================================================\n");
//				printf("ID\t이름\t주소\t\t\t연락처\n");
//				printf("-----------------------------------------------------\n");
//				break;
//			case '3':
//				i = user_count + 2;
//				break;
//			default:
//				printf("enter 1 to 3\n");
//			}
//			
//		}
//	}
//}
