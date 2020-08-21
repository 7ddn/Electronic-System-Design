#include "display.h"

void musicLock(){
	int nowPW = 0;
	//char str[2];

	while(nowPW < 5){
		//itoa(nowPW, str, 10);
		//Disp(4,0,1,str);
		
		if (nowPW == 0){
			Ini_Lcd();
			Disp(1,0,16,"请演奏以输入密码");
			Disp(2,0,9,"_ _ _ _ _");
		}
		OPT_CHECK = 0xFF;
	 	if (OPT_CHECK&0x01) {
				Playnote(0x01, 7, 0);
				if (password[nowPW] == 7) {
					Disp(2,nowPW,1,"7");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x02) {
				Playnote(0x02, 6, 0);
				if (password[nowPW] == 6) {
					Disp(2,nowPW,1,"6");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x04) {
				Playnote(0x04, 5, 0);
				if (password[nowPW] == 5) {
					Disp(2,nowPW,1,"5");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x08) {
				Playnote(0x08, 4, 0);
				if (password[nowPW] == 4) {
					Disp(2,nowPW,1,"4");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x10) {
				Playnote(0x10, 3, 0);
				if (password[nowPW] == 3) {
					Disp(2,nowPW,1,"3");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x20) {
				Playnote(0x20, 2, 0);
				if (password[nowPW] == 2) {
					Disp(2,nowPW,1,"2");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
			else if (OPT_CHECK&0x40) {
				Playnote(0x40, 1, 0);
				if (password[nowPW] == 1) {
					Disp(2,nowPW,1,"1");
					nowPW++;
					Disp(3,2,8,"密码无误");
				}
				else {
					nowPW = 0;
					Disp(3,2,8,"密码错误");
				}
			}
	}
}