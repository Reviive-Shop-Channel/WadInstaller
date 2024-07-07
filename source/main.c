#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <curl/curl.h>

static u32 *xfb;
static GXRModeObj *rmode;


void Initialise() {
  
	VIDEO_Init();
	PAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}


int main() {
	CURL *curl;
    FILE *file;
    CURLcode res;
	Initialise();
	fatInitDefault();
    
    char *url = "https://raw.githubusercontent.com/OpenShopChannel/WSC-Patcher/main/.gitignore";
    char *destination = "sd:/.gitignore";

    curl = curl_easy_init();
    if (curl) {
        file = fopen(destination, "wb");
        if (file) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                printf("File downloaded successfully\n");
            }

            fclose(file);
        } else {
            fprintf(stderr, "Could not open file for writing\n");
        }

        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize curl\n");
    }
	
	printf("Hello World!\n");
	sleep(5);
	
	return 0;
}
