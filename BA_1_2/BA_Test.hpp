//BASIC_ALL_1_2 BA_STRING_H
//H
//Writen By BHM
//2021年11月24日 21点11分

//#define USE_OPENCV

#ifndef BA_TEST_H
#define BA_TEST_H

#include "BA_Base.hpp"
#include "BA_CMD.hpp"
#include "BA_Mem.hpp"
#include "BA_Thread.hpp"
#include "BA_Math.hpp"
#include "BA_File.hpp"
#include "BA_String.hpp"
#include "BA_BioInfo.hpp"
#include "BA_JSON.hpp"

namespace ba
{
    namespace test
    {
        namespace _cmd
        {
            void open(void);
            void openlog(void);
            void _missMatch(void);
        }
        namespace _thread
        {
            void WordsCount_SubThr(_LL id, balist<ba::str>& getQ,
                balist<char>& putQ, balist<float>& proc, balist<bool>& sig, void* data);
            void WordsCount(void);
        }
        namespace _json
        {
            void simpleJsonLife(void);
            //https://github.com/nlohmann/json#stl-like-access
            void stlAccess(void);
            void myAccess(void);
            //https://github.com/nlohmann/json#conversion-from-stl-containers
            void convertionWithSTL(void);
            //https://github.com/nlohmann/json#custom-data-source
            void customData(void);
            //https://github.com/nlohmann/json#implicit-conversions
            void conversions(void);
            //https://github.com/nlohmann/json#arbitrary-types-conversions
            void customDataConv(void);
            void concat(void);
            //https://github.com/nlohmann/json#binary-formats-bson-cbor-messagepack-ubjson-and-bjdata
            void binaryFormatStore(void);
        }
        namespace _jieba
        {
            void jiebaDemo(void);
            void jiebaDealFile(void);
            void jiebaClass(void);
        }
        namespace _ui
        {
            void initSDL2(void);
        }
        namespace _comprehensive
        {
            void tensor(void);

            // or use std::tuple for more var to deliver
            void jiebaThreads_SubThr(_LL id, balist<char>& getQ,
                balist<std::pair<char*, std::vector<int>*>>& putQ, balist<float>& proc, balist<bool>& sig, void* data);
            void jiebaThreads(void);

            void glob(void);
        }
    }
}

#endif