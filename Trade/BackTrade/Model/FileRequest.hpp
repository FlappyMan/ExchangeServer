/*
 * @Author: your name
 * @Date: 2020-05-21 20:44:06
 * @LastEditTime: 2020-05-21 20:48:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/FileRequest.hpp
 */ 
#ifndef _FILEREQ_MODEL_HEADER_
#define _FILEREQ_MODEL_HEADER_

#include "../global.h"
class CFileRequest
{
public:
    CFileRequest &operator=(const CFileRequest &obj)
    {
        if (this != &obj)
        {
            this->name = obj.name;
            this->path = obj.path;
            this->size = obj.size;
            this->modifytime = obj.modifytime;
            this->offset = obj.offset;
            this->length = obj.length;
            this->name = obj.name;
            this->token = obj.token;
        }
        return *this;
    }
    CFileRequest &operator=(UPFileRequest *pkg)
    {
        this->name = dynamic_cast<UPFileRequest *>(pkg)->name();
        this->path = dynamic_cast<UPFileRequest *>(pkg)->path();
        this->size = dynamic_cast<UPFileRequest *>(pkg)->size();
        this->modifytime = dynamic_cast<UPFileRequest *>(pkg)->modifytime();
        this->offset = dynamic_cast<UPFileRequest *>(pkg)->offset();
        this->length = dynamic_cast<UPFileRequest *>(pkg)->length();
        this->name = dynamic_cast<UPFileRequest *>(pkg)->name();
        this->token = dynamic_cast<UPFileRequest *>(pkg)->token();
        return *this;
    }

public:
    string name;
    string path;
    uint64_t size;
    uint64_t modifytime;
    uint64_t offset;
    uint32_t length;
    string token;
};

#endif
