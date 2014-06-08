#ifndef __USER_MANAGEMENT_H__
#define __USER_MANAGEMENT_H__

typedef struct userinfo{

	int userId;
	char userName[50];
	char userAddress[100];
	char userPhone[20];

}UserInfo;

#define USER_COUNT 50
#define PRINT_NUM 10

#define QUIT 48
#define ID 49
#define NAME 50
#define PHONE 51

#define LEFT 75
#define RIGHT 77

void menu();

int loadData(UserInfo *userInfo, FILE *fp);
void printAllData(UserInfo *userInfo, int user_count);
void insertData(UserInfo *userInfo, int *user_count, int *new_count, int *data_idx, int *search_num);
void searchData2(UserInfo *userInfo, int user_count, int *data_idx, int *search_num);
void deleteData(UserInfo *userInfo, int *data_idx, char *continue_act, int *user_count, int *search_num);
void updateData(UserInfo *userInfo, int *data_idx, char *continue_act, int *search_num);
void searchData(UserInfo *userInfo, int user_count, int *data_idx, int *search_num);
int saveData(UserInfo *userInfo, int user_count, int *chk_exit);

void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num);
int checkBigId(UserInfo *userInfo, int *user_count);
void checkDuplicated(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num);
void checkSave(UserInfo *userInfo, int user_count);

void continueAct(char *input);

void gotoxy(int x, int y);
void printUp();

int isNotValidId(char *data);
int isNotValidName(char *data);
int isNotValidChar(char *data);
int isNotValidNum(char *data);

#endif