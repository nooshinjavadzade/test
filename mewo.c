#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
struct memoryy {
    char *data;
    size_t size_d;
};
static int callback(void *contents, int size, int nmemb, void *x) {
    int sizer = size * nmemb; 
    struct memoryy *mem = (struct memoryy *)x; 
    int new_size = mem->size_d + sizer + 1; 
    char *new_data = realloc(mem->data, new_size);
    if (new_data == NULL) {
        printf("Error in memory allocation\n");
        return 0;
    }
    memcpy(new_data + mem->size_d, contents, sizer);
    new_data[new_size - 1] = 0; 
    mem->data = new_data; 
    mem->size_d = new_size - 1; 
    return sizer; 
}

void print(const char *url, const char *query, const char *api) {
    snprintf(url, 512, "https://serpapi.com/search?q=%s&api_key=%s", query, api);
    CURL *curl;
    CURLcode result;
    struct memoryy chunk;

    chunk.data = malloc(1); 
    chunk.size_d = 0;           

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        result = curl_easy_perform(curl);
        if(result != CURLE_OK) {
            printf("ERROR!!!!");
        } else {
            cJSON *json = cJSON_Parse(chunk.data);
            if (json == NULL) {
                printf("Data parsing error\n");
                free(chunk.data);
                return;
        }
        cJSON *kham = cJSON_GetObjectItem(json, "kham_results");
            int count = cJSON_GetArraySize(kham);
            for (int i = 0; i < count; i++) {
                cJSON *r = cJSON_GetArrayItem(kham, i);
                cJSON *title = cJSON_GetObjectItem(r, "title");
                cJSON *link = cJSON_GetObjectItem(r, "link");
                if (title && link) {
                    printf("title: %s\n", title->valuestring);
                    printf("link: %s\n\n", link->valuestring);
                }
            }
        cJSON_Delete(json);
    }
    curl_easy_cleanup(curl);
}
    free(chunk.data);
    curl_global_cleanup();
}

int main() {
    const char *api = "d3ee3ef805c10f762022ea37e91f357a8e310150dfa576dbb4f14bcb53e872a5";
    char word[256];
    char url[512];
    printf("Search whatever you want: ");
    int size = 0;
    fgets(&word, &size, stdin);
    word[strcspn(word, "\n")] = 0; 
    print(url,word,api);
    return 0;
}
