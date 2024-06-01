#include <stdio.h>
#include <dirent.h>
#include <malloc.h>
#include <getopt.h>
#include <sys/stat.h>
#include <windows.h>

#define MAX_LINE_LENGTH 1000

struct Config {
    char *name;
    char *email;
    int numberOfAlias;
    char **alias;
};

void firstRunInitialize() {
    char homeDirectoryPath[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", homeDirectoryPath, MAX_PATH)) {
        // Create a path to the file
        char filePath[MAX_PATH];
        sprintf(filePath, "%s\\neogit-config", homeDirectoryPath);


        DIR *configDirectory = opendir(filePath);
        if (!configDirectory) {

            mkdir(filePath);

            FILE *fptr;
            sprintf(filePath, "%s\\global.config", filePath);
            fptr = fopen(filePath, "w");

            fprintf(fptr, "%s", "user.name=\"\"\nuser.email=\"\"\nalias.count=\"0\"");

            fclose(fptr);

        }
    }


}


char *getWordInStringBetweenDoubleQuotes(char text[]) {
    char *str = text;
    int start = strchr(str, '"') - str + 1;
    int end = strrchr(str, '"') - str;

    int length = end - start;
    char *word = malloc(length * sizeof(char) + 1);

    if (word == NULL)
        return NULL;

    for (int i = start; i < end; i++) {
        word[i - start] = str[i];
    }

    word[length] = '\0';

    return word;
}

struct Config *readConfigFile(char *filepath) {
    // Open Config File
    FILE *fptr = fopen(filepath, "r");

    // Check For Errors
    if (fptr == NULL) {
        printf("Error opening File !\n");
    }

    // Create A Buffer For Reading From File
    char line[MAX_LINE_LENGTH];

    // Get Name From File
    fgets(line, MAX_LINE_LENGTH, fptr);
    char *name = getWordInStringBetweenDoubleQuotes(line);

    // Get Email From File
    fgets(line, MAX_LINE_LENGTH, fptr);
    char *email = getWordInStringBetweenDoubleQuotes(line);

    // Get Alias Count
    fgets(line, MAX_LINE_LENGTH, fptr);
    char *aliasCountChar = getWordInStringBetweenDoubleQuotes(line);
    int aliasCount = atoi(aliasCountChar);

    // Get Alias
    char *alias[aliasCount];
    int index = 0;
    while (fgets(line, sizeof(line), fptr) != NULL) {
        alias[index] = strdup(line);
        if (alias[index][strlen(alias[index]) - 1] == '\n') {
            alias[index][strlen(alias[index]) - 1] = '\0';
        }
        index++;
    }

    // Create Instance Of Config
    struct Config *cnf = malloc(sizeof(struct Config));

    cnf->name = malloc(sizeof(char) * strlen(name));
    cnf->email = malloc(sizeof(char) * strlen(email));

    // Populate Name & Email
    memcpy(cnf->name, name, sizeof(char) * strlen(name));
    cnf->name[strlen(name)] = '\0';

    memcpy(cnf->email, email, sizeof(char) * strlen(email));
    cnf->email[strlen(email)] = '\0';

    cnf->numberOfAlias = aliasCount;

    // Allocating Memory For Storing Alias
    cnf->alias = (char **) malloc(sizeof(char *) * aliasCount);

    // Allocating Memory For Each Alias And Populating it
    for (int i = 0; i < aliasCount; ++i) {

        cnf->alias[i] = malloc(sizeof(char) * strlen(alias[i]));
        strcpy(cnf->alias[i], alias[i]);

    }

    fclose(fptr);

    free(name);
    free(email);

    return cnf;
}

void writeToConfigFile(char *filepath, char *name, char *email, char **alias, int numberOfNewAlias) // NEW ALIAS
{
    // Read and modify
    struct Config *cnf = readConfigFile(filepath);
    if (strlen(name) > 0)
        strcpy(cnf->name, name);
    if (strlen(email) > 0)
        strcpy(cnf->email, email);
    if (numberOfNewAlias > 0) {
        int newAliasLength = numberOfNewAlias + cnf->numberOfAlias;

        cnf->alias = realloc(cnf->alias, newAliasLength * sizeof(char *));

        for (int i = cnf->numberOfAlias; i < newAliasLength; ++i) {
            int valueLength = strlen(alias[i - cnf->numberOfAlias]);

            cnf->alias[i] = malloc(sizeof(char) * valueLength);
            strcpy(cnf->alias[i], alias[i - cnf->numberOfAlias]);
        }

        cnf->numberOfAlias = newAliasLength;
    }


    //Delete File
    unlink(filepath);

    // Recreate File
    FILE *fptr = fopen(filepath, "w");

    //print name
    fprintf(fptr, "user.name=\"%s\"\n", cnf->name);
    //print email
    fprintf(fptr, "user.email=\"%s\"\n", cnf->email);
    //print number of alias
    fprintf(fptr, "alias.count=\"%d\"\n", cnf->numberOfAlias);

    //print alias
    for (int i = 0; i < cnf->numberOfAlias; ++i) {
        fprintf(fptr, "%s", cnf->alias[i]);
        if (i < cnf->numberOfAlias - 1)
            fprintf(fptr, "%s", "\n");
    }

    fclose(fptr);

    // FREE STUFF no i mean free the memory
    free(cnf);
}

struct option long_options[] = {
        {"help",     no_argument, NULL, 'h'},
        {"global\0", no_argument, NULL, 'g'},
};

int directory_exists(const char *path) {
    struct stat stat_buf;
    int result = stat(path, &stat_buf);

    if (result == 0) {
        if (S_ISDIR(stat_buf.st_mode)) {
            return 1;
        } else {
            return 2;
        }
    } else {
        return 0;
    }
}

int check_parent_directories_for_folder(const char *directory_path, const char *folder_name) {
    if (directory_exists(folder_name))
        return 1;
    char path[254];
    strcpy(path, directory_path);
    while (strlen(path) >= 3) {
        char tempPath[254];

        int last_slash_pos = strrchr(path, '\\') - path;
        if (last_slash_pos == -1) {
            sprintf(path, "%s", path);
        }

        char *new_string = malloc(last_slash_pos + 1);
        strncpy(new_string, path, last_slash_pos);
        new_string[last_slash_pos] = '\0';

        sprintf(path, "%s", new_string);
        sprintf(tempPath, "%s\\%s", new_string, folder_name);
        if (directory_exists(tempPath))
            return 1;
    }
    return 0;
}

const char *optstring = "-hgf:::";

void Init(char cwd[]) {
    mkdir(cwd);
    int attributes = GetFileAttributes(cwd);
    attributes |= FILE_ATTRIBUTE_HIDDEN;
    SetFileAttributes(cwd, attributes);

    char *filepath;
    sprintf(filepath, "%s\\staged", cwd);

    FILE *file = fopen(filepath, "w");
    fclose(file);
}

unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }
    return hash;
}

void stageFileOrDirectory(char *path) {

    if (directory_exists(path) == 1) {
        // its a directory
        // read all the file names and folders and recall this method
        // while() { stageFileOrDirectory() }
        DIR *dir;
        struct dirent *ent;

        dir = opendir(path);

        if (dir != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                    continue;
                printf("%s\n", ent->d_name);
                char res[50];
                sprintf(res, "%s\\%s", path, ent->d_name);
                stageFileOrDirectory(res);
            }
            closedir(dir);
        } else {
            perror("opendir");
            return;
        }

        return;
    }


    char *filepath = ".neogit\\staged";
    char line[MAX_LINE_LENGTH];

    FILE *file = fopen(filepath, "r+");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        if (strcmp(line, path) == 0) {
            fgets(line, sizeof(line), file);
            FILE *stagingFile = fopen(path, "rb+");
            fseek(stagingFile, 0, SEEK_END);
            long size = ftell(stagingFile);
            fseek(stagingFile, 0, SEEK_SET);
            char *fcontent = malloc(size);
            fread(fcontent, 1, size, stagingFile);
            fclose(stagingFile);

            fcontent[size] = '\0';

            int hshvlu = hash(fcontent);
            char hashValue[100];
            sprintf(hashValue, "%d", hshvlu);

            if (strcmp(line, hashValue) == 0) {
                printf("This File/Folder Is already staged");
                fclose(file);
                return;
            } else {
                // FILE IS CHANGED
                printf("This File/Folder Staged Again After Changes");
                int offset = -strlen(line);
                fseek(file, offset, SEEK_CUR);
                fputs(hashValue, file);
                fclose(file);
                return;
            }
        }
    }

    if (directory_exists(path) == 2) // its a file
    {
        FILE *stagingFile = fopen(path, "rb");
        fseek(stagingFile, 0, SEEK_END);
        long size = ftell(stagingFile);
        fseek(stagingFile, 0, SEEK_SET);
        char *fcontent = malloc(size);
        fread(fcontent, 1, size, stagingFile);
        fclose(stagingFile);

        int hashedContent = 0;
        if (size > 0)
            hashedContent = hash(fcontent);

        file = fopen(filepath, "a");
        fprintf(file, "%s\n%d\n", path, hashedContent);
        fclose(file);

        printf("\nFile Staged +%s", path);
    }
}

void unStageFileOrDirectory(char *path) {

    if (directory_exists(path) == 1) {
        DIR *dir;
        struct dirent *ent;

        dir = opendir(path);

        if (dir != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                    continue;
                printf("%s\n", ent->d_name);
                char res[50];
                sprintf(res, "%s\\%s", path, ent->d_name);
                unStageFileOrDirectory(res);
            }
            closedir(dir);
        } else {
            perror("opendir");
            return;
        }

        return;
    }

    char *filepath = ".neogit\\staged";
    char line[MAX_LINE_LENGTH];

    FILE *file = fopen(filepath, "r+");
    FILE* temp_file = fopen("temp.txt", "w");
    int line_count = 0;

    char* toCheckPath;

    int isFound=0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            sprintf(toCheckPath, "%s", line);
            toCheckPath[strlen(line) - 1] = '\0';
        }
        if (strcmp(toCheckPath, path) == 0) {
            line_count++;
            isFound = 1;
            continue;
        }
        if(isFound == 1)
        {
            isFound = 0;
            continue;
        }
        fputs(line, temp_file);
        line_count++;
    }
    fclose(file);
    fclose(temp_file);

    remove(filepath);
    rename("temp.txt", filepath);
}

void Status(char path[254]) {
    DIR *dir;
    struct dirent *ent;

    dir = opendir(path);

    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;

            char res[254];
            sprintf(res, "%s\\%s", path, ent->d_name);

            if(directory_exists(res) == 1){
                Status(res);
                continue;
            }

            char X = '-';
            char Y = ' ';

            char *filepath = ".neogit\\staged";
            char line[MAX_LINE_LENGTH];

            FILE *file = fopen(filepath, "r");
            while (fgets(line, sizeof(line), file) != NULL) {
                if (line[strlen(line) - 1] == '\n') {
                    line[strlen(line) - 1] = '\0';
                }
                if (strcmp(line, ent->d_name) == 0) {
                    X = '+';

                    fgets(line, sizeof(line), file);
                    FILE *stagingFile = fopen(res, "rb+");

                    if(stagingFile == NULL)
                    {
                        Y = 'D';
                        fclose(file);
                        break;
                    }

                    fseek(stagingFile, 0, SEEK_END);
                    long size = ftell(stagingFile);
                    fseek(stagingFile, 0, SEEK_SET);
                    char *fcontent = malloc(size);
                    fread(fcontent, 1, size, stagingFile);
                    fclose(stagingFile);

                    fcontent[size] = '\0';
                    int hshvlu = hash(fcontent);
                    char hashValue[100];
                    sprintf(hashValue, "%d", hshvlu);

                    if (strcmp(line, hashValue) == 0) {
                        Y = 'A';
                        fclose(file);
                        break;
                    } else {
                        // FILE IS CHANGED
                        Y = 'M';
                        fclose(file);
                        break;
                    }
                }
            }

            printf("%c %c %s\n", X, Y, ent->d_name);
        }
        closedir(dir);
    } else {
        perror("opendir");
        return;
    }
}


int main(int argc, char *argv[]) {
    firstRunInitialize();
    getopt_long(argc, argv, optstring, long_options, NULL);
    if (optarg == NULL) {
        printf("%s", "neogit\n\tinit\n\tconfig\t[-h, --help]\n\t\t[-g, --global]");
        return 0;
    }
    if (strcmp(optarg, "config") == 0) {
        int c;
        int isGlobal = 0;

        while ((c = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
            switch (c) {
                case 'h':
                    printf("%s", "neogit\n\tconfig\t[-h, --help]\n\t\t[-g, --global]");
                    break;
                case 'g':
                    isGlobal = 1;
                    if (argc <= 4) {
                        printf("%s", "Invalid Use !! run \"neogit -h\" for help\n");
                        return -1;
                    }
                    continue;
                case ':':
                    printf("%s", "Invalid Use !! run \"neogit -h\" for help\n");
                    break;
                case '?':
                    printf("%s", "Invalid Use !! run \"neogit -h\" for help\n");
                    break;
                default:
                    break;
            }

            char *configFilePath = ".neogit/local.config";
            if (isGlobal == 1) {
                configFilePath = "neogit-config/global.config";
            }

            if (strcmp(optarg, "user.name") == 0) {
                int argumentLength = strlen(optarg);
                char *result = malloc(argumentLength * sizeof(char));

                getopt_long(argc, argv, optstring, long_options, NULL);
                strcpy(result, optarg);

                if (argc > 4)
                    for (int i = 5; i < argc; ++i) {
                        getopt_long(argc, argv, optstring, long_options, NULL);

                        argumentLength += (strlen(optarg) + 1);
                        result = realloc(result, argumentLength * sizeof(char));

                        sprintf(result, "%s %s", result, optarg);
                    }


                writeToConfigFile(configFilePath, result, "", NULL, 0);


                printf("Name Updated To : %s\n", result);
                return 0;
            } else if (strcmp(optarg, "user.email") == 0) {
                int argumentLength = strlen(optarg);
                char *result = malloc(argumentLength * sizeof(char));

                getopt_long(argc, argv, optstring, long_options, NULL);
                strcpy(result, optarg);

                writeToConfigFile(configFilePath, "", result, NULL, 0);

                printf("Email Updated To : %s\n", result);
                return 0;
            } else if (strncmp(optarg, "alias.", 6) == 0) {
                printf("%s", "here for alias");
                char **alias = malloc(1 * sizeof(char *));

                int argumentLength = strlen(optarg) - 6; // alias. length
                int startIndex = strlen(optarg) - argumentLength;
                alias[0] = malloc(argumentLength * sizeof(char));
                sprintf(alias[0], "%s=\"", optarg + startIndex);

                getopt_long(argc, argv, optstring, long_options, NULL);
                sprintf(alias[0], "%s%s\"", alias[0], optarg);

                writeToConfigFile(configFilePath, "", "", alias, 1);
                return 0;
            }
        }
    }
    else if (strcmp(optarg, "init") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(1);
        }
        if (!check_parent_directories_for_folder(cwd, ".neogit")) {
            sprintf(cwd, "%s\\%s", cwd, ".neogit");
            Init(cwd);
        } else
            printf("%s", "neogit already exists !!");
    }
    else if (strcmp(optarg, "add") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(1);
        }
        if (!check_parent_directories_for_folder(cwd, ".neogit")) {
            printf("%s", "Please Use \"neogit init\" First!");
            return 0;
        }
        int c;
        int isMultipleFiles = 0;
        while ((c = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
            switch (c) {
                case 'h':
                    //printf("%s", "neogit\n\tconfig\t[-h, --help]\n\t\t[-g, --global]");
                    break;
                case 'f':
                    isMultipleFiles = 1;
                    break;
                case ':':
                    printf("%s", "Invalid Use !! run \"neogit -h\" for help\n");
                    break;
                case '?':
                    printf("%s", "Invalid Use !! run \"neogit -h\" for help\n");
                    break;
                default:
                    break;
            }
            if(isMultipleFiles)
            {
                for (int i = 3; i < argc; ++i) {
                    stageFileOrDirectory(argv[i]);
                }
            } else{
                stageFileOrDirectory(optarg);
            }
            return 0;
        }

    }
    else if (strcmp(optarg, "rest") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(1);
        }
        if (!check_parent_directories_for_folder(cwd, ".neogit")) {
            printf("%s", "Please Use \"neogit init\" First!");
            return 0;
        }
        getopt_long(argc, argv, optstring, long_options, NULL);
        unStageFileOrDirectory(optarg);
    }
    else if(strcmp(optarg, "status") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(1);
        }
        if (!check_parent_directories_for_folder(cwd, ".neogit")) {
            printf("%s", "Please Use \"neogit init\" First!");
            return 0;
        }
        stageFileOrDirectory("CMakeCache.txt");
        Status(cwd);
    }
    return 0;
}
