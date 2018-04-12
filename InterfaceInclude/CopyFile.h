/////////////////////////////////////////////////////////////////////////////
/// @File name: CopyFile.h
/// @Author: h
/// @Copyright Shenyang new generation of information technology Co., Ltd.
/// @Version: 1.0
/// @Date: 2016/03/18
/// @Description:
/// 	Config-File Copy
/// @History:
/// 	version: author, date, desc\n 
/////////////////////////////////////////////////////////////////////////////
#ifndef COPYFILE_H
#define COPYFILE_H


#include <stdio.h>
#include <string.h>
#include <vector>
#include "MeasureDef.h"

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a),0777)
#endif

std::string dirpath;

#ifdef _WIN32
void print_dir(char *root, char *secondpath, std::vector<std::string> *outlist,const char * ctype = NULL)
{
	_finddata_t fileDir;
	char dir[255] = {0};
	long lfDir;
	std::string path;
	std::string spath;
	strcpy(dir,root);
	strcat(dir, "*.*");
	printf("dir:%s\n",dir);

	if ((lfDir = _findfirst(dir, &fileDir)) == -1l)
		printf("No file is found\n");
	else {
		printf("file list:\n");
		do {
			if (strcmp(fileDir.name, ".") == 0 || strcmp(fileDir.name, "..") == 0)
				continue;
			if (fileDir.attrib & _A_SUBDIR) {
				spath = std::string(fileDir.name) + CONFIG_PATH_DELIMITER;
				path = std::string(root) + spath;
				printf("dir:%s : %s\n", fileDir.name, path.c_str());
				//print_dir((char*)path.c_str(), (char*)spath.c_str(), outlist);
				continue;
			}
			printf("%s\n", fileDir.name);
			outlist->push_back(std::string(secondpath) + std::string(fileDir.name));
		} while (_findnext(lfDir, &fileDir) == 0);
	}
	_findclose(lfDir);

}
#else
void print_dir (char *root, char *secondpath,std::vector<std::string> *outlist,const char * ctype = "cement")
{
    DIR *dir;
    struct dirent *ptr;
    dir = opendir (root);
    //char path[128] = {0};
    std::string path;
    std::string spath;

    if (dir == NULL) {
        perror ("fail to open dir");
        return;
    }
    errno = 0;
    while ( (ptr = readdir (dir)) != NULL) {
        if (strcmp (ptr->d_name, ".") == 0 || strcmp (ptr->d_name, "..") == 0)
            continue;
        if (ptr->d_type == DT_DIR ) {
            if (0 == strcmp(ptr->d_name, ctype)) {
                spath = std::string(ptr->d_name) + CONFIG_PATH_DELIMITER;
                path = std::string(root) + spath;
                //printf("dir:%s : %s\n",ptr->d_name, path.c_str());
                print_dir ((char*)path.c_str(), (char*)spath.c_str(), outlist);
            }
            continue;
        }
        //printf ("file: %s \n", ptr->d_name);
        outlist->push_back(std::string(secondpath) + std::string(ptr->d_name));
    }
    if (errno != 0) {
        perror ("fail to read dir");
        return;
    }
    closedir (dir);
}
#endif

int CopyFile(const char * srcfile, const char *dstfile)
{
    FILE *fpR, *fpW;
    char buffer[255];
    int lenR, lenW;
    if ((fpR = fopen(srcfile, "r")) == NULL)
    {
        printf("Open SrcFile Error! \n");
        return -1;
    }
    if ((fpW = fopen(dstfile, "w")) == NULL)
    {
        printf("Open DstFile Error! \n");
        fclose(fpR);
        return -1;
    }

    memset(buffer, 0, 255);
    while ((lenR = fread(buffer, 1, 255, fpR)) > 0)
    {
        if ( ( lenW = fwrite(buffer, 1, lenR, fpW) ) != lenR )
        {
            printf("Write to file failed!\n");
            fclose(fpR);
            fclose(fpW);
            return -1;
        }
        memset(buffer, 0, 255);
    }

    fclose(fpR);
    fclose(fpW);
    return 1;
}

const char * BackupConfig(std::string timestamp, int mac_type=1)
{
    std::string dstpath = CONFIG_BACKUP_PATH;
    dstpath += std::string(CONFIG_BACKUP_DIR);
    dstpath = dstpath + timestamp + CONFIG_PATH_DELIMITER;
    printf("dstPath:%s\n",dstpath.c_str());

    //Create Dir
    if ( 0 != ACCESS(CONFIG_BASE_PATH,0)) {
        printf("dstPath:%s\n",CONFIG_BASE_PATH);
        return NULL;
    }

    if ( 0 != ACCESS(dstpath.c_str(),0)) {
        if ( 0 != MKDIR(dstpath.c_str()) ) {
            return NULL;
        }
    }
    dirpath = dstpath;
    //return dstpath.c_str();

    //获取文件列表
    std::vector<std::string> filelist;
    //const char * ctype = (1 == mac_type )?"asphalt":((2 == mac_type)?"cement":"cement");
    const char * ctype = (1 == mac_type)?"asphalt":((2 == mac_type)?"cement":((3 == mac_type)?"sbs":((4 == mac_type)?"tank":"cement")));
    printf("type:%s\n",ctype);
    print_dir(CONFIG_BASE_PATH, "",&filelist, ctype);
    std::string dstfile;
    std::string srcfile;
    int pos;

    //CopyFile
    std::vector<std::string>::iterator it;
    for (it = filelist.begin(); it != filelist.end(); it++) {
        pos =(*it).find_last_of(CONFIG_PATH_DELIMITER);
        printf("allfile:%s , pos:%d\n",(*it).c_str(), pos);

        dstfile = dstpath + (*it).substr(pos+1);
        srcfile = CONFIG_BASE_PATH + *it;

        printf("src:%s\n",srcfile.c_str());
        printf("dst:%s\n",dstfile.c_str());

        CopyFile(srcfile.c_str(),dstfile.c_str());
    }
    printf("retpath: %s \n",dstpath.c_str());

    srcfile = CONFIG_RUNTIME_PATH;
    srcfile += "SC_TransBreakPoint.xml";
    dstfile = dstpath + "SC_TransBreakPoint.xml";
    printf("src:%s\n",srcfile.c_str());
    int i = CopyFile(srcfile.c_str(),dstfile.c_str());
    printf("@@@@Copyefile:%d[%s]\n",i,dstfile.c_str());

    srcfile = CONFIG_RUNTIME_PATH;
    srcfile += "SC_TransBreakPoint.db";
    dstfile = dstpath + "SC_TransBreakPoint.db";
    printf("src:%s\n",srcfile.c_str());
    i = CopyFile(srcfile.c_str(),dstfile.c_str());
    return dstpath.c_str();
}


#endif // COPYFILE_H
