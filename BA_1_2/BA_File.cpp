//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月24日 21点13分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"


_ULL  Get_File_Size(FILE* pf)
{
    fseek(pf, 0, SEEK_SET);
    _ULL begin = ftell(pf);
	fseek(pf, 0, SEEK_END);
	_ULL end = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	return end - begin;
}
char* ReadTXT(const char* path, _ULL loadSize, List* mem)
{
    FILE* pf = NULL;
    if (fopen_s(&pf, path, "r") == 0)
    {
        _ULL size = Get_File_Size(pf);
        if (loadSize > size)
            return (char*)MyBA_Errs(1, "ReadTXT: loadSize > size:", path, " ,return NULL", NULL);
        if (loadSize == 0)
            loadSize = size;
        char* pc = BALLOC_R(loadSize + 1, char, mem);
        if (!pc)
        {
            PPW("OOM");
            return (char*)1;
        }
        fread(pc, 1, loadSize, pf);
        fclose(pf);
        return pc;
    }
    else
    {
        if (pf)
            fclose(pf);
        return (char*)MyBA_Errs(1, "ReadTXT: Err to open file:", path, " ,return NULL", NULL);
    }
}

char* GetFileType(char* ppath, List* mem)
{
    _LL i = 0;
	char* pte = ppath + strlen(ppath) - 1;
	for (; *pte != '.'; pte--, i++);//*pte=='.'  ,  i = type_len
    char* ret = BALLOC_R(i + 1, char, mem);
    strcpy_s(ret, i, pte);
	return ret;
}

bool Check_File_Exist(char* path)
{
	FILE* pf = NULL;
	if (fopen_s(&pf, path, "rb") == 0)
	{
		fclose(pf);
		return 1;
	}
	if (pf == NULL)
		return 0;
	else
		fclose(pf);
	return 0;
}

bool Encrypt_File(char* path, char* Passwords)
{
	return 1;
}

char* StringWrite(FILE* pf, char* pc)
{
	_ULL* plen = MCALLOC(1, _ULL);
    if (plen != NULL)
    {
	    (*plen) = strlen(pc);
	    fwrite(plen, sizeof(_ULL), 1, pf);
	    fwrite(pc, sizeof(char), *plen, pf);
	    free(plen);
    }
    else
    {
        PPW("char* StringWrite(FILE* pf, char* pc):Err to alloc mem, return NULL");
        return NULL;
    }
	return pc;
}

char* StringRead(FILE* pf, List* mem)
{
	_ULL* plen = MCALLOC(1, _ULL);
    if (plen != NULL)
    {
	    fread(plen, sizeof(_ULL), 1, pf);
	    if (*plen == 0)
	    {
            _LL i = ftell(pf);
		    free(plen);
		    return NULL;
	    }
        mem = mem ? mem : pba->STmem;
	    char* pc = BALLOC_R(*plen + 1, char, mem);
        if (pc != NULL)
	        fread(pc, sizeof(char), *plen, pf);
        else
            PPW("char* StringRead(FILE* pf):Err to alloc mem, return NULL");
	    free(plen);
	    return pc;
    }
    else
    {
        PPW("char* StringRead(FILE* pf):Err to alloc mem, return NULL");
        return NULL;
    }
}

//char* root = mstrdup("E:\\HC\\KOMODO\\Komodo Organism\\");
//List* fileName = listFiles(StringAdd(root,"*.txt",NULL));
//List* listFiles(char* dir)
//{
//    List* pli = List_Init();
//    intptr_t handle;
//    _finddata_t findData;
//
//    handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
//    if (handle == -1)
//        return (List*)MyBA_Err("Failed to find first file!\n", 1);
//
//    do
//    {
//        if (!(findData.attrib & _A_SUBDIR) && strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0)
//            List_Put(pli, (void*)mstrdup(findData.name));
//    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
//
//    _findclose(handle);    // 关闭搜索句柄
//    return pli;
//}

BA_Dir::BA_Dir(const char* _root)
{
    if (root)
    {
        _ULL _rootLen = strlen(root);
        if (root == NULL || _rootLen < 3)
        {
            MyBA_Err("BA_Dir::BA_Dir(const char* root):root == NULL || strlen(root) < 3,do nothing", 1);
            isErr = true;
        }
        else
        {
            root = BALLOC_R(_rootLen + 1, char, mem);
            if (root != NULL)
            {
                if (strcpy_s(root, _rootLen + 1, _root) == 0)
                {
                    intptr_t handle;
                    _finddata_t findData;

                    handle = _findfirst(ba::StrAdd(pba->STmem, root, "\\*", NULL), &findData);    // 查找目录中的第一个文件
                    if (handle != -1)
                    {
                        do
                        {
                            if ((findData.attrib & _A_SUBDIR))
                            {
                                if (strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0)
                                    dirs->Put(mstrdup(findData.name, mem));
                            }
                            else
                            {
                                files->Put(mstrdup(findData.name, mem));
                                files->plast->usage = findData.size;
                            }
                        } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
                    }
                    else
                    {
                        MyBA_Errs(1, "Failed to find first file! with root:", _root, ", do nothing", NULL);
                        isErr = true;
                    }
                    _findclose(handle);    // 关闭搜索句柄
                }
                else
                {
                    MyBA_Errs(1, "BA_Dir::BA_Dir(const char* root):strcpy_s(root, _rootLen + 1, _root) err with _root:", _root, ", do nothing", NULL);
                    isErr = true;
                }
            }
            else
            {
                PPW("BALLOC_R(strlen(_root), char, mem) == NULL");
                isErr = true;
            }
        }
    }
}

BA_Dir::BA_Dir(const char* _root, const char* _type)
{

    _ULL _rootLen = strlen(_root);
    mem = List_Init();
    root = BALLOC_R(_rootLen + 1, char, mem);
    if (root == NULL || _type == NULL || _rootLen < 3)
    {
        MyBA_Err("BA_Dir::BA_Dir(const char* root):root == NULL || strlen(root) < 3,do nothing", 1);
        isErr = true;
    }
    else
    {
        if (root != NULL)
        {
            if (strcpy_s(root, _rootLen + 1, _root) == 0)
            {
                intptr_t handle;
                _finddata_t findData;

                handle = _findfirst(ba::StrAdd(pba->STmem, root, "\\*",_type, NULL), &findData);    // 查找目录中的第一个文件
                if (handle != -1)
                {
                    do
                    {
                        if (!(findData.attrib & _A_SUBDIR))
                        {      
                            files->Put(mstrdup(findData.name, mem));
                            files->plast->usage = findData.size;
                        }
                    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
                }
                else
                {
                    MyBA_Errs(1, "Failed to find first file! with root:", _root, ", do nothing", NULL);
                    isErr = true;
                }
                _findclose(handle);    // 关闭搜索句柄
            }
            else
            {
                MyBA_Errs(1, "BA_Dir::BA_Dir(const char* root):strcpy_s(root, _rootLen + 1, _root) err with _root:", _root, ", do nothing", NULL);
                isErr = true;
            }
        }
        else
        {
            PPW("BALLOC_R(strlen(_root), char, mem) == NULL");
            isErr = true;
        }
    }
}

void BA_Dir::Print(void)
{
    int js = 0;
    for (_ULL a = 0; a < 100; a++)
        printf("=");
    printf("\n%s\n",root);

    for(char* p = dirs->Copy(); p; p = dirs->Copy(), js++)
        printf("|||%5d - [%10.2fMB] ||| %s\n", js + 1, -1.0, p);
    for (_ULL a = 0; a < 100; a++)
        printf("-");
    printf("\n");

    for (balistDot<char>* dot = dirs->pfirst; dot; dot = dot->pnext, js++)
        printf("|||%5d - [%10.2fMB] ||| %s\n", js + 1, (dot->usage) / 1024.0 / 1024.0, (char*)(dot->pdata));
    for (_ULL a = 0; a < 100; a++)
        printf("=");
    printf("\n");
}
