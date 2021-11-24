//BASIC_ALL_1_2
//LIB
//Writen By BHM
//2021年11月24日 21点13分

//#define USE_OPENCV
//#define USE_WINDOWS
//#define USE_SDL2

#include"BA_Base.hpp"
#include"BA_File.hpp"

//char* root = mstrdup("E:\\HC\\KOMODO\\Komodo Organism\\");
//List* fileName = listFiles(StringAdd(root,"*.txt",NULL));
List* listFiles(char* dir)
{
    List* pli = List_Init();
    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
    if (handle == -1)
        return (List*)MyBA_Err("Failed to find first file!\n", 1);

    do
    {
        if (!(findData.attrib & _A_SUBDIR) && strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0)
            List_Put(pli, (void*)mstrdup(findData.name));
    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

    _findclose(handle);    // 关闭搜索句柄
    return pli;
}

BA_Dir::BA_Dir(const char* _root)
{
    _ULL _rootLen = strlen(root);
    dirs = List_Init();
    files = List_Init();
    mem = List_Init();
    if (root == NULL || _rootLen < 3)
    {
        MyBA_Err("BA_Dir::BA_Dir(const char* root):root == NULL || strlen(root) < 3,do nothing", 1);
    }
    else
    {
        root = BALLOC_R(_rootLen+1, char, mem);
        if (root != NULL)
        {            
            if (strcpy_s(root, _rootLen + 1, _root) == 0)
            {
                intptr_t handle;
                _finddata_t findData;

                handle = _findfirst(StringAdd_S(root,"\\*",NULL), &findData);    // 查找目录中的第一个文件
                if (handle != -1)
                {                   
                    do
                    {
                        if ((findData.attrib & _A_SUBDIR))
                        {
                            if(strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0)
                                List_Put(dirs, (void*)mstrdup(findData.name));
                        }
                        else
                        {
                            List_Put(files, (void*)mstrdup(findData.name));
                            files->plast->usage = findData.size;
                        }
                    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
                }
                else
                {
                    MyBA_Errs(1,"Failed to find first file! with root:",_root,"do nothing",NULL);
                }
                _findclose(handle);    // 关闭搜索句柄
            }
            else
            {
                MyBA_Errs(1,"BA_Dir::BA_Dir(const char* root):strcpy_s(root, _rootLen + 1, _root) err with _root:",_root,", do nothing",NULL);
            }
        }
        else
        {
            PPW("BALLOC_R(strlen(_root), char, mem) == NULL");
        }
    }
}

void BA_Dir::Print(void)
{
    int js = 0;
    for (_ULL a = 0; a < 100; a++)
        printf("=");
    printf("\n");

    LIST_FORS(char,p,dirs)
        printf("|||%5d - [%10.2fMB] ||| %s\n", js + 1, -1.0, p);
    for (_ULL a = 0; a < 100; a++)
        printf("-");
    printf("\n");

    for(ListDot* dot = files->pfirst;dot != NULL;dot = dot->pnext)
        printf("|||%5d - [%10.2fMB] ||| %s\n", js + 1, (dot->usage) / 1024.0 / 1024.0, (char*)(dot->pdata));
    for (_ULL a = 0; a < 100; a++)
        printf("=");
    printf("\n");
}
