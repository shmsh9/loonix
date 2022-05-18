#include <stdlib.h>
#include <stdio.h>

int main(int argc, CHAR16 **argv){
	CHAR16 *prompt = L"sh33w4x $> ";
	size_t lprompt = strlen(prompt);
	CHAR16 *buffercmd = calloc(256, sizeof(CHAR16));
	size_t positioncmd = 0;
	print(prompt);
	while(1){
		EFI_INPUT_KEY k = getchar();
		switch(k.ScanCode){
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
			/*SCANCODE*/
			case 0x0 :
				break;
			/*CTRL+C*/
			case 0x03:
				puts(L"^C");
				break;
			/*CTRL+D*/
			case 0x04:
				break;
			/*BACKSPACE*/
			case 0x08:
				break;
			/*TAB*/
			case 0x09:
				break;
			/*RETURN*/
			case 0x0d:
				print(L"\n");
				print(prompt);
				//printf(L"\n%s", prompt);
				break;
			default  :
				putchar(k.UnicodeChar);
				break;
		}
	}
}
