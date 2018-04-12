#include "BitmapSaveThread_Cali.h"
#include "libpng/png.h"
#include "ConfigInitializer.h"
#include "CommonDef.h"
#include <imagehlp.h>
#include "SCPluginDef.h"
#include <time.h>
#include <shellapi.h>
#include "ScreenShotHolder.h"


#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "../Lib/zlib.lib")
#pragma comment(lib, "../Lib/libpng16.lib")

struct SaveTask_Cali : public IBitmapSaveTask_Cali
{
	SaveTask_Cali(const HBITMAP& hbitmap, const string& timeStamp)
		: m_bitmap()
		, m_timeStamp(timeStamp)
	{
		m_bitmap.Load(hbitmap);
	}
	virtual void Execute() override
	{
		string path = ".\\CalibrationInfo\\";

		string::iterator itB = m_timeStamp.begin();
		string::iterator itE = m_timeStamp.end();
		for (string::iterator it = itB; it != itE; ++it)
		{
			if (*it == ':')
			{
				*it = '_';
			}
			if (*it == ' ')
			{
				*it = '\\';
			}
		}

		path = path + m_timeStamp + ".png";
		::MakeSureDirectoryPathExists(path.c_str());
		
		save_png_to_file(&m_bitmap, path.c_str());

		FILE* file = fopen(".\\RemoteLog\\pnglist.log", "a");
		if (file)
		{
			string fileName = m_timeStamp + ".png\n";
			string::iterator itB = fileName.begin();
			string::iterator itE = fileName.end();
			for (string::iterator it = itB; it != itE; ++it)
			{
				if (*it == '\\')
				{
					*it = '/';
				}
			}
			fprintf(file, fileName.c_str());
			fclose(file);
			file = NULL;
		}
	}

private:

	/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
	success, non-zero on error. */
	void save_png_to_file(CBitmapEx *bitmap, const char *path)
	{
		FILE * fp;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		size_t x, y;
		png_byte ** row_pointers = NULL;
		/* "status" contains the return value of this function. At first
		it is set to a value which means 'failure'. When the routine
		has finished its work, it is set to a value which means
		'success'. */
		int status = -1;
		/* The following number is set by trial and error only. I cannot
		see where it it is documented in the libpng manual.
		*/
		int pixel_size = 3;
		int depth = 8;

		fp = fopen(path, "wb");
		if (fp) {
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (png_ptr) {
				info_ptr = png_create_info_struct(png_ptr);
				if (info_ptr) {
					/* Set up error handling. */
					if (setjmp(png_jmpbuf(png_ptr)))
					{
					}
					else
					{
						/* Set image attributes. */
						png_set_IHDR(png_ptr,
							info_ptr,
							bitmap->GetWidth(),
							bitmap->GetHeight(),
							depth,
							PNG_COLOR_TYPE_RGB,
							PNG_INTERLACE_NONE,
							PNG_COMPRESSION_TYPE_DEFAULT,
							PNG_FILTER_TYPE_DEFAULT);

						/* Initialize rows of PNG. */
						row_pointers = (png_byte **)png_malloc(png_ptr, bitmap->GetHeight() * sizeof(png_byte *));
						for (y = 0; y < bitmap->GetHeight(); ++y) {
							png_byte *row =
								(png_byte *)png_malloc(png_ptr, sizeof(uint8_t) * bitmap->GetWidth() * pixel_size);
							row_pointers[y] = row;
							for (x = 0; x < bitmap->GetWidth(); ++x) {
								DWORD pixel = bitmap->GetPixel(x, y);
								*row++ = _GetRValue(pixel);
								*row++ = _GetGValue(pixel);
								*row++ = _GetBValue(pixel);
							}
						}

						/* Write the image data to "fp". */
						png_init_io(png_ptr, fp);
						png_set_rows(png_ptr, info_ptr, row_pointers);
						png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
						/* The routine has successfully written the file, so we set
						"status" to a value which indicates success. */
						status = 0;

						for (y = 0; y < bitmap->GetHeight(); y++) {
							png_free(png_ptr, row_pointers[y]);
						}
						png_free(png_ptr, row_pointers);
					}
				}
				png_destroy_write_struct(&png_ptr, &info_ptr);
			}
			fclose(fp);
		}
	}

	string m_timeStamp;
	CBitmapEx m_bitmap;
};



/*通过调用ShFileOperation来实现整个目录的删除*/
/*只删除单个目录*/
BOOL SHDeleteFolderA_Cali(LPCSTR pstrFolder, BOOL bAllowUndo)
{
	if ((NULL == pstrFolder))
	{
		return FALSE;
	}

	int iPathLen = strlen(pstrFolder);
	if (iPathLen >= MAX_PATH)
	{
		return FALSE;
	}

	/*确保目录的路径以2个\0结尾*/
	char tczFolder[MAX_PATH + 1];
	memset(tczFolder, 0, (MAX_PATH + 1)*sizeof(char));
	strcpy_s(tczFolder, pstrFolder);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen + 1] = _T('\0');

	SHFILEOPSTRUCTA FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags |= FOF_SILENT;        /*不显示进度*/
	FileOp.fFlags |= FOF_NOERRORUI;        /*不报告错误信息*/
	FileOp.fFlags |= FOF_NOCONFIRMATION;/*直接删除，不进行确认*/
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = tczFolder;            /*要删除的目录，必须以2个\0结尾*/
	FileOp.pTo = NULL;

	/*根据传递的bAllowUndo参数确定是否删除到回收站*/
	if (bAllowUndo)
	{
		FileOp.fFlags |= FOF_ALLOWUNDO; /*删除到回收站*/
	}
	else
	{
		FileOp.fFlags &= ~FOF_ALLOWUNDO; /*直接删除，不放入回收站*/
	}

	/*删除目录*/
	if (0 == SHFileOperationA(&FileOp))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

struct DiskSpaceCheckStopTask : public IBitmapSaveTask_Cali
{
	DiskSpaceCheckStopTask(CBitmapSaveThread_Cali* pThread)
		: m_pSaveThread(pThread)
	{}
	virtual void Execute() override
	{
		//OutputDebugStringA("CheckStop Start");
		string path = ".\\CalibrationInfo\\";
		long maxSize = 1 * 1024 * 1024;		//1GB = 1024 * 1024 KB
		//long maxSize = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheUsedSpace() * 1024 * 1024;		//KB
		char cpathend = *(path.crbegin());
		if (cpathend != '\\')
		{
			path = path + '\\';
		}

		::MakeSureDirectoryPathExists(path.c_str());
		list<SubFolder> folders;
		long lSize = FindSubFolders(path, "*-*-*", "*_*_*", folders);
		//char log[256] = { 0 };
		//sprintf(log, "Check Stop size = %d", lSize);
		//OutputDebugStringA(log);
		if (lSize > maxSize)
		{
			//OutputDebugStringA("Check Stop Stop");
			ScreenShotHolderInstance::GetInstance()->SetIsSaveCaliPNG(false);
			if (m_pSaveThread)
			{
				m_pSaveThread->ClearTask();
			}
		}
	}

protected:
	struct SubFile
	{
		string name;
		long size;		//KB
	};
	struct SubFolder
	{
		string name;
		list<SubFile> files;
		long size;		//KB
	};

	long FindSubFolders(string parentPath, const char* pFolderWildcard, const char* pFileWildcard, list<SubFolder>& folders)
	{
		long lTotalSize = 0;
		char cpathend = *(parentPath.crbegin());
		if (cpathend != '\\')
		{
			parentPath = parentPath + '\\';
		}
		string findPath = parentPath + pFolderWildcard;

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(findPath.c_str(), &ffd);
		BOOL bFind = TRUE;
		while (hFind && hFind != INVALID_HANDLE_VALUE && bFind)
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				SubFolder folder;
				folder.name = ffd.cFileName;
				folder.size = FindSubFiles(parentPath + folder.name, pFileWildcard, folder.files);
				folders.push_back(folder);
				lTotalSize = lTotalSize + folder.size;
			}

			bFind = FindNextFileA(hFind, &ffd);
		}
		FindClose(hFind);
		return lTotalSize;
	}
	long FindSubFiles(string parentPath, const char* pWildcard, list<SubFile>& files)
	{
		long lTotalSize = 0;
		char cpathend = *(parentPath.crbegin());
		if (cpathend != '\\')
		{
			parentPath = parentPath + '\\';
		}
		parentPath = parentPath + pWildcard;

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(parentPath.c_str(), &ffd);
		BOOL bFind = TRUE;
		while (hFind && hFind != INVALID_HANDLE_VALUE && bFind)
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				SubFile file;
				file.name = ffd.cFileName;
				file.size = ((ffd.nFileSizeHigh << 8) + ffd.nFileSizeLow) / 1024;	//Byte to KB
				files.push_back(file);
				lTotalSize = lTotalSize + file.size;
			}

			bFind = FindNextFileA(hFind, &ffd);
		}
		FindClose(hFind);
		return lTotalSize;
	}

private:
	CBitmapSaveThread_Cali* m_pSaveThread;
};

CBitmapSaveThread_Cali::CBitmapSaveThread_Cali()
{
	m_hTaskEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_CleanTaskTimer.SetTimedEvent(this, &CBitmapSaveThread_Cali::CleanTimerFunc);
}


CBitmapSaveThread_Cali::~CBitmapSaveThread_Cali()
{
	if (m_hTaskEvent)
	{
		::CloseHandle(m_hTaskEvent);
		m_hTaskEvent = nullptr;
	}
}

DWORD CBitmapSaveThread_Cali::StartImpl(DWORD dwCreationFlags)
{
	DWORD dw = CThreadBase::StartImpl(dwCreationFlags);
// 	AddTimeCleanTask();
// 	AddDiskCleanTask();
	AddDiskCheckTask();
	m_CleanTaskTimer.Start(5 * 60 * 1000);
	return dw;
}

void CBitmapSaveThread_Cali::StopImpl(int nWaitTime)
{
	m_CleanTaskTimer.Stop();
	CThreadBase::StopImpl(nWaitTime);
}

void CBitmapSaveThread_Cali::AddTask(const HBITMAP & hbitmap, const string& time)
{
	shared_ptr<IBitmapSaveTask_Cali> spTask(new SaveTask_Cali(hbitmap, time));
	PushTask(spTask);
}

void CBitmapSaveThread_Cali::AddDiskCheckTask()
{
	shared_ptr<IBitmapSaveTask_Cali> spTask(new DiskSpaceCheckStopTask(this));
	PushTask(spTask);
}

void CBitmapSaveThread_Cali::ClearTask()
{
	list<shared_ptr<IBitmapSaveTask_Cali>> tmpTaskList;
	PopTask(tmpTaskList);
}

DWORD CBitmapSaveThread_Cali::Execute()
{
	constexpr DWORD dwHandleCount = 2;
	HANDLE handles[dwHandleCount] = { m_hStopEvent, m_hTaskEvent };
	while (true)
	{
		DWORD dw = ::WaitForMultipleObjects(dwHandleCount, handles, FALSE, INFINITE);
		if (WAIT_OBJECT_0 == dw)
		{
			break;
		}
		else if (WAIT_OBJECT_0 + 1 == dw)
		{
			list<shared_ptr<IBitmapSaveTask_Cali>> tmpTaskList;
			PopTask(tmpTaskList);
			list<shared_ptr<IBitmapSaveTask_Cali>>::iterator itB = tmpTaskList.begin();
			list<shared_ptr<IBitmapSaveTask_Cali>>::iterator itE = tmpTaskList.end();
			for (list<shared_ptr<IBitmapSaveTask_Cali>>::iterator it = itB; it != itE; ++it)
			{
				if (*it)
				{
					(*it)->Execute();
				}
			}
		}
		else
		{
			continue;
		}
	}

	return 0;
}

void CBitmapSaveThread_Cali::PushTask(shared_ptr<IBitmapSaveTask_Cali> spTask)
{
	CriticalSectionLock lock(m_lisTaskCS);
	if (m_lisTask.size() < 100)
	{
		m_lisTask.push_back(spTask);
		::SetEvent(m_hTaskEvent);
	}
}

void CBitmapSaveThread_Cali::PopTask(list<shared_ptr<IBitmapSaveTask_Cali>>& taskList)
{
	CriticalSectionLock lock(m_lisTaskCS);
	if (m_lisTask.size() > 0)
	{
		taskList.swap(m_lisTask);
	}
}

void CBitmapSaveThread_Cali::CleanTimerFunc()
{
	AddDiskCheckTask();
}
