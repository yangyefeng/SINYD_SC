#include "TRWData.h"
#include <stdlib.h>
#include <string.h>

CTRWData::CTRWData()
{
}
CTRWData::~CTRWData()
{
}

void CTRWData::_DeleteFile(char *filename)
{

    FILE *stream;
    stream = fopen(filename, "a");
    if ( NULL != stream ) {
		fclose(stream);
		remove(filename);
		return;
    }
	fclose(stream);
}

bool CTRWData::_CreateFile(char *filename, char *wstr)
{
	FILE *stream;
    stream = fopen(filename, "r");
	if (NULL != stream) {
		//printf("%s :Exist!\n", filename);
		fclose(stream);
		return false;
	}
	
	stream = fopen(filename, "w");
	if (NULL != wstr) {
		//fprintf(stream, wstr);
	}
	fclose(stream);
	return true;
}

//清空文件内容
void CTRWData::_ClearFileContent(char *filename)
{
    char temp[1024]={0};
    sprintf(temp,"echo ''  > %s",filename);
    system(temp);
}

//将信息追加写入日志
void CTRWData::_InsertLog(char *logString,char *filePath)
{
    char cmd[1024]={0};
    sprintf(cmd,"echo '%s' >> %s ",logString,filePath);
    system(cmd);
}

//写配置文件
bool CTRWData::_WriteProfileString(char lpAppName[], char lpKeyName[], char lpString[], char lpFileName[])
{
	FILE *stream;
	void *pVoid = NULL; //文件的后半部分
	bool bFindKey = false;
	bool bFindSection = false;
	char line[LINESIZE];
	size_t sectionLength, keyLength, lineLength, nBytesRead = 0;
	long nInsertPos = -1, nCopyPos = -1, nFileEndPos, nPos; //文件指针位置
	long nSectionBegin = -1, nKeyBegin = -1, nNextKey = -1, nNextSection = -1;
	int mode = 0;
	//如果 sectionName 为NULL，返回成功
	if (lpAppName == NULL)
		return true;
	//r+: Opens for both reading and writing. (The file must exist.)
	stream = fopen(lpFileName, "r+");
	if (stream == NULL)
	{
		printf("stream is null");
		return false;
	}
	//先取一次mode的默认值
	if (lpKeyName == NULL)
		mode = MODE_DELETE_SECTION;
	else if (lpString == NULL)
		mode = MODE_DELETE_KEY;
	else
		mode = MODE_OVERWRITE_KEY;
	sectionLength = strlen(lpAppName);
	//每次读行前，保存文件指针位置
	while (nPos = ftell(stream), fgets(line, LINESIZE, stream) != NULL)
	{
		//忽略注释行和空行
		if (line[0] == 0 || line[0] == ';') continue;
		lineLength = strlen(line);
		//注意：把LF(0xa)字符替换成0，这在UNICODE环境下可能出现结尾是LF)
		if (line[lineLength - 1] == 0x0a)
		{
			line[lineLength - 1] = 0;
			lineLength--;
			//注意此时可能会成为空字符串
			if (lineLength == 0) continue;
		}
		//尝试寻找到 section
		if (!bFindSection)
		{
			if (line[0] != '[') continue; //本行是否是 [section]
										  //这里是我们想要的Section吗?
										  //检查这一行的宽度是否正好是section长度加2， [lpAppName]
			if (line[sectionLength + 1] != ']') continue;
			if (strncmp(line + 1, lpAppName, sectionLength) != 0) continue;
			//Now Section will appear on next line
			//读取section前求出 Key 的长度
			if (lpKeyName != NULL)
				keyLength = strlen(lpKeyName);
			nSectionBegin = nPos;
			bFindSection = true;
			continue;
		}
		//Section找到了，
		//Section End ?
		if (line[0] == '[')
		{
			nNextSection = nPos;
			break; //遇到了下一个
		}
		//是否需要查找KEY?
		if (lpKeyName != NULL)
		{
			if (lineLength < keyLength + 1 || line[keyLength] != '=') continue; //"KeyName="
			if (strncmp(line, lpKeyName, keyLength) != 0) continue;
			//Now We Get the Key!
			nKeyBegin = nPos;
			nNextKey = ftell(stream); //要拷贝的起始位置
									  //Now It's done.
			bFindKey = true;
			break;
		}
	}
	//如果已经到达文件尾部，则追加换行
	//	if(feof(stream))
	//		_ftprintf(stream, _T("\n"));
	if (nNextSection < 0) nNextSection = ftell(stream);
	if (nNextKey < 0) nNextKey = ftell(stream);
	//遍历后再次更新mode值
	if (mode == MODE_DELETE_SECTION)
	{
		if (!bFindSection)
		{
			fclose(stream);
			return true;
		}
		else
		{
			nInsertPos = nSectionBegin;
			nCopyPos = nNextSection;
		}
	}
	if (mode == MODE_DELETE_KEY)
	{
		if (!bFindKey)
		{
			fclose(stream);
			return true;
		}
		else
		{
			nInsertPos = nKeyBegin;
			nCopyPos = nNextKey;
		}
	}
	if (mode == MODE_OVERWRITE_KEY)
	{
		if (!bFindSection)
		{
			mode = MODE_APPEND_SECTION;
		}
		else
		{
			if (bFindKey)
			{
				nInsertPos = nKeyBegin;
				nCopyPos = nNextKey;
			}
			else
			{
				mode = MODE_APPEND_KEY;
				nInsertPos = nNextSection;
				nCopyPos = nNextSection;
			}
		}
	}
	//追加一个新的Section
	if (mode == MODE_APPEND_SECTION)
	{
		fprintf(stream, "\n[%s]\n%s=%s\n", lpAppName, lpKeyName, lpString);
		fclose(stream);
		return true;
	}
	//先把文件的后半部分拷贝到内存
	fseek(stream, 0, SEEK_END);
	nFileEndPos = ftell(stream);
	if (nCopyPos >= 0 && nCopyPos < nFileEndPos)
	{
		//分配内存作为缓冲区
		pVoid = malloc(nFileEndPos - nCopyPos + 1);
		if (pVoid == NULL)
		{
			fclose(stream);
			printf("memory is not enough");
			return false; //堆内存不足
		}
		fseek(stream, nCopyPos, SEEK_SET);
		nBytesRead = fread(pVoid, 1, nFileEndPos - nCopyPos + 1, stream);
	}
	//写入新的value值
	fseek(stream, nInsertPos, SEEK_SET);
	if (lpKeyName != NULL && lpString != NULL)
        fprintf(stream, "%s=%s\n", lpKeyName, lpString);
	//现在把文件的后半部分写回文件中
	if (pVoid != NULL && nBytesRead > 0)
	{
		fwrite(pVoid, 1, nBytesRead, stream);
		free(pVoid);
	}
	//此时结尾可能还有一些内容，属于原来的ini文件
	//我们把它写成注释
	nPos = ftell(stream);
	fclose(stream);
	//如果文件变小了，那么我们需要更改文件大小
	return true;
}

//从配置文件中获取String值
int CTRWData::_GetProfileString(const char lpAppName[], const char lpKeyName[], const char lpDefault[], char lpReturnedString[], const char lpFileName[])
{
    char szTemp[LINESIZE] = "";
    int ret = 0;
    FILE *stream;
    bool bFindVal = false;
    bool bFindSection = false;
    char line[LINESIZE];
    size_t sectionLength, keyLength, lineLength;
    stream = fopen(lpFileName, "r");
    if ( NULL == stream)
    {
        //设置默认值
        strcpy(lpReturnedString, lpDefault);
        ret = strlen(lpReturnedString);
        return ret;
    }
    sectionLength = strlen(lpAppName);
    while ( NULL != fgets(line, LINESIZE, stream) )
    {
        //忽略注释行和空行
        if( 0 == line[0] || ';' == line[0] ) continue;
        lineLength = strlen(line);
        //注意：把LF(0xa)字符替换成0，这在UNICODE环境下可能出现结尾是LF)
        if ( 0x0a == line[lineLength - 1] )
        {
            line[lineLength - 1] = 0;
            lineLength--;
            //注意此时可能会成为空字符串
            if ( 0 == lineLength ) continue;
        }
        //尝试寻找到 section
        if ( !bFindSection ) 
        {
            if(line[0] != '[') continue; //本行是否是 [section]
            //这里是我们想要的Section吗?
            //检查这一行的宽度是否正好是section长度加2， [lpAppName]
            if ( ']' != line[sectionLength + 1] ) continue;
            if ( 0 != strncmp(line+1, lpAppName, sectionLength) ) continue;
            //Now Section will appear on next line
            //读取section前求出 Key 的长度
            keyLength = strlen(lpKeyName);
            bFindSection = true;
            continue;
        }
        //查找Key, Section End?
        if ( '[' == line[0] ) break; //遇到了下一个
        if ( lineLength < keyLength+1 || '=' != line[keyLength] ) continue; //"KeyName="
        if ( 0 != strncmp(line, lpKeyName, keyLength) ) continue;
        //Now We Get the Key!
        //      strcpy(lpReturnedString, line + keyLength + 1);
        strcpy(szTemp, line + keyLength + 1);
        //Now It's done.
        bFindVal = true;
        break;
    }
    fclose(stream);
    if ( !bFindVal )
    {
        //设置默认值
        strcpy(szTemp, lpDefault);
    }
    strcpy(lpReturnedString, remove_empty(szTemp));
    ret = strlen(lpReturnedString);
    //DebugInfoLog("ret : %d,lpReturnedString:%s\n",ret, lpReturnedString);
    return ret;

}

//从配置文件中获取Int值
int CTRWData::_GetProfileInt(const char lpAppName[], const char lpKeyName[], int nDefault, const char lpFileName[])
{
    int ret = nDefault;
    FILE *stream;
    bool bFindVal = false;
    bool bFindSection = false;
    char line[LINESIZE];
    size_t sectionLength, keyLength, lineLength;
    stream = fopen(lpFileName, "r");
    if ( NULL == stream )
    {
        //设置默认值
        return nDefault;
    }
    //DebugLog("====lpAppName=%s,lpKeyName=%s!!====\n",lpAppName,lpKeyName);
    sectionLength = strlen(lpAppName);
    while ( NULL != fgets(line, LINESIZE, stream) )
    {
        //忽略注释行和空行
        if( 0 == line[0] || ';' == line[0] )
            continue;

        lineLength = strlen(line);
        //注意：把LF(0xa)字符替换成0，这在UNICODE环境下可能出现结尾是LF)
        if ( 0x0a == line[lineLength - 1] )
        {
            line[lineLength - 1] = 0;
            lineLength--;
            //注意此时可能会成为空字符串
            if ( 0 == lineLength )
                continue;
        }
        //尝试寻找到 section
        if ( !bFindSection )
        {
            if ( '[' != line[0] )
                continue; //本行是否是 [section]
            //这里是我们想要的Section吗?
            //检查这一行的宽度是否正好是section长度加2， [lpAppName]
            if ( ']' != line[sectionLength + 1] )
                continue;
            if ( 0 != strncmp(line+1, lpAppName, sectionLength) )
                continue;
            //Now Section will appear on next line
            //读取section前求出 Key 的长度
            keyLength = strlen(lpKeyName);
            bFindSection = true;
            continue;
        }
        //查找Key, Section End?
        if ( '[' == line[0] ) break; //遇到了下一个
        if ( lineLength < keyLength+1 || '=' != line[keyLength] )
            continue; //"KeyName="
        if ( 0 != strncmp(line, lpKeyName, keyLength) )
            continue;
        //Now We Get the Key!
        char *pStopChar = NULL;
        ret = strtol(remove_empty(line + keyLength + 1), &pStopChar, 10); //默认为10进制
        //Now It's done.
        bFindVal = true;
        break;
    }
    fclose(stream);
    return ret;
}

bool CTRWData::_WriteProfileInt(char lpAppName[], char lpKeyName[], int value, char lpFileName[])
{
	FILE *stream;
	void *pVoid = NULL; //文件的后半部分
	bool bFindKey = false;
	bool bFindSection = false;
	char line[LINESIZE];
	size_t sectionLength, keyLength, lineLength, nBytesRead = 0;
	long nInsertPos = -1, nCopyPos = -1, nFileEndPos, nPos; //文件指针位置
	long nSectionBegin = -1, nKeyBegin = -1, nNextKey = -1, nNextSection = -1;
	int mode = 0;
	//如果 sectionName 为NULL，返回成功
	if (lpAppName == NULL)
		return true;
	//r+: Opens for both reading and writing. (The file must exist.)
	stream = fopen(lpFileName, "r+");
	if (stream == NULL)
	{
		printf("stream is null");
		return false;
	}
	//先取一次mode的默认值
	if (lpKeyName == NULL)
		mode = MODE_DELETE_SECTION;
	else
		mode = MODE_OVERWRITE_KEY;
	sectionLength = strlen(lpAppName);
	//每次读行前，保存文件指针位置
	while (nPos = ftell(stream), fgets(line, LINESIZE, stream) != NULL)
	{
		//忽略注释行和空行
		if (line[0] == 0 || line[0] == ';') continue;
		lineLength = strlen(line);
		//注意：把LF(0xa)字符替换成0，这在UNICODE环境下可能出现结尾是LF)
		if (line[lineLength - 1] == 0x0a)
		{
			line[lineLength - 1] = 0;
			lineLength--;
			//注意此时可能会成为空字符串
			if (lineLength == 0) continue;
		}
		//尝试寻找到 section
		if (!bFindSection)
		{
			if (line[0] != '[') continue; //本行是否是 [section]
										  //这里是我们想要的Section吗?
										  //检查这一行的宽度是否正好是section长度加2， [lpAppName]
			if (line[sectionLength + 1] != ']') continue;
			if (strncmp(line + 1, lpAppName, sectionLength) != 0) continue;
			//Now Section will appear on next line
			//读取section前求出 Key 的长度
			if (lpKeyName != NULL)
				keyLength = strlen(lpKeyName);
			nSectionBegin = nPos;
			bFindSection = true;
			continue;
		}
		//Section找到了，
		//Section End ?
		if (line[0] == '[')
		{
			nNextSection = nPos;
			break; //遇到了下一个
		}
		//是否需要查找KEY?
		if (lpKeyName != NULL)
		{
			if (lineLength < keyLength + 1 || line[keyLength] != '=') continue; //"KeyName="
			if (strncmp(line, lpKeyName, keyLength) != 0) continue;
			//Now We Get the Key!
			nKeyBegin = nPos;
			nNextKey = ftell(stream); //要拷贝的起始位置
									  //Now It's done.
			bFindKey = true;
			break;
		}
	}
	//如果已经到达文件尾部，则追加换行
	//	if(feof(stream))
	//		_ftprintf(stream, _T("\n"));
	if (nNextSection < 0) nNextSection = ftell(stream);
	if (nNextKey < 0) nNextKey = ftell(stream);
	//遍历后再次更新mode值
	if (mode == MODE_DELETE_SECTION)
	{
		if (!bFindSection)
		{
			fclose(stream);
			return true;
		}
		else
		{
			nInsertPos = nSectionBegin;
			nCopyPos = nNextSection;
		}
	}
	if (mode == MODE_DELETE_KEY)
	{
		if (!bFindKey)
		{
			fclose(stream);
			return true;
		}
		else
		{
			nInsertPos = nKeyBegin;
			nCopyPos = nNextKey;
		}
	}
	if (mode == MODE_OVERWRITE_KEY)
	{
		if (!bFindSection)
		{
			mode = MODE_APPEND_SECTION;
		}
		else
		{
			if (bFindKey)
			{
				nInsertPos = nKeyBegin;
				nCopyPos = nNextKey;
			}
			else
			{
				mode = MODE_APPEND_KEY;
				nInsertPos = nNextSection;
				nCopyPos = nNextSection;
			}
		}
	}
	//追加一个新的Section
	if (mode == MODE_APPEND_SECTION)
	{
		fprintf(stream, "\n[%s]\n%s=%d\n", lpAppName, lpKeyName, value);
		fclose(stream);
		return true;
	}
	//先把文件的后半部分拷贝到内存
	fseek(stream, 0, SEEK_END);
	nFileEndPos = ftell(stream);
	if (nCopyPos >= 0 && nCopyPos < nFileEndPos)
	{
		//分配内存作为缓冲区
		pVoid = malloc(nFileEndPos - nCopyPos + 1);
		if (pVoid == NULL)
		{
			fclose(stream);
			printf("memory is not enough");
			return false; //堆内存不足
		}
		fseek(stream, nCopyPos, SEEK_SET);
		nBytesRead = fread(pVoid, 1, nFileEndPos - nCopyPos + 1, stream);
	}
	//写入新的value值
	fseek(stream, nInsertPos, SEEK_SET);
	if (lpKeyName != NULL)
		fprintf(stream, "%s=%d\n", lpKeyName, value);
	//现在把文件的后半部分写回文件中
	if (pVoid != NULL && nBytesRead > 0)
	{
		fwrite(pVoid, 1, nBytesRead, stream);
		free(pVoid);
	}
	//此时结尾可能还有一些内容，属于原来的ini文件
	//我们把它写成注释
	nPos = ftell(stream);
	fclose(stream);
	//如果文件变小了，那么我们需要更改文件大小
	return true;
}

//移除空格
char* CTRWData::remove_empty (char *p)
{
    char * ptmp = p;
    while (*ptmp == ' ')
    {
        ptmp ++;
    }
    while(*(ptmp + strlen(ptmp) - 1) == ' ')
    {
        *(ptmp + strlen(ptmp) - 1) = '\0';
    }

    return ptmp;
}

void CTRWData::_WriteDebugForLog(char logstr[], char FileName[])
{
    FILE *stream;
    stream = fopen(FileName, "a");
    if ( NULL == stream ) {
        printf("文件不存在 \n");
    } 
    fprintf(stream, "%s\n", logstr);
    fclose(stream);
}

