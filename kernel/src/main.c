#include <kernel.h>

int strlen(char *str){
    int ret = 0;
    while (*str++)
        ret++;
    return ret;
}

int main(int argc, char **argv){
    //SystemTable->BootServices->ExitBootServices();
    EFI_TIME Time;
	RuntimeServices->GetTime(&Time, NULL);
    //SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello from kernel\n");
    return Time.Second;
}