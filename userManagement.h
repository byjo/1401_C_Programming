#ifndef __USER_MANAGEMENT_H__
#define __USER_MANAGEMENT_H__

typedef struct userinfo{

	int userId;
	char userName[50];
	char userAddress[100];
	char userPhone[20];

}UserInfo;

void menu();
int loadData(UserInfo *userInfo, FILE *fp);
void printAllData(UserInfo *userInfo, int user_count);
void insertData(UserInfo *userInfo, int *user_count, int *new_count);
void searchData2(UserInfo *userInfo, int user_count, int *data_idx);
void printSingleData(UserInfo *userInfo, int *data_idx);
void deleteData(UserInfo *userInfo, int *data_idx);
void updateData(UserInfo *userInfo, int *data_idx);
void searchData(UserInfo *userInfo, int user_count);
int saveData();

int isNotValidNum(char *data);
int isNotValidChar(char *data);
int isNotValidId(char *data);

#endif