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
int user_size = 50;

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
		case VIEW_USERINFO: // ȸ������
			printAllData(userInfo, user_count);
			break;
		case INSERT_USERINFO: // ȸ�����
			insertData(userInfo, &user_count, data_idx, &search_num);
			break;
		case DELETE_USERINFO: // ȸ���˻� �� �˻� ȸ�� ����
			searchMain(userInfo, &user_count, data_idx, &search_num, 1);
			break;
		case UPDATE_USERINFO: // ȸ���˻� �� �˻� ȸ�� ����
			searchMain(userInfo, &user_count, data_idx, &search_num, 2);
			break;
		case SEARCH_USERINFO: // ȸ���˻�
			searchMain(userInfo, &user_count, data_idx, &search_num, 3);
			break;
		case SAVE_USERINFO: // ����
			saveData(userInfo, user_count, 0);
			break;
		case QUIT: // ����
			checkSave(userInfo, user_count);
			free(userInfo);
			gotoxy(6, 22); printf("�߰���. . . ");
			system("exit");
			break;
		default:
			warnInvaildInsert();
		}
	}

	return 0;
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

// ������ �ʱ�ȭ: data.txt�� �ڷḦ userInfo ����ü�� ����
void initData(UserInfo *userInfo)
{
	FILE *readFile = fopen("data.txt", "rt");

	for (int i = 0; i < user_size; i++){ // data �ʱ�ȭ
		userInfo[i].userId = 0;
	}

	if (readFile == NULL){
		puts("data.txt file open error\n");
		return;
	}
	user_count = loadData(userInfo, readFile); // data.txt�� userInfo ����ü �迭�� �Է�, user_count ����

	fclose(readFile);
}

//ȸ������
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
	gotoxy(55, 3); puts("��ü ȸ�� ����");
	gotoxy(6, 6); puts("NO.   I D\t     �� ��\t\t �� ��\t\t     ��ȭ��ȣ");
	gotoxy(6, 7); puts("��������������������������������������������������������������������");

	while (1){
		gotoxy(6, 8 + i - (page - 1)*PRINT_NUM);
		if (userInfo[i].userId == 0) // ȸ�� ������ NULL �� ��
			clearLine();

		else // ȸ�� ������ ���� �� ��
			printf("%2d   %d  %8s\t%20s\t   %s",
				i+1,userInfo[i].userId, userInfo[i].userName, userInfo[i].userAddress, userInfo[i].userPhone);
		
		user_num++;
		i++;

		if (user_num > PRINT_NUM){

			user_num = 1;
			gotoxy(66, 20); printf("%d/%d page", page, total_page);
			gotoxy(6, 22); puts("1: ���� ������ 2: ���� ������ 0: ���θ޴�                         ");

			warnInvaildInput(&input);
			switch (input){
			case PREV_PAGE: // ���� ������
				clearSection(6,8,PRINT_NUM);
				if (page == 1){ // �� ù�� ° ������
					i = (page - 1) * PRINT_NUM;
					break;
				}
				else{
					i = (page - 2) * PRINT_NUM;
					page--;
				}
				break;

			case NEXT_PAGE: // ���� ������
				clearSection(6,8,PRINT_NUM);
				if (page == total_page){ //������ ������
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

// ȸ����� 
void insertData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num)
{
	char input = 0;
	char input_chk_dup = 0;
	
	int biggerId;

	while (input != QUIT){

		checkSize(userInfo);

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
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 9); gets(userInfo[*user_count].userName);
		}

		gotoxy(6, 22); clearLine();
		gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		while (isNotValidChar(userInfo[*user_count].userAddress)){ // �ּ� ��ȿ�� üũ
			warnInvaildInsert();
			gotoxy(39, 10); clearPart();
			gotoxy(39, 10); gets(userInfo[*user_count].userAddress);
		}

		gotoxy(6, 22); printf("000-0000-0000�� �������� '-'���� �Է����ּ���");
		gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		while (isNotValidNum(userInfo[*user_count].userPhone)){ // ��ȭ��ȣ ��ȿ�� üũ
			warnInvaildInsert();
			gotoxy(39, 11); clearPart();
			gotoxy(39, 11); gets(userInfo[*user_count].userPhone);
		}

		checkDuplicated(userInfo, user_count, data_idx, search_num); // �ߺ� ȸ�� üũ, ����� �ߺ�ȸ���� data_idx�� ����

		if (*search_num == 0){ // �ߺ�ȸ���� ���� ���

			gotoxy(30, 19); printf("��� �Ǿ����ϴ�!\n");
			(*user_count)++;
			bf_update_save++;

			continueAct(&input, "���");
		}

		else{// �ߺ�ȸ���� ���� ���

			printSingleData(userInfo, data_idx, search_num);
			gotoxy(6, 22); printf("�ߺ�ȸ���� �ִµ��� ����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n");

			warnInvaildInput(&input_chk_dup);
			switch (input_chk_dup){
			case YES: // �ߺ� ȸ�� ���
				(*user_count)++;
				bf_update_save++;
				gotoxy(30, 19); printf("��� �Ǿ����ϴ�!\n");
				continueAct(&input,"���");
				break;
			case NO: // �ߺ� ȸ�� ��� ����
				userInfo[*user_count].userId = 0;
				continueAct(&input,"���");
				break;
			default:
				warnInvaildInsert();
			}
		}
	}
}

// ȸ�� ����, ����, �˻��� ���� �˻� ����
void searchMain(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, int menu)
{
	char continue_act = 0;

	while (continue_act != QUIT){

		searchData(userInfo, *user_count, data_idx, search_num, &continue_act);

		if (continue_act == QUIT)
			break;

		if (menu == 1){ // ����
			deleteData(userInfo, user_count, data_idx, search_num, &continue_act);
			continueAct(&continue_act, "����");
		}

		else if (menu == 2){ // ����
			updateData(userInfo, user_count, data_idx, search_num, &continue_act);
			if (continue_act!=QUIT)
				continueAct(&continue_act, "����");
		}

		else if (menu == 3) // �˻�
			continueAct(&continue_act, "�˻�");
	}
}

// ȸ���˻�: �˻� �������� ��ġ�� data_idx�� ����
void searchData(UserInfo *userInfo, int user_count, int *data_idx, int *search_num, char *continue_act)
{
	char data[100];
	char input = 0;

	int i;

	*search_num = 0; // �ʱ�ȭ: �����ָ� ���� �˻��� search_num�� �̹� �����Ǿ� ����

	printUp();
	gotoxy(55, 3); puts("ȸ�� ���� �˻�");
	gotoxy(25, 6); printf("� ������ �˻� �Ͻðڽ��ϱ�?\n");
	gotoxy(6, 22); puts("1.ID 2.�̸� 3.��ȭ��ȣ 0.���θ޴�");

	warnInvaildInput(&input);
	switch (input){
	case SEARCH_ID: // ID �˻�
		gotoxy(6, 22); clearLine();
		gotoxy(25, 9); printf(" I        D : ");
		gotoxy(6, 22); printf("6�ڸ� ������ ID�� �Է����ּ���");
		gotoxy(39, 9); gets(data);
		while (isNotValidId(data)){ // ID ��ȿ�� üũ
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 9); gets(data);
		}

		for (i = 0; i < user_count; i++){ // data_idx ����
			if (atoi(data) == userInfo[i].userId){
				data_idx[*search_num] = i;
				(*search_num)++;
			}
		}

		printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
		break;

	case SEARCH_NAME: // �̸� �˻�
		gotoxy(6, 22); clearLine();
		gotoxy(25, 9); printf("��       �� : ");
		gotoxy(6, 22); printf("1-4���� �ѱ� �̸��� �Է��� �ּ���");
		gotoxy(39, 9); gets(data);
		while (isNotValidName(data)){ // �̸� ��ȿ�� üũ
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 9); gets(data);
		}

		for (i = 0; i < user_count; i++){ // data_idx ����
			if (strcmp(data, userInfo[i].userName) == 0){
				data_idx[*search_num] = i;
				(*search_num)++;
			}
		}

		printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
		break;

	case SEARCH_PHONE: // ��ȭ��ȣ �˻�
		gotoxy(6, 22); clearLine();
		gotoxy(25, 9); printf("�� ȭ �� ȣ : ");
		gotoxy(6, 22); printf("(000-0000-0000�� �������� '-'���� �Է����ּ���)\n");
		gotoxy(39, 9); gets(data);
		while (isNotValidNum(data)){ // ��ȭ��ȣ ��ȿ�� üũ
			warnInvaildInsert();
			gotoxy(39, 9); clearPart();
			gotoxy(39, 9); gets(data);
		}

		for (i = 0; i < user_count; i++){ // data_idx ����
			if (strcmp(data, userInfo[i].userPhone) == 0){
				data_idx[*search_num] = i;
				(*search_num)++;
			}
		}

		printSingleData(userInfo, data_idx, search_num); // �˻� �ڷ� ���
		break;

	case QUIT:
		*search_num = -1;
		*continue_act = QUIT;
		return;

	default:
		warnInvaildInsert();
	}
}

// ȸ������: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void deleteData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act)
{
	char input = 0;
	int select_idx;

	select_idx=selectData(userInfo, user_count, data_idx, continue_act, search_num, "����");
	if (*search_num == 0)
		return;
	
	gotoxy(6, 22); clearLine();
	gotoxy(6, 22); printf("���� ȸ���� ���� �Ͻðڽ��ϱ�? 1.���� 2.�ƴϿ�\n");

	warnInvaildInput(&input);

	switch (input){
	case YES:
		gotoxy(30, 19); printf("���� �Ǿ����ϴ�!\n");
		deleteUser(userInfo, &select_idx);
		bf_update_save++;
		(*user_count)--;
		break;

	case NO:
		return;

	default:
		warnInvaildInsert();
	}
}

//ȸ�� ����: ȸ�� �˻� �� data_idx�� ���� userInfo�� �ڷῡ ����
void updateData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act)
{
	char input=0;

	selectData(userInfo, user_count, data_idx, continue_act, search_num, "����");
	if (*search_num == 0)
		return;

	while (*continue_act != QUIT){

		if (*continue_act == NO)
			break;

		gotoxy(25, 6); printf("� ������ ���� �Ͻðڽ��ϱ�?\n");
		gotoxy(25, 9); clearLine();
		gotoxy(6, 22); clearLine();
		gotoxy(6, 22); puts("1.�̸� 2.�ּ� 3.��ȭ��ȣ 0.���θ޴�");

		warnInvaildInput(&input);
		switch(input){
		case UPDATE_NAME:
			gotoxy(25, 9); printf("��       �� : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("�����Ͻ� 1-4���� �ѱ� �̸��� �Է��� �ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			while (isNotValidChar(userInfo[*data_idx].userName)){ // �Է¹��� �̸� ��ȿ�� üũ
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userName);
			}
			gotoxy(30, 19); printf("���� �Ǿ����ϴ�!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"�� ����");
			break;

		case UPDATE_ADDRESS:
			gotoxy(25, 9); printf("��       �� : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("�����Ͻ� �ּҸ� �Է��� �ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			while (isNotValidChar(userInfo[*data_idx].userAddress)){ // �Է¹��� �ּ� ��ȿ�� üũ
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userAddress);
			}
			gotoxy(30, 19); printf("���� �Ǿ����ϴ�!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"�� ����");
			break;

		case UPDATE_PHONE:
			gotoxy(25, 9); printf("�� ȭ �� ȣ : ");
			gotoxy(6, 22); clearLine();
			gotoxy(6, 22); printf("�����Ͻ� ��ȭ��ȣ�� 000-0000-0000�� �������� '-'���� �Է����ּ���");
			gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			while (isNotValidNum(userInfo[*data_idx].userPhone)){ // �Է¹��� ��ȭ��ȣ ��ȿ�� üũ
				warnInvaildInsert();
				gotoxy(39, 9); clearPart();
				gotoxy(39, 9); gets(userInfo[*data_idx].userPhone);
			}
			gotoxy(30, 19); printf("���� �Ǿ����ϴ�!\n");
			bf_update_save++;
			printSingleData(userInfo, data_idx, search_num);
			continueAct(continue_act,"�� ����");
			break;

		case QUIT:
			*continue_act = QUIT;
			return;

		default:
			warnInvaildInsert();
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

// �˻� ���ǿ� �´� ȸ���� �� �� ���� ȸ�� ����
int selectData(UserInfo *userInfo, int *user_count, int *data_idx, char *continue_act, int *search_num, char menu[])
{
	int select;

	gotoxy(55, 3); printf("ȸ�� ���� %s", menu);

	if (*search_num == 0){ // ã�� �����Ͱ� ���� ���
		gotoxy(6, 22); printf("���� ������ �ش��ϴ� ȸ���� �������� �ʽ��ϴ�\n");
		return -1;
	}

	if (*search_num > 1){ // ã�� �����Ͱ� ������ �� ��� -> �� �� ����
		gotoxy(6, 22); printf("%s �Ͻ� ȸ���� NO.�� �Է��ϰ� enterŰ�� ���� �ּ���: ", menu);
		scanf("%d", &select);
		*search_num = 1;
		data_idx += (select - 1); // data_idx ����

		clearSection(6, 13, 8);
		printSingleData(userInfo, data_idx, search_num); // ���õ� �� �� �����
	}
	return *(data_idx); // ������ ȸ���� index �� return
}

// ȸ�� ������ �ڿ� �ִ� ȸ�� ������ �����
void deleteUser(UserInfo *userInfo, int *select_idx)
{
	int i;

	if (*select_idx == user_count - 1){ // �� ������ ȸ�� ������
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

// �˻�ȸ������: �˻� �������� ������ ���
void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num)
{
	int i;

	if (*search_num == 0){ // ã�� �����Ͱ� ���� ���
		gotoxy(17, 13); printf("���� ������ �ش��ϴ� ȸ���� �������� �ʽ��ϴ�");
	}

	else if (*search_num == -1)
		return;

	else{ // �˻� �ڷ� ���
		clearSection(6, 14, *search_num);
		gotoxy(6, 22); clearLine();
		for (i = 0; i < *search_num; i++){
			gotoxy(6, 13); puts("NO.   I D\t     �� ��\t\t �� ��\t\t     ��ȭ��ȣ");
			gotoxy(6, 14 + i); printf("%2d   %d  %8s\t%20s\t   %s\n", i + 1, userInfo[*(data_idx + i)].userId, userInfo[*(data_idx + i)].userName,
				userInfo[*(data_idx + i)].userAddress, userInfo[*(data_idx + i)].userPhone);
		}
	}
}

/* �۾��� ��� �������� Ȯ��
��� �Լ����� input==QUIT �� �� ���θ޴��� �̵�
*/
void continueAct(char *continue_act, char *menu)
{
	char continue_input;

	gotoxy(6, 22); clearLine();

	if (menu == "�� ����"){
		gotoxy(6, 22); printf("�� ȸ���� %s �Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n", menu);
	}
	else{
		gotoxy(6, 22); printf("�ٸ� ȸ���� %s �Ͻðڽ��ϱ�? 1.�� 0.���θ޴�\n", menu);
	}

	//continue_input = _getch();
	warnInvaildInput(&continue_input);
	switch (continue_input) {
	case YES: // ���
		*continue_act = 0;
		return;
	case NO: // �ƴϿ�
		*continue_act = NO;
		break;
	case QUIT: // ���θ޴�
		*continue_act = QUIT;
		break;
	default:
		warnInvaildInsert();
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

	if (bf_update_save == 0){ // ��������� ��� ���� or ��������� ���� ��
		gotoxy(64, 3); printf("Exit");
		gotoxy(25, 9); printf("��� ������ ����Ǿ� �ֽ��ϴ�.");
		return;
	}

	gotoxy(58, 3); printf("Before Exit");
	gotoxy(20, 9); printf("%d���� ������� ���� ������ �ֽ��ϴ�.", bf_update_save);
	gotoxy(6, 22); printf("�����Ͻð� �����Ͻðڽ��ϱ�? 1.�� 2.�ƴϿ�\n");
	
	warnInvaildInput(&input);
	switch (input){
	case YES: // ���� �� ����
		saveData(userInfo, user_count, &chk_exit);
		break;
	case NO: // �׳� ����
		break;
	default:
		warnInvaildInsert();
	}
}

/* userInfo �뷮 Ȯ��:
1. ���� ������ 5���� ���� ��
2. ���� �뷮�� 2��� ���� ���� �Ҵ�
3. ���� ������ �����
4. ���� �����Ͱ� ���� �κ��� �ʱ�ȭ
5. userInfo�� ��ü
; realloc�� ���� ����
*/
void checkSize(UserInfo *userInfo)
{
	UserInfo *newUserInfo;
	int i;

	if (user_count > user_size - 5){
		user_size *= 2;
		newUserInfo = (UserInfo *)malloc(sizeof(UserInfo)*user_size);

		for (i = 0; i<user_count; i++){
			newUserInfo[i].userId = userInfo[i].userId;
			strcpy(newUserInfo[i].userName, userInfo[i].userName);
			strcpy(newUserInfo[i].userAddress, userInfo[i].userAddress);
			strcpy(newUserInfo[i].userPhone, userInfo[i].userPhone);
		}

		for (i; i<user_size; i++){
			newUserInfo[i].userId = 0;
		}
		userInfo = newUserInfo;
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

// ������ �����ÿ� �߸��� �Է½� �ϴ� �ٿ� ���� �޼���
void warnInvaildInsert()
{
	gotoxy(55, 22); puts("�߸��� �Է��Դϴ�");
}

// ���������� �߸��� �Է½� �ϴ� �ٿ� ���� �޼���
void warnInvaildInput(char *input)
{
	while (1){
		*input = _getch();
		if (*input != '1' && *input != '2' && *input != '3' && *input != QUIT){
			warnInvaildInsert();
		}
		else
			break;
	}
}

// �޴��� ���
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

// ��� ���
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

// �κ� �����
void clearPart()
{
	printf("\t\t\t\t\t");
}

// 1�� �����
void clearLine()
{
	printf("\t\t\t\t\t\t\t\t\t\t");
}

// ���õ� x,y ��ǥ���� line ��ŭ �����
void clearSection(int x, int y, int line)
{
	int i;

	gotoxy(x, y); for (i = 0; i < line; i++)
		clearLine();
}

// x, y ��ǥ ���
void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}