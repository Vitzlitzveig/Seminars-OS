#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <unistd.h>

char flag0[] = {225, 70, 16, 150, 111, 45, 155, 72, 125, 82, 2, 127, 7, 37, 215, 189, 116, 200, 197, 177, 51, 1, 223, 21, 252, 145, 80, 178, 154, 116, 15, 143, 5, 62, 127, 157};
char flag1[] = "FLAG{Cat says meow}";
char flag2[] = {227, 68, 18, 148, 109, 37, 216, 66, 124, 80, 21, 118, 3, 98, 192, 179, 51, 142, 193, 166, 44, 5, 192, 20, 237, 209, 91};
char flag3[] = {226, 69, 19, 149, 108, 35, 221, 67, 112, 8, 64, 107, 15, 38, 132, 184, 50, 128, 208, 178, 55, 14, 142, 13};
char flag4[] = {229, 66, 20, 146, 107, 55, 215, 77, 120, 86, 19, 112, 5, 100, 207, 168, 38, 142, 215, 244, 33, 30, 205, 22, 242, 133, 93};

char passphrase[] = {249, 111, 52, 152, 113, 1, 221, 114, 119, 54, 12, 94, 15, 41, 198, 166, 58};

void decode_flag(int n);
int main(int argc, char **argv)
{
	int dbg = ptrace(PTRACE_TRACEME,0,0,0);
	if (dbg == -1)
	{
		printf("Debug someone else, dude\n");
		return 1;
	}

	if (argc == 5)
	{
		decode_flag(0);
		printf("%s\n", flag0);
		return 0;
	}

	if (argc == 2)
	{
		int num;
		int n = sscanf(argv[1], "%d", &num);
		if (n != 1) return -1;
		switch (num) {
			case 0: execl("/bin/echo", "echo", "You",    NULL); break;
			case 1: execl("/bin/echo", "echo", "can't",  NULL); break;
			case 2: execl("/bin/echo", "echo", "get",    NULL); break;
			case 3: execl("/bin/echo", "echo", "this",   NULL); break;
			case 4: execl("/bin/echo", "echo", "little", NULL); break;
			case 5: execl("/bin/echo", "echo", "flag",   NULL); break;
			default:
				execl("echo", "echo", "wrong cmd", NULL);
				break;
		}

		decode_flag(2);
		printf("%s\n", flag2);
	}

	decode_flag(10);
	decode_flag(3);
	for (int i = 2; i > 0; --i)
	{
		const int buff_size = 255;
		char buffer[buff_size];
		volatile int auto_auth = 0;
		bzero(buffer, buff_size);

		printf("Enter passphrase: ");
		scanf("%s", buffer);
		if (auto_auth || strcmp(buffer, passphrase) == 0)
		{
			decode_flag(4);
			printf("%s\n", flag4);
			return  0;
		}
		else
		{
			char answer[buff_size];
			snprintf(answer, buff_size, "Passphrase %s is incorrect!\n%%d attempts remain\n", buffer);
			printf(answer, i, flag3);
		}
	}

	return -1;
}



