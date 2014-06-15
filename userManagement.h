#ifndef __USER_MANAGEMENT_H__
#define __USER_MANAGEMENT_H__

typedef struct userinfo{

	int userId;
	char userName[50];
	char userAddress[100];
	char userPhone[20];

}UserInfo;

#define PRINT_NUM 10

#define YES '1'
#define NO '2'
#define QUIT 48

#define VIEW_USERINFO '1'
#define INSERT_USERINFO '2'
#define DELETE_USERINFO '3'
#define UPDATE_USERINFO '4'
#define SEARCH_USERINFO '5'
#define SAVE_USERINFO '6'

#define PREV_PAGE '1'
#define NEXT_PAGE '2'

#define SEARCH_ID '1'
#define SEARCH_NAME '2'
#define SEARCH_PHONE '3'

#define UPDATE_NAME '1'
#define UPDATE_ADDRESS '2'
#define UPDATE_PHONE '3'

int loadData(UserInfo *userInfo, FILE *fp);
void initData(UserInfo *userInfo);
void printAllData(UserInfo *userInfo, int user_count);
void insertData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num);
void searchMain(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, int menu);
void searchData(UserInfo *userInfo, int user_count, int *data_idx, int *search_num, char *continue_act);
void deleteData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act);
void updateData(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num, char *continue_act);
int saveData(UserInfo *userInfo, int user_count, int *chk_exit);

int selectData(UserInfo *userInfo, int *user_count, int *data_idx, char *continue_act, int *search_num, char menu[]);
void deleteUser(UserInfo *userInfo, int *select_idx);
void printSingleData(UserInfo *userInfo, int *data_idx, int *search_num);

void continueAct(char *continue_act, char *menu);

int checkBigId(UserInfo *userInfo, int *user_count);
void checkDuplicated(UserInfo *userInfo, int *user_count, int *data_idx, int *search_num);
void checkSave(UserInfo *userInfo, int user_count);
void checkSize(UserInfo *userInfo);

int isNotValidId(char *data);
int isNotValidName(char *data);
int isNotValidChar(char *data);
int isNotValidNum(char *data);

void warnInvaildInsert();
void warnInvaildInput(char *input);

void menu();
void printUp();

void clearPart();
void clearLine();
void clearSection(int x, int y, int line);
void gotoxy(int x, int y);

#endif