#include <stdlib.h>
#include <stdio.h>
int main(int argc, CHAR16 **argv){
	CHAR16 prompt[] = L"sh33w4x $> ";
	
	//size_t lprompt = strlen(prompt);
	//CHAR16 *buffercmd = calloc(256, sizeof(CHAR16));
	//size_t positioncmd = 0;
	EFI_INPUT_KEY k = {0, 0};
	printf(prompt);
	while(1){
		k = getchar();
		switch(k.ScanCode){
			case 0x00:
				break;
			/*UP ARROW*/
			case 0x01:
				break;
			/*DOWN ARROW*/
			case 0x02:
				break;
			/*RIGHT ARROW*/
			case 0x03:
				break;
			/*LEFT ARROW*/
			case 0x04:
				break;
		}
		switch(k.UnicodeChar){
			case 0x0 :
				break;
			/*CTRL+C*/
			case 0x03:
				printf(L"^C\n%s",prompt);
				break;
			/*CTRL+D*/
			case 0x04:
				puts(L"^D");
				return 0;
				break;
			/*BACKSPACE*/
			case 0x08:
				break;
			/*TAB*/
			case 0x09:
				break;
			/*RETURN*/
			case 0x0d:
				printf(L"\n%s", prompt);
				break;
			default  :
				/*Only print ASCII (Printable)*/
				if(k.UnicodeChar > 0x1f && k.UnicodeChar < 0x7f)
					putchar(k.UnicodeChar);
				break;
		}
	}
}
