#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include "UserManagement.h"


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
	int data_idx[USER_COUNT] = { 0 };
	int search_num = 0;

	char input = 0;
	char continue_act;

	
	if (readFile == NULL){
		puts("data.txt file open error\n");
		return -1;
	}
	user_count=loadData(userInfo, readFile); // data.txt�� userInfo ����ü �迭�� �Է�, user_count ����
	
	while (input!=QUIT){

		menu();
		continue_act = 0;

		input = _getch();
		switch (input){
		case '1': // ȸ������
			printAllData(userInfo, user_count);
			break;
		case '2': // ȸ�����
			insertData(userInfo, &user_count, &new_count, data_idx, &search_num);
			break;
		case '3': // ȸ���˻� �� �˻� ȸ�� ����
			while (continue_act!=QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
				deleteData(userInfo, data_idx, &continue_act, &user_count, &search_num);
			}
			break;
		case '4': // ȸ���˻� �� �˻� ȸ�� ����
			while (continue_act != QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
				updateData(userInfo, data_idx, &continue_act, &search_num);
			}
			break;
		case '5': // ȸ���˻�
			while (continue_act != QUIT){
				searchData2(userInfo, user_count, data_idx, &search_num);
			}
			break;
		case '6': // ����
			saveData(userInfo, user_count, 0);
			break;
		case QUIT: // ����
			if (bf_update_save > 0)
				checkSave(userInfo, user_count);
			gotoxy(6, 22); printf("�߰���. . . ");
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

	gotoxy(32, 6); puts("1. ȸ �� �� ��");
	gotoxy(32, 8); puts("2. ȸ �� �� ��");
	gotoxy(32, 10); puts("3. ȸ �� �� ��");
	gotoxy(32, 12); puts("4. ȸ �� �� ��");
	gotoxy(32, 14); puts("5. ȸ �� �� ��");
	gotoxy(32, 16); puts("6. ��       ��");
	gotoxy(32, 18); puts("0. ��       ��");

	gotoxy(6, 21); puts("��������������������������������������������������������������������");
	gotoxy(6, 22); puts("���Ͻô� �޴��� ���ڸ� �Է��� �ּ���");
	gotoxy(6, 23); puts("��������������������������������������������������������������������");
	

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

void printUp()
{
	system("cls");

	gotoxy(6, 0); puts("��������������������������������������������������������������������");
	gotoxy(6, 1); puts("��                     ȸ �� �� �� �� �� �� ��                    ��");
	gotoxy(6, 2); puts("��                                                                ��");
	gotoxy(6, 3); puts("��                                                                ��");
	gotoxy(6, 4); puts("��������������������������������������������������������������������");

	gotoxy(6, 21); puts("��������������������������������������������������������������������");
	gotoxy(6, 23); puts("��������������������������������������������������������������������");
}

//ȸ������
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
	gotoxy(55, 3); puts("��ü ȸ�� ����");
	gotoxy(6, 6); puts("   I D     �� ��\t �ּ�\t\t\t\t ��ȭ��ȣ");
	gotoxy(6, 7); puts("��������������������������������������������������������������������");

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
			//gotoxy(6, 21); puts("��������������������������������������������������������������������");
			gotoxy(6, 22); puts("1: ���� ������ 2: ���� ������ 0: ���θ޴�                         ");
			//gotoxy(6, 23); puts("��������������������������������������������������������������������");

			while (1){
				input = _getch();
				if (input != '1' && input != '2' && input != QUIT){
					gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
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
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
		
			}
		}
	}
	return;
}

// ȸ����� 
void insertData(UserInfo *userInfo, int *user_count, int *new_count, int *data_idx, int *search_num)
{
	char input = 0;
	char input_chk_dup = 0;

	int biggerId;
	
	while (input != '0'){
		printUp();
		gotoxy(55, 3); puts("�ű� ȸ�� ���");
		gotoxy(25, 6); printf("�߰��Ͻ� �����͸� �Է��ϼ���\n");

		biggerId = checkBigId(userInfo, user_count); // ���� ū ID�� Ȯ��
		userInfo[*user_count].userId = biggerId + 1;
		gotoxy(25, 8); printf(" I        D : %d", userInfo[*user_count].userId);
		gotoxy(25, 9); printf("��       �� : ");
		gotoxy(25, 10); printf("��       �� : ");
		gotoxy(25, 11); printf("�� ȭ �� ȣ : ");

		gotoxy(6, 22); printf("1-4���� �ѱ� �̸��� �Է��� �ּ���");
		gotoxy(39, 9); gets(userInfo[*user_count].userName);
		while (isNotValidName(userInfo[*user_count].userName)){ // �̸� ��ȿ�� üũ
			gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
			gotoxy(39, 9); printf("                                   ");
			gotoxy(39, 9); gets(userInfo[*user_count].userName);
		}

		gotoxy(6, 22); printf("                                          ");
		gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // �ּ� ��ȿ�� üũ
			gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
			gotoxy(39, 10); printf("                                   ");
			gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		}

		gotoxy(6, 22); printf("000-0000-0000�� �������� '-'���� �Է����ּ���");
		gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // ��ȭ��ȣ ��ȿ�� üũ
			gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
			gotoxy(39, 11); printf("                                   ");
			gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		}

		checkDuplicated(userInfo, user_count, data_idx, search_num); // �ߺ� ȸ�� üũ, ����� �ߺ�ȸ���� data_idx�� ����

		if (*search_num == 0){ // �ߺ�ȸ���� ���� ���

			gotoxy(55, 22); printf("��� �Ǿ����ϴ�\n");
			(*user_count)++;
			(*new_count)++;
			bf_update_save++;

			continueAct(&input);
		}

		else{// �ߺ�ȸ���� ���� ���

			gotoxy(6, 22); gotoxy(6, 22); printf("                                                      "); 
			gotoxy(6, 22); printf("���� �ߺ� ȸ���� �ֽ��ϴ�. �׷��� ����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n");

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
				gotoxy(55, 22); printf("�߸��� �Է��Դϴ�\n");
			}
		}

	}
}

// ȸ���˻�(ȸ������ �� ����� ����): �˻� �������� ��ġ�� data_idx�� ����
void searchData2(UserInfo *userInfo, int user_count, int *data_idx, int *search_num)
{
	char data[100];
	char input = 0;

	int i;

	*search_num = 0; // �ʱ�ȭ: �����ָ� ���� �˻��� data_idx�� �̹� �����Ǿ� ����

	printUp();
	gotoxy(55, 3); puts("ȸ�� ���� ����");
	gotoxy(25, 6); printf("� ������ �˻� �Ͻðڽ��ϱ�?\n");

	gotoxy(6, 22); puts("1.ID 2.�̸� 3.��ȭ��ȣ 0.���θ޴�");

	//input = _getch();
	/*if (input == QUIT)
		*search_num = -1;*/

	while (input != QUIT){

		input = _getch();

		switch (input){
		case '1':
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("6�ڸ� ������ ID�� �Է����ּ���");
			gotoxy(25, 9); printf(" I        D : ");
			gotoxy(39, 9); gets(data);
			while (isNotValidId(data)){ // ID ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (atoi(data) == userInfo[i].userId){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			input = QUIT;
			break;

		case '2':
			gotoxy(6, 22); printf("1-4���� �ѱ� �̸��� �Է��� �ּ���");
			gotoxy(25, 9); printf("��       �� : "); gets(data);
			while (isNotValidChar(data)){ // �̸� ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (strcmp(data, userInfo[i].userName) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			input = QUIT;
			break;

		case '3':
			gotoxy(6, 22); printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n");
			gotoxy(25, 9); printf("�� ȭ �� ȣ : ");
			gets(data);
			while (isNotValidNum(data)){ // ��ȭ��ȣ ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(data);
			}

			for (i = 0; i < user_count; i++){ // data_idx ����
				if (strcmp(data, userInfo[i].userPhone) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			input = QUIT;
			break;

		case QUIT:
			*search_num = -1;
			break;

		default:
			gotoxy(55, 22); printf("�߸��� �Է��Դϴ�\n");
		}

	}
}

// ȸ������: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void deleteData(UserInfo *userInfo, int *data_idx, char *continue_act, int *user_count, int *search_num)
{
	char input=0;

	if (*search_num == -1){// �˻� ���� ���θ޴��� ���� ���
		*continue_act = QUIT;
		return;
	}
	gotoxy(55, 3); puts("ȸ�� ���� ����");

	if (*search_num == 0){ // ã�� �����Ͱ� ���� ���
		gotoxy(6, 22); printf("���� ������ �ش��ϴ� ȸ���� �������� �ʽ��ϴ�\n");
		continueAct(continue_act);
		return;
	}
	
	gotoxy(55, 22); printf("                    ");
	gotoxy(6, 22); printf("���� ȸ���� ���� �Ͻðڽ��ϱ�? 1.���� 0.���θ޴�\n");

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
		gotoxy(55, 22); printf("�߸��� �Է��Դϴ�\n");
		break;
	}
}

//ȸ�� ����: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void updateData(UserInfo *userInfo, int *data_idx, char *continue_act, int *search_num)
{

	char input = 0;

	gotoxy(55, 3); puts("ȸ�� ���� ����");

	if (*search_num == -1) // �˻� ���� ���θ޴��� ���� ���
		return;

	else if (*search_num == 0){ // ã�� �����Ͱ� ���� ���
		gotoxy(6, 22); printf("���� ������ �ش��ϴ� ȸ���� �������� �ʽ��ϴ�\n");
		continueAct(continue_act);
		return;
	}

	while (*continue_act != QUIT){

		gotoxy(25, 6); printf("� ������ ���� �Ͻðڽ��ϱ�?\n");
		gotoxy(25, 9); printf("                                                    ");
		gotoxy(6, 22); printf("                                                    ");
		gotoxy(6, 22); puts("1.�̸� 2.�ּ� 3.��ȭ��ȣ 0.���θ޴�");

		input = _getch();

		switch(input){
		case '1':
			gotoxy(25, 9); printf("��       �� : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("�����Ͻ� 1-4���� �ѱ� �̸��� �Է��� �ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // �Է¹��� �̸� ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			}
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act);
			break;

		case '2':
			gotoxy(25, 9); printf("��       �� : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("�����Ͻ� �ּҸ� �Է��� �ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // �Է¹��� �ּ� ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
				gotoxy(39, 9); printf("                                   ");
				gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			}
			bf_update_save++;
			continueAct(continue_act);
			break;

		case '3':
			gotoxy(25, 9); printf("�� ȭ �� ȣ : ");
			gotoxy(6, 22); printf("                                             ");
			gotoxy(6, 22); printf("�����Ͻ� ��ȭ��ȣ�� 000-0000-0000�� �������� '-'���� �Է����ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // �Է¹��� ��ȭ��ȣ ��ȿ�� üũ
				gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
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
			gotoxy(55, 22); printf("�߸��� �Է��Դϴ�\n");
		}
	}
	
}

//ȸ���˻�
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
		printf("ȸ���˻�\n");
		
		printf("� ����?\n");

		puts("1.ID");
		puts("2.�̸�");
		puts("3.��ȭ��ȣ");
		puts("4.���θ޴�");

		input = _getch();

		if (input == '4')
			input = QUIT;
		
		switch (input){
		case ID:
			printf("ID�� 6�ڸ� ���ڷ� �Է����ּ���\n");
			printf("ã���� ID: ");
			gets(data);
			while (isNotValidId(data)){ // ID ��ȿ�� üũ
				printf("�ٽ��Է�\nID: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (atoi(data) == userInfo[i].userId){ // str->int ��ȯ
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}
			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			continueAct(&input);
			break;

		case NAME:
			printf("ã���� �̸�: ");
			gets(data);
			while (isNotValidChar(data)){ // �̸� ��ȿ�� üũ
				printf("�ٽ��Է�\n�̸�: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userName) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			continueAct(&input);
			break;
			
		case PHONE:
			printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n��ȭ��ȣ: ");
			printf("ã���� ��ȭ��ȣ: ");
			gets(data);
			while (isNotValidNum(data)){ // ��ȭ��ȣ ��ȿ�� üũ
				printf("�ٽ��Է�\n��ȭ��ȣ: ");
				gets(data);
			}

			for (i = 0; i < user_count; i++){
				if (strcmp(data, userInfo[i].userPhone) == 0){
					data_idx[*search_num] = i;
					(*search_num)++;
				}
			}

			printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
			continueAct(&input);
			break;
			
		case QUIT:
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

	printUp();
	gotoxy(55, 3); puts("ȸ�� ���� ����");

	fprintf(saveFile, "ȸ�� ���̵�	ȸ�� �̸�	ȸ�� �ּ�	�ڵ��� ��ȣ\n"); //Index ����
	for (i = 0; i < user_count; i++){
		if (userInfo[i].userId!=0)
			fprintf(saveFile, "%d\t%s\t%s\t%s\n",
				userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
	}

	fclose(saveFile);

	bf_update_save = 0;
	gotoxy(18, 9); printf("��� ȸ�� ������ data.txt�� ���� �Ǿ����ϴ�\n");
	
	if (chk_exit){
		gotoxy(55, 3); puts("  ���α׷� ����");
		gotoxy(6, 22); printf("�����Ϸ��� �ƹ�Ű�� ��������\n");
		_getch();
		return 0;
	}
	
	gotoxy(6, 22); printf("���θ޴��� ���ư����� �ƹ�Ű�� ��������\n");
	_getch();
	
	return 0;
}

// ȸ������(ȸ������ �� ����� ����): �˻� �������� ���� �ϳ��� ���
void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num)
{
	int i;
	if (*search_num == 0){ // ã�� �����Ͱ� ���� ���
		gotoxy(17, 13); printf("���� ������ �ش��ϴ� ȸ���� �������� �ʽ��ϴ�");
	}
	/*searchData2 �Լ����� �ٷ� ���θ޴��� ���� ��쿡��
	�� �Լ��� ������ �ʱ� ������ data_idx==-1�� ���� Ȯ���� �ʿ� ����*/

	else{ // �˻� �ڷ� ���

		for (i = 0; i < *search_num; i++){
			gotoxy(6, 13); puts("   I D     �� ��\t �ּ�\t\t\t\t ��ȭ��ȣ");
			gotoxy(6, 14+i); printf("  %d\t%s\t%s\t\t%s\n", userInfo[*data_idx].userId, userInfo[*data_idx].userName,
			userInfo[*data_idx].userAddress, userInfo[*data_idx].userPhone);

			data_idx++;
		}
	}
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
1) �ߺ� ������ return
2) �ߺ� ����� �ߺ��Ǵ� ���� �������� index�� ��ȯ
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

// ���� �� ��������� ������� �ʾ��� �� ���忩�� Ȯ��
void checkSave(UserInfo *userInfo, int user_count)
{
	char input;
	int chk_exit = 1;

	printUp();
	gotoxy(58, 3); printf("Before Save");
	gotoxy(20, 9); printf("%d���� ������� ���� ������ �ֽ��ϴ�.", bf_update_save);
	gotoxy(6, 22); printf("�����Ͻð� �����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ� ");
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

	gotoxy(6, 22); printf("                                                                    "); 
	gotoxy(6, 22); printf("�� �Ͻðڽ��ϱ�? 1.�� 0.���θ޴�\n");
	
	continue_input = _getch();
	
	switch(continue_input) {
		case '1':
			return;
		case QUIT:
			*input = QUIT;
			break;
		default:
			gotoxy(55, 22); printf("�߸��� �Է��Դϴ�.\n");
	}

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

/* �̸� ��ȿ�� üũ:
1. �̸� ���� Ȯ��(1~4��)
2. �� ù��° ĭ ���� Ȯ��
+) ��0���� ���� �̸��� ���ڰ� ���� üũ�� ���� �ʾҽ��ϴ�.(��ȣ��)
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

/* �ּ� ��ȿ�� üũ:
1. NULL���� �ƴ��� Ȯ��
2. �� ù��° ĭ ���� Ȯ��
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

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
