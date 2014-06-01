#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include "UserManagement.h"

#define USER_COUNT 50
#define PRINT_NUM 10
//Q.�����ϴ� ���� ������ define�ϰ� ������ �ȵǴµ� ��_�� 
//#define	QUIT_CHAR '0'; 

//Q. ��� �Լ����� ȣ��Ǵ� ������ ��� ���������� �ƴ� ���������� �޴� ���� �� ����������?
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
	user_count=loadData(userInfo, readFile); // data.txt�� userInfo ����ü �迭�� �Է�, user_count ����
	
	while (input!='7'){

		menu();
		continue_act = 0;

		input = _getch();
		switch (input){
		case '1': // ȸ������
			printAllData(userInfo, user_count);
			break;
		case '2': // ȸ�����
			insertData(userInfo, &user_count, &new_count);
			break;
		case '3': // ȸ���˻� �� �˻� ȸ�� ����
			while (continue_act!='0'){
				searchData2(userInfo, user_count, &data_idx);
				deleteData(userInfo, &data_idx, &continue_act);
			}
			break;
		case '4': // ȸ���˻� �� �˻� ȸ�� ����
			searchData2(userInfo, user_count, &data_idx);
			updateData(userInfo, &data_idx);
			break;
		case '5': // ȸ���˻�
			searchData(userInfo, user_count);
			break;
		case '6': // ����
			saveData(userInfo, user_count, 0);
			break;
		case '7': // ����
			if (bf_update_save > 0)
				checkSave(userInfo, user_count);
			printf("�߰���\n");
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
	puts("1.ȸ�� ����");
	puts("2.ȸ�� ���");
	puts("3.ȸ�� ����");
	puts("4.ȸ�� ����");
	puts("5.ȸ�� �˻�");
	puts("6.����");
	puts("7.����");
	puts("===============");

}

// ȸ�� �ҷ�����: ���α׷� ����� �� �� ����, �� ù�� �ε��� ����������
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
	printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
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
			printf("      1:���� ������ 2.���� ������ 3.���� �޴� \n");
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
			printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
			printf("-----------------------------------------------------\n");
		}

		if (i == user_count){

			while (PRINT_NUM%user_num == 0){
				printf("\n");
				user_num++;
			}
			printf("------------------------------------------%d page-----\n", i / PRINT_NUM);
			printf("=====================================================\n");
			printf("      1:���� ������ 2.���� ������ 3.���� �޴� \n");
			printf("=====================================================\n");

			switch (_getch()){
			case '1':
				i = ((i / PRINT_NUM) - 2)*PRINT_NUM + 1;
				user_num = 1;

				system("cls");
				printf("=====================================================\n");
				printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
				printf("-----------------------------------------------------\n");
				break;
			case '2':
				i = i - user_count%PRINT_NUM;
				user_num = 1;

				system("cls");
				printf("=====================================================\n");
				printf("ID\t�̸�\t�ּ�\t\t\t����ó\n");
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
//ȸ������
void printAllData(UserInfo *userInfo, int user_count){

	int i = 0;
	int user_num = 1;
	int page = 1;
	char input=0;

	system("cls");

	printf("ȸ������\n");

	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId != 0)
			printf("%d\t%s\t%s\t%s\n",
			userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	printf("3.���θ޴� �̵�\n");

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


// ȸ����� 
void insertData(UserInfo *userInfo, int *user_count, int *new_count)
{
	char input = 0;
	char input_chk_dup = 0;

	int biggerId;
	int data_idx;
	int warningMsg = 0;

	while (input != '0'){

		system("cls");

		printf("ȸ�����\n");

		printf("�߰��Ͻ� �����͸� �Է��ϼ���\n");

		biggerId = checkBigId(userInfo, user_count); // ���� ū ID�� Ȯ��
		userInfo[*user_count].userId = biggerId + 1;
		printf("ID\t: %d\n", userInfo[*user_count].userId);

		printf("�̸�\t: ");
		gets(userInfo[*user_count].userName);
		while (isNotValidChar(userInfo[*user_count].userName)){ // �̸� ��ȿ�� üũ
			
			if (warningMsg == 0){
				printf("�ٽ��Է�\n�̸�\t: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userName);
		}

		warningMsg = 0;
		printf("�ּ�\t: ");
		gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // �ּ� ��ȿ�� üũ
			
			if (warningMsg == 0){
				printf("�ٽ��Է�\n�ּ�\t: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userAddress);
		}

		warningMsg = 0;
		printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n��ȭ��ȣ: ");
		gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // ��ȭ��ȣ ��ȿ�� üũ

			if (warningMsg == 0){
				printf("�ٽ��Է�\n��ȭ��ȣ: ");
				warningMsg = 1;
			}
			else
				gets(userInfo[*user_count].userPhone);
		}

		data_idx = checkDuplicated(userInfo, user_count); // �ߺ� ȸ�� üũ, ����� �ߺ�ȸ���� data_idx�� ����

		if (data_idx == -1){ // �ߺ�ȸ���� ���� ���

			printf("��� �Ǿ����ϴ�\n");
			(*user_count)++;
			(*new_count)++;
			bf_update_save++;

			continueAct(&input);
		}

		else{// �ߺ�ȸ���� ���� ���

			printf("�Ʒ��� ���� �ߺ� ȸ���� �ֽ��ϴ�. �׷��� ����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n");

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
				printf("�ߴ�����\n");
			}
		}

	}
}

// ȸ���˻�(ȸ������ �� ����� ����): �˻� �������� ��ġ�� data_idx�� ����
void searchData2(UserInfo *userInfo, int user_count, int *data_idx)
{
	char data[100];
	char input = 0;

	int i;
	while (input != '4'){
		*data_idx = 0; // �ʱ�ȭ: �����ָ� ���� �˻��� data_idx�� �̹� �����Ǿ� ����

		system("cls");

		printf("� ����?\n");

		puts("1.ID");
		puts("2.�̸�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		switch (input){
		case '1':
			printf("ID�� 6�ڸ� ���ڷ� �Է����ּ���\n");
			printf("ã���� ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID ��ȿ�� üũ
				printf("�ٽ��Է�\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (atoi(data) == userInfo[i].userId)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // �˻� �ڷ� ���
			input = '4';
			break;

		case '2':
			printf("ã���� �̸�: ");
			gets(data);
			while (isNotValidChar(data)){ // �̸� ��ȿ�� üũ
				printf("�ٽ��Է�\n�̸�: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (strcmp(data, userInfo[i].userName) == 0)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // �˻� �ڷ� ���
			input = '4';
			break;

		case '3':
			printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n");
			printf("ã���� ��ȭ��ȣ: ");
			gets(data);
			while (isNotValidNum(data)){ // ��ȭ��ȣ ��ȿ�� üũ
				printf("�ٽ��Է�\n��ȭ��ȣ: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (strcmp(data, userInfo[i].userPhone) == 0)
					*data_idx = i;
			}

			printSingleData(userInfo, data_idx); // �˻� �ڷ� ���
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

// ȸ������: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void deleteData(UserInfo *userInfo, int *data_idx, char *continue_act)
{
	char input=0;

	if (*data_idx == -1) // �˻� ���� ���θ޴��� ���� ���
		return;

	else if (*data_idx == 0){ // ã�� �����Ͱ� ���� ���
		printf("������ ����\n");
		continueAct(continue_act);
		return;
	}
	
	printf("���� �ҷ�?\n");

	puts("1. ����");
	puts("2. ���θ޴�");

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
		printf("����� �Է�\n");
		break;
	}
}

//ȸ�� ����: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void updateData(UserInfo *userInfo, int *data_idx)
{

	char input = 0;

	if (*data_idx == -1) // �˻� ���� ���θ޴��� ���� ���
		return;

	else if (*data_idx == 0){ // ã�� �����Ͱ� ���� ���
		printf("������ ����, ������� �ƹ��ų� ����");
		_getch();
		return;
	}

	while (input != '4'){

		printf("�� ����?\n");

		puts("1.�̸�");
		puts("2.�ּ�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		switch(input){
		case '1':
			printf("�����Ͻ� �̸��� �Է�: ");
			scanf("%s", userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // �Է¹��� �̸� ��ȿ�� üũ
				printf("�ٽ��Է�\n�̸�: ");
				scanf("%s", userInfo[*data_idx].userName);
			}
			bf_update_save++;
			break;

		case '2':
			printf("�����Ͻ� �ּ� �Է�: ");
			scanf("%s", userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // �Է¹��� �ּ� ��ȿ�� üũ
				printf("�ٽ��Է�\n�ּ�: ");
				scanf("%s",userInfo[*data_idx].userAddress);
			}
			bf_update_save++;
			break;

		case '3':
			printf("�����Ͻ� ��ȭ��ȣ�� �Է�: ");
			scanf("%s", userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // �Է¹��� ��ȭ��ȣ ��ȿ�� üũ
				printf("�ٽ��Է�\n��ȭ��ȣ: ");
				scanf("%s",userInfo[*data_idx].userPhone);
			}
			bf_update_save++;
			break;

		case '4':
			break;

		default:
			printf("�ٽ� ������\n");
		}
	}
	
}

//ȸ���˻�
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
		printf("ȸ���˻�\n");
		
		printf("� ����?\n");

		puts("1.ID");
		puts("2.�̸�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		if (input == '4')
			input = '0';
		
		switch (input){
		case '1':
			printf("ID�� 6�ڸ� ���ڷ� �Է����ּ���\n");
			printf("ã���� ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID ��ȿ�� üũ
				printf("�ٽ��Է�\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId) // str->int ��ȯ
					data_idx = i;
			}
			
			printSingleData(userInfo, &data_idx); // �˻� �ڷ� ���
			continueAct(&input);
			break;

		case '2':
			printf("ã���� �̸�: ");
			gets(data);
			while (isNotValidChar(data)){ // �̸� ��ȿ�� üũ
				printf("�ٽ��Է�\n�̸�: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data,userInfo[i].userName)==0)
					data_idx = i;
			}

			printSingleData(userInfo, &data_idx); // �˻� �ڷ� ���
			continueAct(&input);
			break;
			
		case '3':
			printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n��ȭ��ȣ: ");
			printf("ã���� ��ȭ��ȣ: ");
			gets(data);
			while (isNotValidNum(data)){ // ��ȭ��ȣ ��ȿ�� üũ
				printf("�ٽ��Է�\n��ȭ��ȣ: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone)==0)
					data_idx = i;
			}

			printSingleData(userInfo, &data_idx); // �˻� �ڷ� ���
			continueAct(&input);
			break;
			
		case '0':
			break;
			
		default:
			printf("enter 1 to 4\n");
		}
		
	}
}

// ȸ������: 
int saveData(UserInfo *userInfo, int user_count, int *chk_exit)
{
	FILE *saveFile=fopen("data.txt", "wt");
	int i;
	
	if (saveFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}

	system("cls");
	printf("ȸ������\n");

	fprintf(saveFile, "ȸ�� ���̵�	ȸ�� �̸�	ȸ�� �ּ�	�ڵ��� ��ȣ\n"); //Index ����
	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId!=0)
			fprintf(saveFile, "%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	fclose(saveFile);

	bf_update_save = 0;
	printf("���� �Ǿ����ϴ�\n");
	
	if (chk_exit){
		printf("�����Ϸ��� �ƹ�Ű�� ��������\n");
		_getch();
		return 0;
	}
	
	printf("���θ޴��� ���ư����� �ƹ�Ű�� ��������\n");
	_getch();
	
	return 0;
}

// ȸ������(ȸ������ �� ����� ����): �˻� �������� ���� �ϳ��� ���
void printSingleData(UserInfo *userInfo, int *data_idx)
{
	if (*data_idx == 0) // ã�� �����Ͱ� ���� ���
		printf("ã�� ������ ����\n");

	/*searchData2 �Լ����� �ٷ� ���θ޴��� ���� ��쿡��
	�� �Լ��� ������ �ʱ� ������ data_idx==-1�� ���� Ȯ���� �ʿ� ����*/

	else // �˻� �ڷ� ���
		printf("%d\t%s\t%s\t%s\n", userInfo[*data_idx].userId, userInfo[*data_idx].userName,
		userInfo[*data_idx].userAddress, userInfo[*data_idx].userPhone);
}

// ���� ū ID�� Ȯ��
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

/* �ߺ� ������ Ȯ��
1) �ߺ� ������ -1 return
2) �ߺ� ����� �ߺ��Ǵ� ���� �������� index�� ��ȯ
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

// ���� �� ��������� ������� �ʾ��� �� ���忩�� Ȯ��
void checkSave(UserInfo *userInfo, int user_count)
{
	char input;
	int chk_exit = 1;

	printf("%d���� ������� ���� ������ �ֽ��ϴ�.\n�����Ͻð� �����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n", bf_update_save);
	input = _getch();

	switch (input){
	case '1':
		//Q. �̷� ��쵵 call(main-checkSave)->call(checkSave-saveData)�� ���� ������ ������
		saveData(userInfo, user_count, &chk_exit);
		break;
	case '2':
		break;
	default:
		printf("����� �Է�\n");
	}
}

/* �۾��� ��� �������� Ȯ��
��� �Լ����� input=='0' �� �� ���θ޴��� �̵�
*/
void continueAct(char *input)
{
	char continue_input;

	printf("�� �Ͻðڽ��ϱ�? 1.�� 2.���θ޴�\n");
	continue_input = _getch();

	if (continue_input == '1')
		return;
	else if (continue_input == '2'){
		*input = '0';
	}
	else
		printf("1�̳� 2������\n");

}

/* ID ��ȿ�� üũ:
1. 6�ڸ����� Ȯ��
2. ��� �Է°��� �������� Ȯ��
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

/* �̸�, �ּ� ��ȿ�� üũ:
1. NULL���� �ƴ��� Ȯ��
2. �� ù��° ĭ ���� Ȯ��
+) ��0���� ���� �̸��� ���ڰ� ���� üũ�� ���� �ʾҽ��ϴ�.(��ȣ��)
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

/* ��ȭ��ȣ ��ȿ�� üũ
1. '-'���� 13�ڸ� Ȯ��
2. 000-0000-0000 �� ���� Ȯ�� 
	2.1) 4, 9��° '-'
	2.2) ������ �ڸ� ����
*/
int isNotValidNum(char *data)
{
	int i;
	int len = strlen(data);
	
	if (len != 13) // 1. '-'���� 13�ڸ� Ȯ��
		return 1;

	for (i = 0; i < len; i++){ // 2. 000 - 0000 - 0000 �� ���� Ȯ��

		if (i == 3 || i == 8){ // 2.1) 4, 9��° '-'
			if (*data != 45)
				return 1;
		}

		else{ // 2.2) ������ �ڸ� ����
			if (*data<48 || *data>57)
				return 1;
		}
		data++;
	}

	return 0;

}
