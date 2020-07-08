

#include "UBJson.h"


// g++ -c UBJson.cpp -o UBJson.o -I../lib/jsoncpp/include/


int UBJsonPack(char* pBuff,int iBuffSize,Json::Value &root,bool bGoodLook)
{
	string str;
	UBJsonPack(str,root);
	if(str.length()>iBuffSize)return -1;
	memcpy(pBuff,str.data(),str.length());
	return str.length();
}


int UBJsonPack(string &str,Json::Value &root,bool bGoodLook)
{
	std::ostringstream os;

	Json::StreamWriterBuilder writerBuilder;
	if(bGoodLook==false)writerBuilder["indentation"] = "";
	std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
	writer->write(root, &os);
  	str=os.str();
	return str.length();
}



bool UBJsonUnpack(Json::Value &root,char *pBuff,int iBuffSize)
{
	Json::CharReaderBuilder readerBuilder;
	readerBuilder["collectComments"] = false;
	Json::CharReaderBuilder::strictMode(&readerBuilder.settings_);
	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());

//	JSONCPP_STRING err;
   	bool ret = jsonReader->parse(pBuff, pBuff+iBuffSize, &root, NULL);
    	if (ret==false) 
    	{
    		return false;
    	}	
	return true;
}


bool UBJsonUnpack(Json::Value &root,string &str)
{
	Json::CharReaderBuilder readerBuilder;
	readerBuilder["collectComments"] = false;
	Json::CharReaderBuilder::strictMode(&readerBuilder.settings_);
	std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());

   	bool ret = jsonReader->parse(str.data(),str.data()+str.length(), &root, NULL);
    	if (ret==false) 
    	{
    		return false;
    	}	
	return true;
}



