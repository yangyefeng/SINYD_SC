#include "BitmapSaveThread.h"
#include "ConfigInitializer.h"
#include "CommonDef.h"
#include "ijg/jpeglib.h"
#include <imagehlp.h>
#include "SCPluginDef.h"
#include <time.h>
#include <shellapi.h>

#pragma comment(lib, "../Lib/jpeg.lib")
#pragma comment(lib, "imagehlp.lib")

struct SaveTask : public IBitmapSaveTask
{
	SaveTask(const HBITMAP& hbitmap, const string& timeStamp, bool bSaveFull = false)
		: m_bitmap()
		, m_timeStamp(timeStamp)
		, m_bSaveFull(bSaveFull)
	{
		m_bitmap.Load(hbitmap);
	}
	SaveTask(CBitmapEx& hbitmap, const string& timeStamp, bool bSaveFull = false)
		: m_bitmap()
		, m_timeStamp(timeStamp)
		, m_bSaveFull(bSaveFull)
	{
		m_bitmap.Create(hbitmap);
	}
	virtual void Execute() override
	{
		if (!m_bitmap.IsValid())
		{
			return;
		}

		const char* cPath = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheAdress();
		if (cPath != NULL)
		{
			string path = cPath;
			char cpathend = *(path.crbegin());
			if (cpathend != '\\')
			{
				path = path + '\\';
			}

			string::iterator itB = m_timeStamp.begin();
			string::iterator itE = m_timeStamp.end();
			bool bFirstColon = true;
			for (string::iterator it = itB; it != itE; ++it)
			{
				if (*it == ':' && bFirstColon)
				{
					*it = '\\';
					bFirstColon = false;
				}
				else if (*it == ':' && !bFirstColon)
				{
					*it = '_';
				}
			}

			path = path + m_timeStamp + ".jpg";
			::MakeSureDirectoryPathExists(path.c_str());

			DWORD dwSize = 0;
			CBitmapEx tmpBitmap;
			if (m_bSaveFull)
			{
				tmpBitmap.Create(m_bitmap);
			}
			else
			{
				DrawMonitorAreas(m_bitmap, tmpBitmap);
			}
			tmpBitmap.Get24BPPData(NULL, &dwSize);
			if (dwSize > 0)
			{
				LPBYTE lpBitmapData = new BYTE[dwSize];
				tmpBitmap.Get24BPPData(lpBitmapData, &dwSize);

				write_JPEG_file(path.c_str(), lpBitmapData, tmpBitmap.GetWidth(), tmpBitmap.GetHeight(), m_bSaveFull? 50:80);
				if (lpBitmapData)
				{
					delete[] lpBitmapData;
					lpBitmapData = nullptr;
				}
			}
		}
	}

private:
	string m_timeStamp;
	CBitmapEx m_bitmap;
	bool m_bSaveFull;

	void DrawMonitorAreas(CBitmapEx& bitmapSrc, CBitmapEx& bitmapDest)
	{
		bitmapDest.Create(bitmapSrc.GetWidth(), bitmapSrc.GetHeight());
		bitmapDest.Clear();

		const vector<MonitorArea*>* pAreas = ConfiginitializerInstance::GetInstance()->CalibrationConfig()->GetMonitorAreas();
		vector<MonitorArea*>::const_iterator itBegin = pAreas->begin();
		vector<MonitorArea*>::const_iterator itEnd = pAreas->end();
		for (vector<MonitorArea*>::const_iterator it = itBegin; it != itEnd; ++it)
		{
			int type = (*it)->GetMonitorAreaType();
			int x = (*it)->GetMonitorAreaX();
			int y = (*it)->GetMonitorAreaY();
			if (type == SC_PLUGIN_CAPABILITY_TYPE_VALVE)
			{
				DWORD dw = bitmapSrc.GetPixel(x, y);
				bitmapDest.SetPixel(x, y, dw);
				bitmapDest.SetPixel(x - 1, y, dw);
				bitmapDest.SetPixel(x + 1, y, dw);
				bitmapDest.SetPixel(x, y - 1, dw);
				bitmapDest.SetPixel(x, y + 1, dw);
				bitmapDest.SetPixel(x - 2, y, dw);
				bitmapDest.SetPixel(x + 2, y, dw);
				bitmapDest.SetPixel(x, y - 2, dw);
				bitmapDest.SetPixel(x - 1, y - 1, dw);
				bitmapDest.SetPixel(x + 1, y + 1, dw);
				bitmapDest.SetPixel(x + 1, y - 1, dw);
				bitmapDest.SetPixel(x - 1, y + 1, dw);
			}
			else if (type == SC_PLUGIN_CAPABILITY_TYPE_LOADCELL)
			{
				int width = (*it)->GetMonitorAreaWidth();
				int height = (*it)->GetMonitorAreaHeight();
				bitmapDest.Draw(x, y, width, height, bitmapSrc, x, y, width, height);
			}
		}
	}

	void write_JPEG_file(const char * filename, unsigned char * image_buffer, int image_width, int image_height, int quality)
	{
		/* This struct contains the JPEG compression parameters and pointers to
		* working space (which is allocated as needed by the JPEG library).
		* It is possible to have several such structures, representing multiple
		* compression/decompression processes, in existence at once.  We refer
		* to any one struct (and its associated working data) as a "JPEG object".
		*/
		struct jpeg_compress_struct cinfo;
		/* This struct represents a JPEG error handler.  It is declared separately
		* because applications often want to supply a specialized error handler
		* (see the second half of this file for an example).  But here we just
		* take the easy way out and use the standard error handler, which will
		* print a message on stderr and call exit() if compression fails.
		* Note that this struct must live as long as the main JPEG parameter
		* struct, to avoid dangling-pointer problems.
		*/
		struct jpeg_error_mgr jerr;
		/* More stuff */
		FILE * outfile = nullptr;		/* target file */
		JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
		int row_stride;		/* physical row width in image buffer */

							/* Step 1: allocate and initialize JPEG compression object */

							/* We have to set up the error handler first, in case the initialization
							* step fails.  (Unlikely, but it could happen if you are out of memory.)
							* This routine fills in the contents of struct jerr, and returns jerr's
							* address which we place into the link field in cinfo.
							*/
		cinfo.err = jpeg_std_error(&jerr);
		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);

		/* Step 2: specify data destination (eg, a file) */
		/* Note: steps 2 and 3 can be done in either order. */

		/* Here we use the library-supplied code to send compressed data to a
		* stdio stream.  You can also write your own code to do something else.
		* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
		* requires it in order to write binary files.
		*/
		errno_t errFile = fopen_s(&outfile, filename, "wb");
		if (!outfile)
		{
			return;
		}
		jpeg_stdio_dest(&cinfo, outfile);

		/* Step 3: set parameters for compression */

		/* First we supply a description of the input image.
		* Four fields of the cinfo struct must be filled in:
		*/
		cinfo.image_width = image_width; 	/* image width and height, in pixels */
		cinfo.image_height = image_height;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
											/* Now use the library's routine to set default compression parameters.
											* (You must set at least cinfo.in_color_space before calling this,
											* since the defaults depend on the source color space.)
											*/
		jpeg_set_defaults(&cinfo);
		/* Now you can set any non-default parameters you wish to.
		* Here we just illustrate the use of quality (quantization table) scaling:
		*/
		jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

		/* Step 4: Start compressor */

		/* TRUE ensures that we will write a complete interchange-JPEG file.
		* Pass TRUE unless you are very sure of what you're doing.
		*/
		jpeg_start_compress(&cinfo, TRUE);

		/* Step 5: while (scan lines remain to be written) */
		/*           jpeg_write_scanlines(...); */

		/* Here we use the library's state variable cinfo.next_scanline as the
		* loop counter, so that we don't have to keep track ourselves.
		* To keep things simple, we pass one scanline per call; you can pass
		* more if you wish, though.
		*/
		row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */

		while (cinfo.next_scanline < cinfo.image_height) {
			/* jpeg_write_scanlines expects an array of pointers to scanlines.
			* Here the array is only one element long, but you could pass
			* more than one scanline at a time if that's more convenient.
			*/
			row_pointer[0] = &image_buffer[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
			(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		/* Step 6: Finish compression */

		jpeg_finish_compress(&cinfo);
		/* After finish_compress, we can close the output file. */
		fclose(outfile);

		/* Step 7: release JPEG compression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);

		/* And we're done! */
	}

};



/*通过调用ShFileOperation来实现整个目录的删除*/
/*只删除单个目录*/
BOOL SHDeleteFolderA(LPCSTR pstrFolder, BOOL bAllowUndo)
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

struct TimeCleanTask : public IBitmapSaveTask
{
	virtual void Execute() override
	{
		const char* cPath = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheAdress();
		if (cPath != NULL)
		{
			string path = cPath;
			char cpathend = *(path.crbegin());
			if (cpathend != '\\')
			{
				path = path + '\\';
			}

			int dayTime = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheSaveTime();
			time_t curTime;
			time(&curTime);

			list<string> listSubDir;
			FindSubDir(path, "*-*-* *", listSubDir);

			list<string>::iterator itEnd = listSubDir.end();
			for (list<string>::iterator it = listSubDir.begin(); it != itEnd; ++it)
			{
				time_t tDirData = StringDayToIntDay(*it);
				int subDay = (curTime - tDirData) / 86400;
				if (subDay > dayTime)
				{
					string dirFullPath = path + *it;
					BOOL bRes = SHDeleteFolderA(dirFullPath.c_str(), FALSE);
				}
			}
		}
	}

private:
	time_t StringDayToIntDay(const string& strData)
	{
		struct tm t;
		memset(&t, 0, sizeof(tm));
		time_t time;

		sscanf_s(strData.c_str(), "%d-%d-%d %d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour);

		t.tm_year -= 1900;
		t.tm_mon -= 1;
		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;
		time = mktime(&t);
		return time;
	}

	void FindSubDir(string parentPath, const char* pWildcard, list<string>& listSubDir)
	{
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
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				listSubDir.push_back(ffd.cFileName);
			}

			bFind = FindNextFileA(hFind, &ffd);
		}
		FindClose(hFind);

	}
};

struct DiskSpaceCleanTask : public IBitmapSaveTask
{
	virtual void Execute() override
	{
		const char* cPath = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheAdress();
		if (cPath != NULL)
		{
			string path = cPath;
			long maxSize = ConfiginitializerInstance::GetInstance()->SettingConfig()->GetCacheUsedSpace() * 1024 * 1024;		//KB
			char cpathend = *(path.crbegin());
			if (cpathend != '\\')
			{
				path = path + '\\';
			}

			list<SubFolder> folders;
			long lSize = FindSubFolders(path, "*-*-* *", "*_*", folders);
			if (lSize > maxSize)
			{
				long delSize = lSize - maxSize + (maxSize * 30 / 100);
				list<SubFolder>::iterator itEnd = folders.end();
				for (list<SubFolder>::iterator it = folders.begin(); it != itEnd; ++it)
				{
					if (it->size < delSize)
					{
						SHDeleteFolderA((path + it->name).c_str(), FALSE);
						delSize = delSize - it->size;
					}
					else
					{
						list<SubFile>::iterator itFileEnd = it->files.end();
						for (list<SubFile>::iterator itFile = it->files.begin(); itFile != itFileEnd; ++itFile)
						{
							::DeleteFileA((path + it->name + "\\" + itFile->name).c_str());
							delSize = delSize - itFile->size;
							if (delSize < 0)
							{
								break;
							}
						}
					}
				}
			}
		}
	}

private:
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
};

CBitmapSaveThread::CBitmapSaveThread()
{
	m_hTaskEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_CleanTaskTimer.SetTimedEvent(this, &CBitmapSaveThread::CleanTimerFunc);
}


CBitmapSaveThread::~CBitmapSaveThread()
{
	if (m_hTaskEvent)
	{
		::CloseHandle(m_hTaskEvent);
		m_hTaskEvent = nullptr;
	}
}

DWORD CBitmapSaveThread::StartImpl(DWORD dwCreationFlags)
{
	DWORD dw = CThreadBase::StartImpl(dwCreationFlags);
	AddTimeCleanTask();
	AddDiskCleanTask();
	m_CleanTaskTimer.Start(5 * 60 * 1000);
	return dw;
}

void CBitmapSaveThread::StopImpl(int nWaitTime)
{
	m_CleanTaskTimer.Stop();
	CThreadBase::StopImpl(nWaitTime);
}

void CBitmapSaveThread::AddTask(const HBITMAP & hbitmap, const string& time, bool bSaveFull)
{
	shared_ptr<IBitmapSaveTask> spTask(new SaveTask(hbitmap, time, bSaveFull));
	PushTask(spTask);
}
void CBitmapSaveThread::AddTask(CBitmapEx & hbitmap, const string& time, bool bSaveFull)
{
	shared_ptr<IBitmapSaveTask> spTask(new SaveTask(hbitmap, time, bSaveFull));
	PushTask(spTask);
}

void CBitmapSaveThread::AddTimeCleanTask()
{
	shared_ptr<IBitmapSaveTask> spTask(new TimeCleanTask());
	PushTask(spTask);
}
void CBitmapSaveThread::AddDiskCleanTask()
{
	shared_ptr<IBitmapSaveTask> spTask(new DiskSpaceCleanTask());
	PushTask(spTask);
}

DWORD CBitmapSaveThread::Execute()
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
			list<shared_ptr<IBitmapSaveTask>> tmpTaskList;
			PopTask(tmpTaskList);
			list<shared_ptr<IBitmapSaveTask>>::iterator itB = tmpTaskList.begin();
			list<shared_ptr<IBitmapSaveTask>>::iterator itE = tmpTaskList.end();
			for (list<shared_ptr<IBitmapSaveTask>>::iterator it = itB; it != itE; ++it)
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

void CBitmapSaveThread::PushTask(shared_ptr<IBitmapSaveTask> spTask)
{
	CriticalSectionLock lock(m_lisTaskCS);
	m_lisTask.push_back(spTask);
	if (m_lisTask.size() > 100)
	{
		m_lisTask.pop_front();
	}
	::SetEvent(m_hTaskEvent);
}

void CBitmapSaveThread::PopTask(list<shared_ptr<IBitmapSaveTask>>& taskList)
{
	CriticalSectionLock lock(m_lisTaskCS);
	if (m_lisTask.size() > 0)
	{
		taskList.swap(m_lisTask);
	}
}

void CBitmapSaveThread::CleanTimerFunc()
{
	if (m_CleanTaskTimer.GetCount() % 2 == 0)
	{
		AddTimeCleanTask();
	}
	else
	{
		AddDiskCleanTask();
	}
}
