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
			printf("�߰���\n");
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
	
	printf("���� �ҷ�?\n");

	puts("1. ����");
	puts("2. ���θ޴���");
	
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
			printf("����� �Է�\n");
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

		printf("� ����?\n");

		puts("1.ID");
		puts("2.�̸�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		switch (input){
		case '1':
			printf("ã���� ID: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx);
			input = '4';
			break;
		case '2':
			printf("ã���� �̸�: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userName) == 0)
					*data_idx = i;
			}
			printSingleData(userInfo, data_idx);
			input = '4';
			break;

		case '3':
			printf("ã���� ��ȭ��ȣ: ");
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

		printf("�� ����?\n");

		puts("1.�̸�");
		puts("2.�ּ�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		switch(input){
		case '1':
			if (data_idx != 0){
				printf("�����Ͻ� �̸��� �Է�: ");
				scanf("%s", userInfo[*data_idx].userName);
			}
			else
				printf("ã�°� ����\n");
			break;
		case '2':
			if (data_idx != 0){
				printf("�����Ͻ� �ּ� �Է�: ");
				scanf("%s", userInfo[*data_idx].userAddress);
			}
			else
				printf("ã�°� ����\n");
			break;
		case '3':
			if (data_idx != 0){
				printf("�����Ͻ� ��ȭ��ȣ�� �Է�: ");
				scanf("%s", userInfo[*data_idx].userPhone);
			}
			else
				printf("ã�°� ����\n");
			break;
		case '4':
			break;
		default:
			printf("�ٽ� ������\n");
		}
	}
	
}

void printSingleData(UserInfo *userInfo, int *data_idx)
{
	if (*data_idx == 0)
		printf("ã�� ������ ����\n");
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
					printf("�� �˻�? 1.�� 2.���θ޴�\n");
				
				input2 = _getch();
				if (input2 == '1')
					break;
				else if (input2 == '2')
					return;
				else if (didInputDirty == 0)
				{
					printf("����� �Է��Ͻÿ�\n");
					didInputDirty = 1;
				}
			}
		}
		system("cls");
		whileLoopCount++;

		printf("� ����?\n");

		puts("1.ID");
		puts("2.�̸�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();
		
		switch (input){
		case '1':
			printf("ã���� ID: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId)
					data_idx = i;
			}
			
			printSingleData(userInfo, &data_idx);
			break;
		case '2':
			printf("ã���� �̸�: ");
			gets(data);
			for (i = 0; i < user_count; i++){
				if (strcmp(data,userInfo[i].userName)==0)
					data_idx = i;
			}
			printSingleData(userInfo, &data_idx);
			break;
			
		case '3':
			printf("ã���� ��ȭ��ȣ: ");
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

	printf("���� �Ǿ����ϴ�\n�����Ϸ��� �ƹ�Ű��\n");
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

		printf("�߰��Ͻ� �����͸� �Է��ϼ���\n");

		userInfo[*user_count].userId = 141100 + *new_count;
		printf("ID\t: %d\n", userInfo[*user_count].userId);
	
		printf("�̸�\t: "); //��0���� ���� �̸��� ���ڰ� ���� üũ�� ���� �ʾҽ��ϴ�.(��ȣ��)
		gets(userInfo[*user_count].userName);
		while (isNotValidChar(userInfo[*user_count].userName)){
			printf("�ٽ��Է�\n�̸�: ");
			gets(userInfo[*user_count].userName);
		}
		
		printf("�ּ�\t: ");
		gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){
			printf("�ٽ��Է�\n�ּ�: ");
			gets(userInfo[*user_count].userAddress);
		}

		
		printf("��ȭ��ȣ: ");
		gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){
			printf("�ٽ��Է�\n��ȭ��ȣ: ");
			gets(userInfo[*user_count].userPhone);
		}

		(*user_count)++;
		(*new_count)++;

		printf("����ؼ� �Է��Ͻðڽ��ϱ�\n");
		printf("1: �� 2: ���θ޴���\n");
		
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

	input = _getch(); //�߸��Ȱ� ������ 3 ���ε� �ٽ� �Ȱ���
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
	puts("1.ȸ�� ����");
	puts("2.ȸ�� ���");
	puts("3.ȸ�� ����");
	puts("4.ȸ�� ����");
	puts("5.ȸ�� �˻�");
	puts("6.����");
	puts("7.����");
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
//	printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
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
//			printf("      1:���� ������ 2.���� ������ 3.���� �޴� \n");
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
//			printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
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
//			printf("      1:���� ������ 2.���� ������ 3.���� �޴� \n");
//			printf("=====================================================\n");
//
//			switch (_getch()){
//			case '1':
//				i = ((i/PRINT_NUM)-2)*PRINT_NUM+1;
//				user_num = 1;
//				
//				system("cls");
//				printf("=====================================================\n");
//				printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
//				printf("-----------------------------------------------------\n");
//				break;
//			case '2':
//				i = i-user_count%PRINT_NUM;
//				user_num = 1;
//
//				system("cls");
//				printf("=====================================================\n");
//				printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
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
